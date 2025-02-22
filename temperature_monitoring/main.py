import utime
import random
import secrets  # Contains Wi-Fi credentials & ThingSpeak API key
from utils import connect_wifi, upload_to_thingspeak
from metrics import read_temperature
from picozero import pico_led

# Ensure Wi-Fi is connected before running
connect_wifi(secrets.SSID, secrets.PASSWORD)

# Loop to send data every 180 seconds
while True:
    pico_led.on()
    sensor_value = read_temperature() 
    upload_to_thingspeak(secrets.THINGSPEAK_API_WRITE, sensor_value)
    
    # This is just a visual queue to know when the sensor has given a value
    utime.sleep(5)
    pico_led.off()
    utime.sleep(175)