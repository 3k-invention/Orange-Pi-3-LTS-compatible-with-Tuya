#!/usr/bin/env python3
"""
File name: outlet.py
Author: Marcin Woszczek
Date Created: 13-02-2024
Python Version: 3.10

Description:
Implementation of a system on Orange Pi/Raspberry Pi compatible with the Tuya application

License: Apache License, Version 2.0
The Apache License 2.0 is a free software license that allows for the free use, modification, and distribution of the code,
provided that a copy of the Apache 2.0 License is included and changes to the code are marked. The full text of the license can be found at
http://www.apache.org/licenses/LICENSE-2.0

Using this license ensures that the code is available to a wide community, encouraging others to improve
the project.

"""

import subprocess
import time
import coloredlogs
import serial
from threading import Thread
from control_pin import set_pin
from tuyalinksdk.client import TuyaClient
from tuyalinksdk.console_qrcode import qrcode_generate

coloredlogs.install(level='DEBUG')

client = TuyaClient(productid='lbogd8zpt32ymugd',
                    uuid='uuid01893e889da14827',
                    authkey='j7tUpibNdygN0AwacFIs4pP4wEjnxlR6')


def on_connected():
    print('Connected.')


def on_qrcode(url):
    qrcode_generate(url)


def on_reset(data):
    print('Reset:', data)


def on_dps(dps):
    print('DataPoints:', dps)
    # LED
    if dps.get('101') is not None:
        set_pin(0, 1 if dps['101'] else 0)
    # Servo
    global servo_vel, servo_time
    if '102' in dps:
        servo_vel = dps['102']
    if '103' in dps:
        servo_time = dps['103']
    if '104' in dps and dps['104']:
        with serial.Serial('/dev/ttyUSB0', 9600, timeout=2) as arduino:
            velocity = int(servo_vel)
            time_value = int(servo_time)
            if 0 <= velocity <= 180 and 1 <= time_value <= 255:
                command = f"{velocity},{time_value}\n"
                arduino.write(command.encode())
    # Send data
    client.push_dps(dps)


client.on_connected = on_connected
client.on_qrcode = on_qrcode
client.on_reset = on_reset
client.on_dps = on_dps
client.connect()
client.loop_start()


def call_and_read():
    result_dht22 = subprocess.run(['sudo', './dht22'],
                                  capture_output=True, text=True)
    return result_dht22.stdout


def get_distance():
    result_distance = subprocess.run(['sudo', './hc_sensor'], capture_output=True)
    return result_distance.returncode if result_distance.returncode <= 255 else None


def update_dps_periodically():
    global value_temperature, value_distance
    while True:
        output_dht22_f = call_and_read()
        if 'Temperature =' in output_dht22_f:
            temperature_str = output_dht22_f.split('Temperature =')[1].split('*')[0].strip()
            value_temperature = int(float(temperature_str))
        pre_value_distance = get_distance()
        if pre_value_distance is not None:
            value_distance = int(pre_value_distance)

        # Prepare appropriate data
        dps_update = {
            '105': value_temperature,
            '106': value_distance
        }

        # send data
        client.push_dps(dps_update)

        # wait
        time.sleep(1)


dps_update_thread = Thread(target=update_dps_periodically)
dps_update_thread.start()

while True:
    time.sleep(1)
