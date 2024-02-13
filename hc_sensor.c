#include <stdio.h>
#include <string.h>
#include <wiringPi.h>

// Define GPIO pins for the ultrasonic sensor
#define TRIG 5
#define ECHO 7

/**
 * Initializes GPIO pins for the ultrasonic sensor.
 */
void setup() {
    wiringPiSetup(); // Initialize wiringPi
    pinMode(TRIG, OUTPUT); // Set trigger pin as output
    pinMode(ECHO, INPUT); // Set echo pin as input
    digitalWrite(TRIG, LOW); // Ensure trigger pin is low
    delay(30); // Wait for sensor to settle
}

/**
 * Measures distance and returns it in centimeters.
 * 
 * Returns:
 *     int: Distance in centimeters.
 */
int getDistanceInCm() {
    // Trigger the sensor to send a pulse
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // Wait for echo start
    while(digitalRead(ECHO) == LOW);

    // Record time when echo is detected
    long startTime = micros();
    
    // Wait for echo end
    while(digitalRead(ECHO) == HIGH);
    
    // Calculate travel time and distance
    long travelTime = micros() - startTime;
    int distance = travelTime / 58;

    return distance;
}

int main(void) {
    setup(); // Initialize sensor setup

    // Measure distance
    int distance = getDistanceInCm();
    return distance; // Return successfully
}
