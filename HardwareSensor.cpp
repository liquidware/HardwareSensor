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

/******************************************************************************
 * Includes
 ******************************************************************************/
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiring.h>
#include <HardwareSensor.h>

#if (defined(_TOUCH_SLIDE_) || defined(_TOUCH_STEALTH_))
//TouchShield
    #include <HardwareSerial.h>

    // The TouchShield has hardware serial
    // on Arduino pins 2 (rx), 3 (tx). Use
    // the hardware serial.
    #define SERIAL_AVAILABLE   Serial.available
    #define SERIAL_READ        Serial.read
    #define SERIAL_PRINT(STR)  (Serial.print(STR))
    #define SERIAL_BEGIN(BAUD) (Serial.begin(BAUD))
#else
//Arduino
    #include <AFSoftSerial.h>

    // The Arduino talks to the TouchShield
    // on it's pins 3 (rx) and 2 (tx). Use
    // AFSoftSerial help to do this.
    AFSoftSerial afSerial(3, 2);
    #define SERIAL_AVAILABLE   afSerial.available
    #define SERIAL_READ        afSerial.read
    #define SERIAL_PRINT(STR)  (afSerial.print(STR))
    #define SERIAL_BEGIN(BAUD) (afSerial.begin(BAUD))
#endif

/******************************************************************************
 * Constructors
 ******************************************************************************/

/*************************************************************
 * Sensor
 *  Creates a new sensor. A sensor can do things like
 *  print and read it's name and value.
 */
HardwareSensor::HardwareSensor(void) {
    msg = 0;
    name = 0;
    printDelay = 100;  //start off slow, speedup using setPrintDelay()
    value = 0;
}

/******************************************************************************
 * User API
 ******************************************************************************/

/*************************************************************
 * begin
 *  Begins the sensor, which is
 *  mostly an alias to Serial.begin()
 */
void HardwareSensor::begin(long baud) {
    SERIAL_BEGIN(baud);
}

/*************************************************************
 * getValue
 *  Gets the integer value of the sensor
 */
int HardwareSensor::getValue(void) {
    return value;
}

/*************************************************************
 * read
 *  Gets the integer value of the sensor
 */
int HardwareSensor::read(void) {
    return value;
}

/*************************************************************
 * getName
 *  Gets the string name of the sensor
 */
char* HardwareSensor::getName(void) {
    return name;
}

/*************************************************************
 * setPrintDelay
 *  Sets the delay after Sensor.print()
 */
void HardwareSensor::setPrintDelay(int delay) {
    printDelay = delay;
}

/*************************************************************
 * Available
 *  Scans for a particular sensor of interest.
 *
 *  sensor - The string name of the sensor
 *
 * Notes:
 *  1. Scans a buffer for a sensor
 *  2. Stores the sensor's name
 *  3. Stores the sensors value
 */
uint8_t HardwareSensor::available(void) {

    //Wait for a sensor message from the serial buffer
    msg = Scan(&msgBuff[0], sizeof(msgBuff));

    //Get the sensor name
    name = GetName(msg);

    //Get the sensor value
    value = GetValue(msg);

    return true;
}

/*************************************************************
 * print
 *  Print the sensor name and value to serial
 *      Format: "\r%s\t%d\t\n"
 *      Where %s is the sensor name
 *      Where %d is the sensor value
 */
void HardwareSensor::print(char * sensorName, int sensorValue) {
    char out[MAX_MSG_LEN];
    sprintf(out, "\r%s\t%d\t\n",
            sensorName, sensorValue);
    //Serial.print(out);
    SERIAL_PRINT(out);
    delay(printDelay);
}

/******************************************************************************
 * Private Methods
 ******************************************************************************/

/*************************************************************
 * SensorAvailable
 *  This function checks a sensor message to see
 *  if it's an interesting one.
 */
uint8_t HardwareSensor::Available(char * sensorMsg, char * sensorTarget) {
    char temp[MAX_MSG_LEN];
    char * str;
    char * tok = temp;

    //make a copy
    strcpy(temp, sensorMsg);

    str = strtok_r(NULL,"\t", &tok);
    if (!strcmp(str, sensorTarget)) {
        return true;
    } else {
        return false;
    }
}

/*************************************************************
 * SensorName
 *  This function returns the sensor name
 */
char * HardwareSensor::GetName(char * sensorMsg) {
    static char temp[MAX_MSG_LEN];
    char * str;
    char * tok = temp;

    //make a copy
    strcpy(temp, sensorMsg);

    str = strtok_r(NULL,"\t", &tok);
    return str;
}

/*************************************************************
 * getValue
 *  This function parses a sensor message
 *
 * Returns:
 *  The sensor value in integer form
 */
int HardwareSensor::GetValue(char * sensorMsg) {
    char temp[MAX_MSG_LEN];
    char * valueStr;
    char * tok = temp;

    //make a copy
    strcpy(temp, sensorMsg);

    valueStr = strtok_r(NULL,"\t", &tok);
    valueStr = strtok_r(NULL,"\t", &tok);

    return atoi(valueStr);
}

/*************************************************************
 * Scan
 *  This function polls the serial link for a sensor message
 *  sent from the Arduino.
 *
 *  buff - A buffer with some space
 *  maxLen - the maximum length a sensor message could be

 * Message Format:
 *  This function expects the sensor to have been sent this way:
 *      sprintf(out, "\rMySensor\t%d\t\n", analogRead(5));
 *      mySerial.print(out);
 *      delay(50);
 * Returns:
 *  A pointer to the sensor character string
 *
 * Notes:
 *   You typically parse the sensor using tokens '\t' character as tokens
 */
char * HardwareSensor::Scan(char * buff, int maxLen) {
    char c = 0;
    char index = 0;

    memset(buff, 0, maxLen);  //clear the buffer

    //Busy-wait for the start character
    //while (Serial.read() != '\r') {
    while (SERIAL_READ() != '\r') {
        ;
    }

    //Loop until we see the end character or maxed out.
    while ((c != '\n') && (index < maxLen)) {
        //if (Serial.available() ) {
        if (SERIAL_AVAILABLE() ) {
            //c = Serial.read();
            c = SERIAL_READ();
            buff[index++] = c;
        }
    }
    buff[index] = 0; //terminate the string

    return buff;      //return a pointer
}

// Preinstantiate Objects //////////////////////////////////////////////////////
HardwareSensor Sensor;
