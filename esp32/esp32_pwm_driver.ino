const int pwmPins[] = {12, 13, 14, 15, 16, 17};  // 6 channels
int weights[6] = {128, 128, 128, 128, 128, 128};  // default duty cycle

void setup() {
  for (int i = 0; i < 6; i++) {
    ledcSetup(i, 5000, 8);        // Channel, 5kHz, 8-bit resolution
    ledcAttachPin(pwmPins[i], i); // Attach PWM channel to GPIO
    ledcWrite(i, weights[i]);
  }

  Serial.begin(115200);  // UART for receiving weight updates
}

void loop() {
  if (Serial.available() >= 6) {
    for (int i = 0; i < 6; i++) {
      weights[i] = Serial.read();
      ledcWrite(i, weights[i]);
    }
  }
}
