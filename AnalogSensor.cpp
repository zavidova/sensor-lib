/***
 * This library, demo sketch and docs are available at:
 * http://www.insanegiantrobots.com/arduino/sensorlib.zip
 * @see AnalogSensor.h
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>)
 *
 * Copyright (c) 2010 by Raymond Blum
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 */

#include "AnalogSensor.h"

/******************************************************************************
 * Wiring/Arduino Includes
 ******************************************************************************/
extern "C" {
  // AVR LibC Includes
  #include <inttypes.h>
  #include <stdlib.h>
  #include <string.h>
  #include "WConstants.h"

  // Wiring Core Includes
  #undef abs
  #include "WConstants.h"

  // Wiring Core Prototypes
  //void pinMode(uint8_t, uint8_t);
  //void digitalWrite(int, uint8_t);
}

int analogSensorValue(analogSensor_t &sensor) {
  return analogRead(sensor.id);
}

/***
 Setup the analog sensor on the designated pin
*/
void initAnalogSensor(analogSensor_t &sensor, char* name, int pin, int sample_count, unsigned long int thresholdPct, unsigned long int thresholdDelta, bool fireThresholdDirectionSensitive,
unsigned long int interval) {
  sensor.name = (char*) calloc(strlen(name)+1, sizeof(char));
  if (sensor.name) { // did we get the memory?
    strcpy(sensor.name, name);
  }
  sensor.id = pin;
  sensor.fireThresholdPct = thresholdPct;
  sensor.fireThresholdDelta = thresholdDelta;
  sensor.fireThresholdDirectionSensitive = fireThresholdDirectionSensitive;
  sensor.sampleCount = sample_count;
  sensor.samples = (int*)malloc(sizeof(int) * sensor.sampleCount);
  sensor.sampleInterval = interval;
  sensor.lastSampleAt = 0;
  // Backfill the sensor's history with the current level
  for (int i=sensor.sampleCount-1; i >= 0; i--) {
    sensor.samples[i] = analogRead(pin);
  }
}

AnalogSensor::AnalogSensor(char* name, int pin, int sample_count, unsigned long int thresholdPct, unsigned long int thresholdDelta, bool fireThresholdDirectionSensitive,
	unsigned long int interval) {
  initAnalogSensor(data, name, pin, sample_count, thresholdPct, thresholdDelta, fireThresholdDirectionSensitive, interval);
}

/***
  If this sensor has reached its sampling interval, take a new value reading usi
ng the
  collection function provided.
*/
void SensorSample_(analogSensor_t &sensor, int(*getValueFunction)(analogSensor_t
&)) {
  unsigned long int now = millis();
  if ((now - sensor.lastSampleAt) >= sensor.sampleInterval) {
    for (int i=sensor.sampleCount-1; i > 0; i--) {
      sensor.samples[i] = sensor.samples[i-1];
    }
    sensor.lastSampleAt = millis();
    sensor.samples[0] = getValueFunction(sensor);  }
}

void takeAnalogSensorSample(analogSensor_t &sensor) {
  SensorSample_(sensor, analogSensorValue);
}

void AnalogSensor::takeSample() {
  takeAnalogSensorSample(data);
}

int currentAnalogSensorAverageReading(analogSensor_t &sensor) {
  int averageReading = 0;
  for (int i=0; i < sensor.sampleCount; i++) {
    averageReading += sensor.samples[i];
  }
  averageReading /= sensor.sampleCount;
  return averageReading;
}

int currentAnalogSensorMaxReading(analogSensor_t &sensor) {
  int maxReading = 0;
  for (int i=0; i < sensor.sampleCount; i++) {
    maxReading = max(maxReading, sensor.samples[i]);
  }
  return maxReading;
}

int currentAnalogSensorMinReading(analogSensor_t &sensor) {
  int minReading = sensor.samples[0];
  for (int i=1; i < sensor.sampleCount; i++) {
    minReading = min(minReading, sensor.samples[i]);
  }
  return minReading;
}

int currentAnalogSensorPercentOfAverage(analogSensor_t &sensor) {
  int averageReading = currentAnalogSensorAverageReading(sensor);
  int percent = ((long int)sensor.samples[0] * 100) / averageReading;
  return percent;
}

int currentAnalogSensorVariance(analogSensor_t &sensor) {
  int averageReading = currentAnalogSensorAverageReading(sensor);
  int variance = sensor.samples[0] - averageReading;
  return variance; 
}

int currentAnalogSensorVariancePercent(analogSensor_t &sensor) {
  int variancePercent = currentAnalogSensorPercentOfAverage(sensor) - 100;
  return variancePercent; 
}

/**
  Return the highest delta between the current state and the samples'
  max or min
**/
int currentAnalogSensorDelta(analogSensor_t &sensor) {
  int max = currentAnalogSensorMaxReading(sensor);
  int min = currentAnalogSensorMinReading(sensor);
  int current = analogSensorLastState(sensor);
  int delta = (current - min);
  if (!sensor.fireThresholdDirectionSensitive) {
    delta = max(delta, (max - current));
  }
  return delta;
}

int AnalogSensor::currentMaxReading() {
  return currentAnalogSensorMaxReading(data);
}

int AnalogSensor::currentMinReading() {
  return currentAnalogSensorMinReading(data);
}

int AnalogSensor::currentAverageReading() {
  return currentAnalogSensorAverageReading(data);
}

char* AnalogSensor::name() {
  return data.name;
}

int AnalogSensor::currentDelta() {
  return currentAnalogSensorDelta(data);
}

int AnalogSensor::currentVariance() {
  return currentAnalogSensorVariance(data);
}

int AnalogSensor::currentVariancePercent() {
  return currentAnalogSensorVariancePercent(data);
}

int AnalogSensor::currentPercentOfAverage() {
  return currentAnalogSensorPercentOfAverage(data);
}

/**
  The sensor is firing if the current reading's variance is higher than the
  pctFiringThreshold for this sensor, or the current reading is either the
  samples' max or min and the current delta is higher than the
  deltaFiringThreshold for this sensor.
**/
bool isAnalogSensorFiring(analogSensor_t &sensor) {
  int variance = currentAnalogSensorVariance(sensor);

  int delta = currentAnalogSensorDelta(sensor);

  bool isFiringByPct = false;
  bool isFiringByDelta = false;

  if (sensor.fireThresholdPct != 0) {
    if (sensor.fireThresholdDirectionSensitive && sensor.fireThresholdPct < 0 && variance < 0) {
      isFiringByPct = (sensor.fireThresholdPct > variance);
    } 
    else {
      isFiringByPct = (abs(sensor.fireThresholdPct) < abs(variance));
    }
  }

  if (sensor.fireThresholdDelta != 0) {
    isFiringByDelta = (
      (analogSensorLastState(sensor) == currentAnalogSensorMinReading(sensor))
      || (analogSensorLastState(sensor) == currentAnalogSensorMaxReading(sensor))
    );
    isFiringByDelta = isFiringByDelta
      && (currentAnalogSensorDelta(sensor) >= sensor.fireThresholdDelta);
  }
 
  return (isFiringByPct || isFiringByDelta);
}

bool AnalogSensor::isFiring() {
  return isAnalogSensorFiring(data);
}

int analogSensorLastState(analogSensor_t &sensor) {
  return sensor.samples[0];
}

int analogSensorLastDelta(analogSensor_t &sensor) {
  return (sensor.sampleCount==1)?0:sensor.samples[1]-sensor.samples[0];
}

int AnalogSensor::lastDelta() {
  return analogSensorLastDelta(data);
}

int AnalogSensor::lastState() {
  return analogSensorLastState(data);
}
