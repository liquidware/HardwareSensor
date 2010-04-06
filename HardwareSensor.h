/*
  HardwareSensor - An Arduino helper library for sensor communication.

  Copyright (c) 2010 Christopher Ladden.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Supported Boards:
            TouchShield Slide : http://www.liquidware.com/shop/show/TSL/TouchShield+Slide
            TouchShield Stealth : http://www.liquidware.com/shop/show/TS/TouchShield+Stealth
            Arduino Duemilanove : http://www.liquidware.com/shop/show/ARD/Arduino+Duemilanove
            Arduino Mega : http://www.liquidware.com/shop/show/AMEGA/Arduino+Mega
            Arduino Diecimila : http://www.liquidware.com/shop/show/ARD/Arduino+Duemilanove

  Usage:
    //Arduino Sketch Example (This guy is broadcasting the sensor)

        void setup() {
            Sensor.begin(19200);
        }

        void loop() {
            int value = analogRead(5);
            Sensor.print("accel", value);
        }

    //-- snip, new sketch -- //

    //TouchShield Sketch Example (This guy is reading the sensor)

        void setup() {
            Sensor.begin(19200);
            background(0);
        }

        void loop() {
            if (Sensor.available()) {
                int value;

                value = Sensor.read();  //get the sensor value
                text(value, 50, 10);             // print the value to the screen
                text(Sensor.getName(), 10, 10);  // print the name to the screen
            }
        }
*/

#ifndef HardwareSensor_h
#define HardwareSensor_h

#include <inttypes.h>

#define MAX_NAME_LEN    10    // the max name length
#define MAX_MSG_LEN     32    // the max possible buffer length

class HardwareSensor
{

    private:
       char msgBuff[MAX_MSG_LEN];           // storage
       char* msg;                           // a pointer to the sensor message
       char* name;                          // a pointer to the message name
       int printDelay;                      // the delay between sensor printing
       int value;                           // the value of the sensor

       char*   Scan(char * buff, int maxLen);
       int     GetValue(char * sensorMsg);
       uint8_t Available(char * sensorMsg, char * sensorTarget);
       char*   GetName(char * sensorMsg);

    public:
       HardwareSensor(void);    //constructor

       uint8_t  available(void);
       int      read(void);                                 // read the integer value of the sensor
       void     print(char * sensorName, int sensorValue);  // print(sensorName, sensorValue) :: prints the sensor to the serial port
       char*    getName(void);                              // return the name of the available sensor
       int      getValue(void);                             // alias to Sensor.read()
       void     setPrintDelay(int);                         // the delay after Sensor.print()
       void     begin(long);                                // alias to Serial.begin()
};

extern HardwareSensor Sensor;

#endif
