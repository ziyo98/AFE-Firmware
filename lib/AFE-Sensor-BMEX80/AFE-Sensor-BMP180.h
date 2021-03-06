/* AFE Firmware for smart home devices, Website: https://afe.smartnydom.pl/ */

#ifndef _AFE_Sensor_BMP180_h
#define _AFE_Sensor_BMP180_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <AFE-BMEX80-Sensor-Structure.h>
#include <AFE-I2C-Structure.h>
#include <Adafruit_BMP085.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class AFESensorBMP180 {

private:
  Adafruit_BMP085 bme;

public:
  BMEX80 *configuration;
  BMEX80_DATA data;

  /* Constructor: entry parameter is GPIO number where Sensor is connected to */
  AFESensorBMP180();

  boolean begin(BMEX80 *, I2CPORT *);

  boolean read();

};

#endif
