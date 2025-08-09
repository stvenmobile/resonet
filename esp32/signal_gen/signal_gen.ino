#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ====== USER CONFIG ======
const char* WIFI_SSID = "googlewifi";    // <-- set your SSID
const char* WIFI_PASS = "abc123def456"; // <-- set your password
// =========================

// Pins
const int leftPWM  = 4;
const int rightPWM = 5;

// Defaults (UI ranges)
int freqL  = 440;   // Hz
int freqR  = 523;   // Hz
int dutyL8 = 127;   // 0..255 (UI) -> scaled to 12-bit
int dutyR8 = 127;   // 0..255 (UI)

// Channel enable states
bool leftEnabled  = true;
bool rightEnabled = true;

// LEDC config
const uint8_t  resBits = 12;                  // 12-bit => 0..4095 duty
const uint32_t dutyMax = (1u << resBits) - 1; // 4095

// Safety limits
const uint32_t F_MIN = 1;
const uint32_t F_MAX = 40000;

AsyncWebServer server(80);

// ---------- Utils ----------
static inline uint32_t clampU32(uint32_t v, uint32_t lo, uint32_t hi){
  if (v < lo) return lo;
  if (v > hi) return hi;
  return v;
}

// Scale 0..255 (UI) -> 0..dutyMax (LEDC)
static inline uint32_t scaleDuty(uint32_t d8){
  return (d8 * dutyMax + 127) / 255;
}

static String jsonStatus() {
  // Report both UI duty (0..255) and effective duty (0..4095)
  uint32_t effL = leftEnabled  ? scaleDuty(clampU32(dutyL8, 0, 255)) : 0;
  uint32_t effR = rightEnabled ? scaleDuty(clampU32(dutyR8, 0, 255)) : 0;

  String ip = (WiFi.status() == WL_CONNECTED) ? WiFi.localIP().toString() : "0.0.0.0";
  String s = "{";
  s += "\"ip\":\"" + ip + "\",";
  s += "\"freqL\":" + String(freqL) + ",";
  s += "\"freqR\":" + String(freqR) + ",";
  s += "\"dutyL8\":" + String(dutyL8) + ",";
  s += "\"dutyR8\":" + String(dutyR8) + ",";
  s += "\"dutyL_eff\":" + String(effL) + ",";
  s += "\"dutyR_eff\":" + String(effR) + ",";
  s += "\"leftEnabled\":" + String(leftEnabled ? "true" : "false") + ",";
  s += "\"rightEnabled\":" + String(rightEnabled ? "true" : "false") + ",";
  s += "\"resolution_bits\":" + String(resBits);
  s += "}";
  return s;
}

static void sendJSON(AsyncWebServerRequest* req, const String& body, int status=200) {
  AsyncWebServerResponse *res = req->beginResponse(status, "application/json", body);
  res->addHeader("Cache-Control", "no-store");
  req->send(res);
}

