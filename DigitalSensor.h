/***
 * This library, demo sketch and docs are available at:
 * http://www.insanegiantrobots.com/arduino/sensorlib.zip
 * @see SensorLib.h
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>)
 *
 * Copyright (c) 2010 by Raymond Blum
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Classes, Data structures, constants and functions for Analog Sensors
 * that provide one value on an analog input pin and Digital Sensors that
 * provide one value (firing/not firing) on one digital pin.
 * 2010
 * @author("Raymond Blum" <raymond@insanegiantrobots.com>)
 *
 * Provides both Object Oriented and non-OO interfaces to 1 pin sensors.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef DigitalSensor_h
#define DigialSensor_h

#include <stdint.h>

/**
  Struct to hold all data for a digital sensor
*/
typedef struct {
  char* name;                           // name for this sensor
  int id;                	 	// digital pin number
  bool firingState;			// The firing state of this sensor, HIGH or LOW
  unsigned long int sampleInterval;	// in millis
  int *samples;				// the historical data
  int sampleCount;			// how many samples[]
  unsigned long int lastSampleAt;	// The timestamp of the most recent sample;
  int firingDurationThreshold;		// The minimum samples the firing state is held to consider this sensor as firing
  int nonFiringDurationThreshold;	// The minimum samples the non-firing state is held before we consider this sensor as not firing 
}  
digitalSensor_t;

/**
  Setup the digital sensor on the specified pin.
*/
void initDigitalSensor(digitalSensor_t &sensor, char* name, int pin, int sampleCount, bool firingState, int firingThreshold,
	int nonFiringThreshold, unsigned long int sampleInterval);

/**
  Take a sample if the sample interval since the last sample has passed.
*/
void takeDigitalSample(digitalSensor_t&);

/**
  Report if the sensor has reached the thresholds specified when it was initialized.
*/
bool isDigitalSensorFiring(digitalSensor_t&);

/**
  Class to represent a Single pin Digital Sensor that fires when its current value matches the specified firing state
  for the specified number of consecutive samples.
*/
class DigitalSensor {
private:
  digitalSensor_t data;
public:
  DigitalSensor(char* name, int pin, int sampleCount, bool firingState, int firingThreshold, int nonFiringThreshold,
    unsigned long int sampleInterval);
  char* name();
  void takeSample();
  bool isFiring();
  bool lastState();
  const digitalSensor_t getData() { return data; }
};
#endif
