# Wiring Table (Low Voltage)

| Subsystem         | From                  | To                       | Notes                           |
|---                |---                    |---                       |---                              |
| Piezo power       | Each module VCC       | ESP32 3V3                | 3.3 V only                      |
| Piezo ground      | Each module GND       | GND                      | Common ground                   |
| Piezo signal      | Each AO               | 10 kΩ resistor → **SUM** | 6x pieces                       | 
| Bleed             | **SUM**               | ~100 kΩ → GND            | Use 10×10 kΩ in series          |
| ADC | **SUM**     | ESP32 **GPIO34**      | ADC1 pin                 |
| Relay control     | ESP32 GPIO26/27/25/33 | Relay IN1/IN2/IN3/IN4    | Match `RELAY_ACTIVE_HIGH`       |
| Relay power       | 5 V PSU               | Relay VCC                | ≥1 A recommended                |
| Common GND        | Relay GND             | ESP32 GND                | Must be shared                  |

# AC Side (One bulb on CH1)
- Brown (Live) → **Fuse** → **Relay COM (CH1)** → **Relay NO (CH1)** → Bulb Live  
- Blue (Neutral) → Bulb Neutral  
