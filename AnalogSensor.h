/***
 *
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
 * Classes, Data structures, constants and functions for Analog Sensors
 * that provide one value on an analog input pin.
 * 2010
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>)
 *
 * Provides both Object Oriented and non-OO interfaces to 1 pin sensors.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef AnalogSensor_h
#define AnalogSensor_h

#include <stdint.h>

/**
  Struct to hold all data for an analog sensor
*/
typedef struct {
  char* name;                           // name of sensor
  int id;                		// analog pin number
  bool fireThresholdDirectionSensitive; // is the abs(threshold) or threshold used to determine firing status?
  int fireThresholdPct;   		// an integer, percent  0 == do not use
  int fireThresholdDelta;   		// an integer           0 == do not use
  unsigned long int sampleInterval;  	// in millis
  int *samples;				// the historical data, access as an array
  int sampleCount;       		// how many samples[] to keep
  unsigned long int lastSampleAt;  	// the timestamp of the most recent sample;
} analogSensor_t;

/***
 Setup the analog sensor on the designated pin
*/
void initAnalogSensor(char* name, analogSensor_t &sensor, int pin, int sample_count, unsigned long int threshold, bool fireThresholdDirectionSensitive,
  unsigned long int interval);

void takeAnalogSensorSample(analogSensor_t&);

int currentAnalogSensorAverageReading(analogSensor_t&);

int currentAnalogSensorMaxReading(analogSensor_t&);

int currentAnalogSensorMinReading(analogSensor_t&);

int currentAnalogSensorDelta(analogSensor_t&);

int currentAnalogSensorVariance(analogSensor_t&);

int currentAnalogSensorVariancePercent(analogSensor_t&);

int currentAnalogSensorPercentOfAverage(analogSensor_t&);

/**
  Report if the sensor has reached the thresholds specified when it was initialized.
*/
bool isAnalogSensorFiring(analogSensor_t&);

int analogSensorLastState(analogSensor_t&);

/**
  Class to represent a Single pin Analog Sensor that fires when its current value shifts more than
  specified thresholds:
   - A sample that is more than N percent from the average of the last S samples.
   - A set of samples where the minimum sample of more than N from the maximum one.
  The shift can be an absolute value shift or in the specified direction. 
*/
class AnalogSensor {
protected:
  analogSensor_t data;
public:
  AnalogSensor(char* name, int a_pin, int sample_count,
  unsigned long int thresholdPct, unsigned long int thresholdDelta,
  bool fireThresholdDirectionSensitive, unsigned long int interval);
  char* name();
  int currentVariance();
  int currentVariancePercent();
  int currentMaxReading();
  int currentMinReading();
  int currentDelta();
  int currentPercentOfAverage();
  int currentAverageReading();
  int lastDelta();
  bool isFiring();
  void takeSample();
  int lastState();
  const analogSensor_t getData() { return data; }
};
#endif
