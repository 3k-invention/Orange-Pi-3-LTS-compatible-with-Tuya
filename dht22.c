#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Constants
#define MAX_TIMINGS 85
#define DHT_PIN 8  // GPIO Pin used to connect to the DHT sensor.

// Function Declarations
int readDHTData(void);

// Function Definitions
int readDHTData(void) {
    uint8_t lastState = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0, i;
    int data[5] = {0, 0, 0, 0, 0};

    // Pull pin down for 20 milliseconds
    pinMode(DHT_PIN, OUTPUT);
    digitalWrite(DHT_PIN, LOW);
    delay(20);

    // Prepare to read the pin
    pinMode(DHT_PIN, INPUT);

    // Detect change and read data
    for (i = 0; i < MAX_TIMINGS; i++) {
        counter = 0;
        while (digitalRead(DHT_PIN) == lastState) {
            counter++;
            delayMicroseconds(3);
            if (counter == 255) {
                break;
            }
        }
        lastState = digitalRead(DHT_PIN);
        if (counter == 255) break;

        // Ignore first 3 transitions
        if ((i >= 4) && (i % 2 == 0)) {
            // Shove each bit into the storage bytes
            data[j / 8] <<= 1;
            if (counter > 16)
                data[j / 8] |= 1;
            j++;
        }
    }

    // Check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
    if ((j >= 40) && (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF))) {
        float tempCelsius = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
        if (data[2] & 0x80) {
            tempCelsius = -tempCelsius;
        }
        printf("Temperature = %.1f *C\n", tempCelsius);
        return 0; // Success
    } else {
        printf("Data not good, skip\n");
        return 1; // Failure
    }
}

int main(void) {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Failed to initialize wiringPi\n");
        exit(1);
    }

    readDHTData();

    return 0;
}
