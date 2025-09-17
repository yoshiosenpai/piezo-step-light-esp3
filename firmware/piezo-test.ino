// ===== Piezo Test  — robust filtering =====
// Code by Solehin Rizal
// View: Tools -> Serial Plotter @ 115200
// Columns: raw    ema    thresh    hit(0/1)

const int PIEZO_ADC_PIN = 34;        // ADC1-only pin
const int SAMPLE_US      = 2000;     // ~500 Hz
const float EMA_ALPHA    = 0.04f;    // 0.03..0.07 (smaller = smoother)
int MARGIN               = 180;      // raise to 200–280 if you see false hits
const unsigned long REFRACT_MS = 250; // dead-time to ignore after a hit

float ema = 0.0f;
unsigned long lastHitMs = 0;

// Take N fast reads and return the median (kills single-sample spikes)
int adcMedian5() {
  int v[5];
  for (int i = 0; i < 5; ++i) v[i] = analogRead(PIEZO_ADC_PIN);
  // sort 5 (tiny, simple)
  for (int i = 0; i < 5; ++i)
    for (int j = i+1; j < 5; ++j)
      if (v[j] < v[i]) { int t=v[i]; v[i]=v[j]; v[j]=t; }
  return v[2]; // median
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);                        // 0..4095
  analogSetPinAttenuation(PIEZO_ADC_PIN, ADC_11db); // ~0..3.3V

  // Init baseline
  long sum = 0;
  for (int i = 0; i < 150; ++i) { sum += analogRead(PIEZO_ADC_PIN); delay(2); }
  ema = sum / 150.0f;
}

void loop() {
  // Median-of-5 to suppress glitches (important when no hardware cap)
  int raw = adcMedian5();

  // Baseline (EMA)
  ema += EMA_ALPHA * (raw - ema);

  // Dynamic threshold
  int thresh = (int)ema + MARGIN;

  // Hit detection with refractory
  int hit = 0;
  unsigned long now = millis();
  if (raw > thresh && (now - lastHitMs) > REFRACT_MS) {
    hit = 1;
    lastHitMs = now;
  }

  // Plotter output
  Serial.print(raw);   Serial.print('\t');
  Serial.print((int)ema); Serial.print('\t');
  Serial.print(thresh); Serial.print('\t');
  Serial.println(hit);

  delayMicroseconds(SAMPLE_US);
}
