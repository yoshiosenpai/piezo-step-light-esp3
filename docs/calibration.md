# Calibration

1. Flash `/firmware/piezo-test.ino`.
2. Open **Serial Plotter @115200**; step on different spots.
3. Note typical peaks (e.g., 1200–3000).  
4. In the relay sketch, set `MARGIN` ≈ 30–40% below typical peaks.
5. If idle false triggers: raise `MARGIN` or increase `REFRACT_MS` to 300 ms.
