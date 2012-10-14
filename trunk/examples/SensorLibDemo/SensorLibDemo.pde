/**
 * Sensor Library example showing two analog and one digital sensors.
 *
 * Documentation and links at
 *  http://www.insanegiantrobots.com/arduino/sensorlib.html
 * This demo also uses my RgbLed library at
 *  http://www.insanegiantrobots.com/arduino/rgbled.html
 *
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>) 
 **/


#include <RgbLed.h>
#include <SensorLib.h>

// Print sensor data to Serial?
#define DEBUG true

#define APIN_CDS 11
#define APIN_MIC 12
#define PIN_PIR 8

#define PIN_STATUS_LED_RED 10
#define PIN_STATUS_LED_GREEN 5
#define PIN_STATUS_LED_BLUE 9

// Settings for a generic CDS with a 10K potentiometer sampling over a 20th of a second
#define CDS_SAMPLE_INTERVAL 10
#define CDS_SAMPLE_COUNT 5
#define CDS_FIRE_THRESHOLD_PCT 20
#define CDS_FIRE_THRESHOLD_DELTA 0
#define CDS_FIRE_PEAKS_ONLY false

// Settings for a Sparkfun audio amp breakout board sampling over a half second
#define MIC_SAMPLE_INTERVAL 10
#define MIC_SAMPLE_COUNT 50
#define MIC_FIRE_THRESHOLD_PCT 0
#define MIC_FIRE_THRESHOLD_DELTA 25
#define MIC_FIRE_PEAKS_ONLY true

// Settings for a PIR sampling over a second
#define PIR_SAMPLE_INTERVAL 250
#define PIR_SAMPLE_COUNT 4
#define PIR_FIRING_STATE HIGH
#define PIR_FIRING_THRESHOLD 1
#define PIR_NON_FIRING_THRESHOLD 1

// Our PIR can take 10 seconds to initialize
#define PIR_CALLIBRATION_DELAY 10000

AnalogSensor sensor_cds("CDS", APIN_CDS, CDS_SAMPLE_COUNT, CDS_FIRE_THRESHOLD_PCT, CDS_FIRE_THRESHOLD_DELTA, CDS_FIRE_PEAKS_ONLY, CDS_SAMPLE_INTERVAL);
AnalogSensor sensor_mic("MIC", APIN_MIC, MIC_SAMPLE_COUNT, MIC_FIRE_THRESHOLD_PCT, MIC_FIRE_THRESHOLD_DELTA, MIC_FIRE_PEAKS_ONLY, MIC_SAMPLE_INTERVAL);
DigitalSensor sensor_pir("PIR", PIN_PIR, PIR_SAMPLE_COUNT, PIR_FIRING_STATE, PIR_FIRING_THRESHOLD, PIR_NON_FIRING_THRESHOLD, PIR_SAMPLE_INTERVAL);
RgbLedCommonCathode led_rgb(PIN_STATUS_LED_RED, PIN_STATUS_LED_GREEN, PIN_STATUS_LED_BLUE,true);

int color_red, color_green, color_blue;

void setup() {
  Serial.begin(9600);
  Serial.println("setup()");
  led_rgb.delayCyclingColors(PIR_CALLIBRATION_DELAY);
}

void loop() {
  // Tell the Sensors to sample often, they'll only record one if they've
  // reached their individual sampling intervals
  sampleSensors();
  showSensorStates();
}

void setLedColor() {
  color_red = (sensor_mic.isFiring()?255:0);
  color_green = (sensor_cds.isFiring()?255:0);
  color_blue = (sensor_pir.isFiring()?255:0);
  led_rgb.setColor(color_red, color_green, color_blue);
}

/**
  Mix the LED's color to reflect the state of our 3 sensors.
  If we're debugging, write each sensor's values to Serial.
*/
void showSensorStates() {
  setLedColor();

  if ( DEBUG ) {
    showAnalogSensor(sensor_cds);
    showAnalogSensor(sensor_mic);
    showDigitalSensor(sensor_pir);
  }
}

/**
  Give each sensor a chance to record a sample.
*/
void sampleSensors() {
  sensor_mic.takeSample();
  sensor_pir.takeSample();
  sensor_cds.takeSample();
}

/**
  Write the specified sensor's sampled data, thresholds and derived levels to Serial.
*/
void showAnalogSensor(AnalogSensor sensor) {
  Serial.print(sensor.name());
  Serial.print("-->");
  for (int s=0;s<sensor.getData().sampleCount;s++) {
    Serial.print(sensor.getData().samples[s]);
    Serial.print(".");
  }
  Serial.print(" ");
  Serial.print(sensor.lastState());
  Serial.print("[");
  Serial.print(sensor.currentMinReading());
  Serial.print("-");
  Serial.print(sensor.currentMaxReading());
  Serial.print("]:");
  Serial.print(sensor.currentDelta());
  Serial.print(" ");
  Serial.print(sensor.currentAverageReading());
  Serial.print(":");
  Serial.print(sensor.currentVariance());
  Serial.print("(");
  Serial.print(sensor.currentPercentOfAverage());
  Serial.print(",");
  Serial.print(sensor.currentVariancePercent());
  Serial.print(")");
  Serial.print("->");
  Serial.print(sensor.isFiring());
  Serial.println("");
}

/**
  Write the specified sensor's sampled data, thresholds and derived levels to Serial.
*/
void showDigitalSensor(DigitalSensor sensor) {
  Serial.print(sensor.name());
  Serial.print("-->");
  for (int s=0;s<sensor.getData().sampleCount;s++) {
    Serial.print(sensor.getData().samples[s]);
    Serial.print(".");
  }
  Serial.print(" ");
  Serial.print(sensor_pir.lastState());
  Serial.print("->");
  Serial.print(sensor_pir.isFiring());
  Serial.println("");
}
