/***
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
 * See file LICENSE.txt for further informations on licensing terms.
 */

#include "PingSensor.h"

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

void SensorSample_(analogSensor_t &sensor,
                     int(*getValueFunction)(analogSensor_t&));

int pingSensorValue(analogSensor_t &sensor) {
  pinMode(sensor.id, OUTPUT);
  digitalWrite(sensor.id, LOW);
  delayMicroseconds(PING_PULSE_MICROS);
  digitalWrite(sensor.id, HIGH);
  delayMicroseconds(PING_ECHO_MICROS);
  digitalWrite(sensor.id, LOW);
  pinMode(sensor.id, INPUT);
  int echo_duration_in_microseconds = pulseIn(sensor.id, HIGH, 1000000L);
  // Sounds travels at 74.746 uSecs per inch, divide by 2 to allow for round trip delay
  return echo_duration_in_microseconds / 74 / 2;
}

void takePingSensorSample(analogSensor_t &sensor) {
  SensorSample_(sensor, pingSensorValue);
}

void PingSensor::takeSample() {
  takePingSensorSample(data);
}
