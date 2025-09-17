# Piezo Step → Light (ESP32)

Footstep-activated lighting: a plate with **6 piezo vibration modules** detects a step and turns an **AC E27 bulb** ON for **1 second** (then OFF if no new steps).  
Controller: **ESP32**. Switching: **4-ch 5V relay** (IN1 used now; others reserved for future bulbs).

## How it works
Piezos output **spikes** when the plate vibrates. We **sum** 6 sensors through resistors into one ESP32 ADC.  
If the spike crosses a threshold, we turn the relay ON and hold it for 1 s.

## Quick start
1. Build the breadboard per `/hardware/breadboard/piezo_breadboard_layout.png`.
2. Upload **`firmware/piezo-test.ino/`** to see live signals in Serial Plotter.
3. Tune `MARGIN` in the test sketch until hits are clean.
4. Upload **`firmware/main.ino/`** to control the relay.
5. Wire the AC side safely (see `/hardware/safety-notes-mains.md`).

## ESP32 pins (default)
- **Piezo SUM → GPIO34 (ADC1)**  
- **Relays:** IN1=GPIO26, IN2=GPIO27, IN3=GPIO25, IN4=GPIO33  
- Set `RELAY_ACTIVE_HIGH` in code to match your relay board jumpers.

## AC wiring (MY/UK color code)
- **Brown = Live (L)** → **Fuse** → **Relay COM** → **Relay NO** → **Bulb L**  
- **Blue = Neutral (N)** → **Bulb N**  
- Use a proper insulated enclosure and strain relief.

## License
MIT
