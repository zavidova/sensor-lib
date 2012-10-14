/***
 *
 * This library, demo sketch and docs are available at:
 * http://www.insanegiantrobots.com/arduino/sensorlib.zip
 * @see PingSensor.h
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
 * Provides both Object Oriented and non-OO interfaces to parallax ping
 * sensors.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 */

#ifndef PingSensor_h
#define PingSensor_h

#include <stdint.h>

#include <AnalogSensor.h>

#define PING_PULSE_MICROS 2
#define PING_ECHO_MICROS 5

/**
  Class to represent a Single pin PING Sensor that fires when its current
  value shifts more than specified thresholds:
   - A sample that is more than N percent from the average of the last S samples.
   - A set of samples where the minimum sample of more than N from the maximum one.
  The sample values are inches from the sensor.
  The class is an AnalogSensor with a different method of getting its current value.
  The PIN is a digital IO pin, not an analog one.
  The shift can be an absolute value shift or in the specified direction.
***/
class PingSensor : public AnalogSensor {
public:
PingSensor(char* name, int d_pin, int sample_count, unsigned long int thresholdPct,
  unsigned long int thresholdDelta, bool fireThresholdDirectionSensitive, unsigned long int interval)
  : AnalogSensor(name, d_pin, sample_count, thresholdPct, thresholdDelta, fireThresholdDirectionSensitive,
    interval)
    {};
  void takeSample();
};
#endif
