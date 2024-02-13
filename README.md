
# Implementation of a system on Orange Pi/Raspberry Pi compatible with the Tuya application

The application was developed for the Orange Pi 3 LTS microcontroller and designed to be compatible with other Orange Pi products, requiring only minor modifications. With appropriate customizations, the application can also be ported to the Raspberry Pi platform. The application's functionality includes LED control, integration with the HC-SR04 motion sensor, DHT22 temperature and humidity sensor, and Servo360 control. The GPIO pin control layer is implemented in C, while the main application code, written in Python, calls these functions. Servo360 is controlled by Arduino Uno, code in repository, communication via UART.

## If you have Orange Pi 3 LTS or other Orange's product, first prepare environment

To ensure you have Git installed on your system, run the following command then install wiringOP lib for GPIO's pins control

```bash
mkdir home/orangepi/Desktop/tuya
cd home/orangepi/Desktop/tuya
sudo apt-get install -y git
git clone https://github.com/orangepi-xunlong/wiringOP.git
cd wiringOP
./build clean
./build
```
Create a bash script to manipulate GPIO pins by Python. Begin by creating a new script file:
```bash
sudo nano /usr/local/bin/set_GPIO.sh
```
Paste the following script into the editor. This script will control the state of a specified GPIO pin:

```bash
#!/bin/bash

# First argument is the pin number
PIN_NUMBER="$1"
# Second argument is the state (0 or 1)
DESIRED_STATE="$2"

# Validate input
if [ -z "$PIN_NUMBER" ] || [ -z "$DESIRED_STATE" ]; then
    echo "Usage: $0 <pin_number> <state (0 or 1)>"
    exit 1
fi

if [ "$DESIRED_STATE" != "0" ] && [ "$DESIRED_STATE" != "1" ]; then
    echo "Error: State must be 0 or 1."
    exit 2
fi

# Set pin as output
gpio mode "$PIN_NUMBER" out

# Set the desired state for the pin
gpio write "$PIN_NUMBER" "$DESIRED_STATE"
```

Make the script executable:

```bash
sudo chmod +x /usr/local/bin/set_GPIO.sh
```

## Installing the Tuya Package

Clone the Tuya SDK for Python and install it:

```bash
git clone https://github.com/tuya/tuyaos-link-sdk-python.git
python3 -m pip install ./tuyaos-link-sdk-python
```

## Library Installation and File Preparation

Install necessary libraries:

```bash
pip install pyserial
pip install paho-mqtt==1.5.1
```
In the case of Oranga Pi 3 LTS, communication with Arduino via UART from USB can be blocked by the brltty library, it can be removed

```bash
sudo apt-get remove brltty
```

Copy files from the repository to a directory home/orangepi/Desktop/tuya/tuyaos-link-sdk-python/examples. Ensure you include the following files:

- `dht22.c`
- `hc_sensor.c`
- `control_pin.py`
- `outlet.py`

Compile the C programs for sensors reading:

```bash
gcc -o hc_sensor hc_sensor.c -lwiringPi
gcc -o dht22 dht22.c -lwiringPi
```

Make the compiled programs executable:

```bash
sudo chmod +x hc_sensor
sudo chmod +x dht22
```

Configuring Sudo Permissions
Use  
```bash
sudo visudo
```
to add permissions for running the sensor programs without requiring a password, add on the end of file:

```plaintext
orangepi ALL=(ALL) NOPASSWD: /home/orangepi/Desktop/tuya/tuyaos-link-sdk-python/examples/hc_sensor
orangepi ALL=(ALL) NOPASSWD: /home/orangepi/Desktop/tuya/tuyaos-link-sdk-python/examples/dht22
```

## Setting Up Tuya Application

Create an application through the Tuya Developer Portal. If unfamiliar with this process, refer to the provided "link" for a step-by-step guide.

After obtaining your product ID, fill it in the `outlet.py` script. Then, run the script to control your device:

```bash
python3 ./outlet.py
```

Follow these instructions meticulously to ensure a smooth setup for your project.
