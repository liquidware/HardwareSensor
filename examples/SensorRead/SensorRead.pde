/**
 * SensorRead
 * TouchShield example by Christopher Ladden
 *
 * This example demonstrates reading Arduino
 * sensor values on the TouchShield. This guy
 * is simply reading sensor packets.
 *
 * Notes:
 *      1.) Set the baud rate
 *      2.) Look for an available sensor
 *      3.) Read the sensor
 *      4.) Display the sensor on screen
 */

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