// ---------- UI ----------
String page() {
  String html = R"HTML(
  <!doctype html><html><head><meta name=viewport content="width=device-width, initial-scale=1">
  <title>ResoNet Control</title>
  <style>
    :root{--fg:#111;--mut:#666}
    body{font-family:system-ui;margin:20px;max-width:720px;color:var(--fg)}
    h3{margin:.25rem 0 1rem}
    .grid{display:grid;grid-template-columns:1fr 1fr;gap:16px}
    .card{border:1px solid #ddd;border-radius:10px;padding:12px;box-shadow:0 1px 2px rgba(0,0,0,.04)}
    label{display:block;font-size:.9rem;color:var(--mut);margin:.25rem 0}
    input{width:100%;padding:.45rem;border:1px solid #ccc;border-radius:8px}
    .row{margin:.5rem 0}
    .btnbar{display:flex;gap:8px;flex-wrap:wrap;margin-top:.5rem}
    button{padding:.5rem .9rem;border-radius:8px;border:1px solid #ccc;background:#f7f7f7;cursor:pointer}
    button.primary{background:#eef6ff;border-color:#b6d4ff}
    .status{font-size:.9rem;color:var(--mut)}
    code{background:#f2f2f2;padding:.1rem .3rem;border-radius:4px}
    .mono{font-family:ui-monospace, SFMono-Regular, Menlo, Consolas, monospace}
  </style></head><body>
  <h3>ESP32 PWM Control (STA mode)</h3>

  <div class=grid>
    <div class=card>
      <h4>Left Channel</h4>
      <form action="/set" method="get">
        <div class=row>
          <label>Frequency (Hz)</label>
          <input name=freqL type=number min=1 max=40000 required>
        </div>
        <div class=row>
          <label>Duty (0-255)</label>
          <input name=dutyL type=number min=0 max=255 required>
        </div>
        <div class=btnbar>
          <button class=primary type=submit name="leftOn" value="1">Start Left</button>
          <button type=submit name="leftOn" value="0">Stop Left</button>
          <button type=submit>Apply (Left)</button>
        </div>
      </form>
      <p class=status>State: <b id=ls></b> • F=<span id=fl></span> Hz • Duty=<span id=dl></span></p>
    </div>

    <div class=card>
      <h4>Right Channel</h4>
      <form action="/set" method="get">
        <div class=row>
          <label>Frequency (Hz)</label>
          <input name=freqR type=number min=1 max=40000 required>
        </div>
        <div class=row>
          <label>Duty (0-255)</label>
          <input name=dutyR type=number min=0 max=255 required>
        </div>
        <div class=btnbar>
          <button class=primary type=submit name="rightOn" value="1">Start Right</button>
          <button type=submit name="rightOn" value="0">Stop Right</button>
          <button type=submit>Apply (Right)</button>
        </div>
      </form>
      <p class=status>State: <b id=rs></b> • F=<span id=fr></span> Hz • Duty=<span id=dr></span></p>
    </div>
  </div>

  <div class=card style="margin-top:16px">
    <h4>API</h4>
    <p class=mono>GET <code>/status</code></p>
    <p class=mono>GET <code>/startL</code> • <code>/stopL</code> • <code>/startR</code> • <code>/stopR</code></p>
    <p class=mono>GET <code>/set?freqL=440&dutyL=127&leftOn=1</code></p>
    <p class=mono>GET <code>/set?freqR=523&dutyR=64&rightOn=0</code></p>
    <p class=status>Resolution: 12-bit (0–4095 internal). Lower frequencies enabled.</p>
  </div>

  <script>
    const curr = {
      freqL: )HTML";
  html += String(freqL);
  html += R"HTML(, freqR: )HTML";
  html += String(freqR);
  html += R"HTML(, dutyL: )HTML";
  html += String(dutyL8);
  html += R"HTML(, dutyR: )HTML";
  html += String(dutyR8);
  html += R"HTML(, leftOn: )HTML";
  html += String(leftEnabled ? 1 : 0);
  html += R"HTML(, rightOn: )HTML";
  html += String(rightEnabled ? 1 : 0);
  html += R"HTML( };

    // Left form fields
    const lf = document.querySelectorAll('form')[0];
    lf.querySelector('[name=freqL]').value = curr.freqL;
    lf.querySelector('[name=dutyL]').value = curr.dutyL;

    // Right form fields
    const rf = document.querySelectorAll('form')[1];
    rf.querySelector('[name=freqR]').value = curr.freqR;
    rf.querySelector('[name=dutyR]').value = curr.dutyR;

    // Status text
    fl.textContent = curr.freqL;
    fr.textContent = curr.freqR;
    dl.textContent = curr.dutyL;
    dr.textContent = curr.dutyR;
    ls.textContent = curr.leftOn ? "Running" : "Stopped";
    rs.textContent = curr.rightOn ? "Running" : "Stopped";
  </script>

  </body></html>
  )HTML";
  return html;
}

// ---------- PWM control ----------
void writeOutputs() {
  uint32_t dL = leftEnabled  ? scaleDuty(clampU32(dutyL8, 0, 255)) : 0;
  uint32_t dR = rightEnabled ? scaleDuty(clampU32(dutyR8, 0, 255)) : 0;
  ledcWrite(leftPWM,  dL);
  ledcWrite(rightPWM, dR);
}

void applyPwmInit() {
  ledcAttach(leftPWM,  clampU32(freqL, F_MIN, F_MAX), resBits);
  ledcAttach(rightPWM, clampU32(freqR, F_MIN, F_MAX), resBits);
  writeOutputs();
}

void applyPwmLive() {
  ledcChangeFrequency(leftPWM,  clampU32(freqL, F_MIN, F_MAX), resBits);
  ledcChangeFrequency(rightPWM, clampU32(freqR, F_MIN, F_MAX), resBits);
  writeOutputs();
}

// ---------- WiFi ----------
void connectWifiSTA() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.printf("Connecting to %s", WIFI_SSID);
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) {
    delay(300);
    Serial.print(".");
  }
  Serial.println();
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WiFi connected. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connect timed out; continuing anyway. (Server will fail without network.)");
  }
}

// ---------- HTTP ----------
void handleSet(AsyncWebServerRequest *req){
  if (req->hasParam("freqL"))  freqL  = req->getParam("freqL")->value().toInt();
  if (req->hasParam("freqR"))  freqR  = req->getParam("freqR")->value().toInt();
  if (req->hasParam("dutyL"))  dutyL8 = req->getParam("dutyL")->value().toInt();
  if (req->hasParam("dutyR"))  dutyR8 = req->getParam("dutyR")->value().toInt();

  if (req->hasParam("leftOn"))  leftEnabled  = (req->getParam("leftOn")->value().toInt()  != 0);
  if (req->hasParam("rightOn")) rightEnabled = (req->getParam("rightOn")->value().toInt() != 0);

  freqL  = clampU32(freqL,  F_MIN, F_MAX);
  freqR  = clampU32(freqR,  F_MIN, F_MAX);
  dutyL8 = clampU32(dutyL8, 0, 255);
  dutyR8 = clampU32(dutyR8, 0, 255);

  applyPwmLive();
  req->redirect("/");
}

void handleStatus(AsyncWebServerRequest *req){
  sendJSON(req, jsonStatus());
}

void handleStartL(AsyncWebServerRequest *req){
  leftEnabled = true;
  writeOutputs();
  sendJSON(req, jsonStatus());
}

void handleStopL(AsyncWebServerRequest *req){
  leftEnabled = false;
  writeOutputs();
  sendJSON(req, jsonStatus());
}

void handleStartR(AsyncWebServerRequest *req){
  rightEnabled = true;
  writeOutputs();
  sendJSON(req, jsonStatus());
}

void handleStopR(AsyncWebServerRequest *req){
  rightEnabled = false;
  writeOutputs();
  sendJSON(req, jsonStatus());
}

void setup() {
  Serial.begin(115200);
  delay(200);

  connectWifiSTA();
  applyPwmInit();

  // Pages & API
  server.on("/",      HTTP_GET, [](AsyncWebServerRequest *req){ req->send(200, "text/html", page()); });
  server.on("/set",   HTTP_GET, [](AsyncWebServerRequest *req){ handleSet(req); });
  server.on("/status",HTTP_GET, [](AsyncWebServerRequest *req){ handleStatus(req); });
  server.on("/startL",HTTP_GET, [](AsyncWebServerRequest *req){ handleStartL(req); });
  server.on("/stopL", HTTP_GET, [](AsyncWebServerRequest *req){ handleStopL(req); });
  server.on("/startR",HTTP_GET, [](AsyncWebServerRequest *req){ handleStartR(req); });
  server.on("/stopR", HTTP_GET, [](AsyncWebServerRequest *req){ handleStopR(req); });

  server.begin();
  Serial.println("HTTP server started.");
}

void loop() {
  // AsyncWebServer handles requests
}
