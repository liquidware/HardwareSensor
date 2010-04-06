/**
 * SensorPrint
 * Arduino example by Christopher Ladden
 *
 * This example demonstrates sending sensor
 * values to the TouchShield. This guy
 * is simply broadcasting a sensor value
 * as a packet.
 *
 * Notes:
 *      1.) Set the baud rate
 *      2.) Read some analog sensor value
 *      3.) Print the sensor to the TouchShield
 */

#include <HardwareSensor.h>

void setup() {
    Sensor.begin(19200);
}

void loop() {
    int value = analogRead(5);
    Sensor.print("accel", value);
}
