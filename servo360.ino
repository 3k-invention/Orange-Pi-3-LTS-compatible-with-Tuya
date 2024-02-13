#include <Servo.h>

Servo myservo;  // create a servo object

void setup() {
  myservo.attach(9);  // attach the servo to pin 9
  Serial.begin(9600); // initialize serial communication at 9600 bps
  while (!Serial) {
    ; // wait for serial connection
  }
}

void loop() {
  if (Serial.available() > 0) {
    // Wait for data to be available
    String data = Serial.readStringUntil('\n'); // Read data until newline
    int separatorIndex = data.indexOf(','); // Find the index of the separator
    if (separatorIndex != -1) {
      // If a separator is found, parse the data
      int velocity = data.substring(0, separatorIndex).toInt(); // Velocity
      int time = data.substring(separatorIndex + 1).toInt(); // Time

      // Check the ranges of the data
      if (velocity >= 0 && velocity <= 180 && time >= 1 && time <= 255) {
        myservo.write(velocity); // Set the servo velocity
        delay(time * 5); // Wait for the specified time (converted to milliseconds)
        myservo.write(90); // Stop 
      }
    }
  }
}
