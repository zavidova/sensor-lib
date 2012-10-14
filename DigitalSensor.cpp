/***
 * This library, demo sketch and docs are available at:
 * http://www.insanegiantrobots.com/arduino/sensorlib.zip
 * @see DigitalSensor.h
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

#include "DigitalSensor.h"

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

void initDigitalSensor(digitalSensor_t &sensor, char* name, int pin, int sampleCount, bool firingState, int firingThreshold,
	int nonFiringThreshold, unsigned long int interval) {
  sensor.name = (char*) calloc(strlen(name)+1, sizeof(char));
  if (sensor.name) {
    strcpy(sensor.name, name);
  }
  sensor.id = pin;
  sensor.firingDurationThreshold = firingThreshold;
  sensor.nonFiringDurationThreshold = nonFiringThreshold;
  sensor.firingState = firingState;
  sensor.sampleCount = sampleCount;
  sensor.samples = (int*)malloc(sizeof(int) * sensor.sampleCount);
  sensor.sampleInterval = interval;
  sensor.lastSampleAt = 0;
  bool currentState = digitalRead(sensor.id);
  // Backfill the sensor's history with the current state
  for (int i=sensor.sampleCount-1; i >= 0; i--) {
    sensor.samples[i] = currentState;
  }
}

DigitalSensor::DigitalSensor(char* name, int pin, int sampleCount, bool firingState, int firingThreshold, int nonFiringThreshold,
  unsigned long int interval) {
  initDigitalSensor(data, name, pin, sampleCount, firingState, firingThreshold, nonFiringThreshold, interval);
}

/**
  If the sample interval has passed, take a new sample and add it to the sensor's history.
**/
void takeDigitalSample(digitalSensor_t &sensor) {
  unsigned long int now = millis();
  if ((now - sensor.lastSampleAt) >= sensor.sampleInterval) {
    // If our state has changed, record a new sampleStart
    for (int i=sensor.sampleCount-1; i > 0; i--) {
      sensor.samples[i] = sensor.samples[i-1];
    }
    sensor.samples[0] = digitalRead(sensor.id);
    sensor.lastSampleAt = millis();
  }
}

void DigitalSensor::takeSample() {
  takeDigitalSample(data);
}

/**
  Report the state that had most recently reached it's threshold.
  If no threshold had been reached in the recorded samples, report the most recent state.
*/
bool isDigitalSensorFiring(digitalSensor_t &sensor) {
  int firingSpanLength = 0;
  int nonFiringSpanLength = 0;
  for (int i=0; i < sensor.sampleCount; i++) {
    if (sensor.samples[i] == sensor.firingState) {
      firingSpanLength += 1;
      nonFiringSpanLength = 0;
    } else {
      nonFiringSpanLength += 1;
      firingSpanLength = 0;
    }
    if (firingSpanLength == sensor.firingDurationThreshold) {
      return true;
    } else if (nonFiringSpanLength == sensor.nonFiringDurationThreshold) {
      return false;
    }
  }
  return sensor.samples[0];
}

bool DigitalSensor::isFiring() {
  return isDigitalSensorFiring(data);
}

char* DigitalSensor::name() {
  return data.name;
}

bool DigitalSensor::lastState() {
  return data.samples[0];
}
