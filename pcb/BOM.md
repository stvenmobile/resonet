# Bill of Materials for ResoNet Dual‑Layer PCB System

## Piezo Disks (Resonator / Sensor)
- 35 mm ceramic piezo disk, ~5 V drive / sensing, pair:
  - 6 disks for resonator side (4 for Layer 1, 2 for Layer 2)
  - 8 disks for sensor side (4 per layer)

## Amplifier Modules
- HiLetgo PAM8403 dual-channel Class D amplifier (typical breakout module)
  - Quantity: 3
  - Use: Layer 1 (2 modules), Layer 2 (1 module)

## ESP32 Module
- ESP32 DevKit (e.g. WROOM-based board with 6 usable PWM outputs)

## Power Supply
- 5 V regulated supply (e.g. 5 V USB power or bench adapter)
- Current rating: ~1 A per board (3 modules + ESP32 ≈ 1.5 A peak)

## Passive Components
- Resistors: 10 kΩ, 1% tolerance × 6
- Capacitors: 0.1 µF ceramic × 6

## MCU–PCB Interconnect
- Female‑header pins / dupont wires: for ESP32 → PAM8403 inputs
- Shielded or twisted pair wire: for piezo disk connections
- Pin headers for sensor board to stack and connect ADS1115

## ADS1115 Sensor Side
- ADS1115 ADC breakout or custom board
  - 1 board per layer (reads 4 sensor disks)
- I²C connection cables to Raspberry Pi (± SDA, SCL, VCC, GND)

## Substrate
- 1″ × 2″ aluminum sheet, 1/16″ or 1/8″ thick (~0.5–1.5 mm)
- Mounting tape or silicone adhesive (non-stiffening)

## Miscellaneous
- Prototyping board for mounting RC filters (or direct wire)
- Zip ties or supports to fix modules near substrate edges
