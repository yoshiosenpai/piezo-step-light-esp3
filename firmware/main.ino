// ===== Piezo → Relay =====
// Code By Solehin Rizal
// - 6 piezos summed to GPIO34 via 10k each, ~100k to GND
// - 4-ch relay: IN1=26, IN2=27, IN3=25, IN4=33 (change if needed)

const int PIEZO_ADC_PIN = 34;           // keep on ADC1
const int RELAY_PINS[]   = {26, 27, 25, 33};
const int NUM_RELAYS     = 4;
int CHANNELS_TO_USE      = 1;           // how many channels to switch together

// Match your relay board jumpers:
const bool RELAY_ACTIVE_HIGH = true;    // set false if your board is active-LOW

// Filtering & detection (no hardware cap)
const int   SAMPLE_US    = 2000;        // ~500 Hz
const float EMA_ALPHA    = 0.04f;       // 0.03..0.07
int         MARGIN       = 180;         // raise if false hits, lower if misses
const unsigned long REFRACT_MS = 250;   // ignore new hits for this long

// Hold time for the light
const unsigned long HOLD_MS = 1000;     // 1 second after last hit

float ema = 0.0f;
unsigned long lastHitMs = 0;
unsigned long lightUntil = 0;

// Median-of-5 to tame single-sample glitches
int adcMedian5() {
  int v[5];
  for (int i = 0; i < 5; ++i) v[i] = analogRead(PIEZO_ADC_PIN);
  // tiny sort
  for (int i = 0; i < 5; ++i)
    for (int j = i+1; j < 5; ++j)
      if (v[j] < v[i]) { int t=v[i]; v[i]=v[j]; v[j]=t; }
  return v[2];
}

inline void relayWrite(int idx, bool on) {
  int pin = RELAY_PINS[idx];
  if (RELAY_ACTIVE_HIGH) digitalWrite(pin, on ? HIGH : LOW);
  else                   digitalWrite(pin, on ? LOW  : HIGH);
}
void setAllControlled(bool on) {
  for (int i = 0; i < CHANNELS_TO_USE; ++i) relayWrite(i, on);
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);                          // 0..4095
  analogSetPinAttenuation(PIEZO_ADC_PIN, ADC_11db);  // ~0..3.3V

  for (int i = 0; i < NUM_RELAYS; ++i) {
    pinMode(RELAY_PINS[i], OUTPUT);
    relayWrite(i, false);
  }

  // Initialize baseline
  long sum = 0;
  for (int i = 0; i < 150; ++i) { sum += analogRead(PIEZO_ADC_PIN); delay(2); }
  ema = sum / 150.0f;
}

void loop() {
  int raw = adcMedian5();

  // Only update baseline when we're not far above it,
  // so big steps don't "drag" the baseline upward too much.
  if (raw < ema + (MARGIN / 2)) {
    ema += EMA_ALPHA * (raw - ema);
  } else {
    // small decay toward raw to keep baseline sane
    ema += (EMA_ALPHA * 0.25f) * (raw - ema);
  }

  int thresh = (int)ema + MARGIN;

  // Hit detection + refractory
  unsigned long now = millis();
  bool hit = false;
  if (raw > thresh && (now - lastHitMs) > REFRACT_MS) {
    hit = true;
    lastHitMs = now;
    lightUntil = now + HOLD_MS;   // extend 1-second window
  }

  // Drive relays
  if (now < lightUntil) setAllControlled(true);
  else                  setAllControlled(false);

  // Debug (optional)
  // Serial.print(raw); Serial.print('\t');
  // Serial.print((int)ema); Serial.print('\t');
  // Serial.print(thresh); Serial.print('\t');
  // Serial.println(hit ? 1 : 0);

  delayMicroseconds(SAMPLE_US);
}
