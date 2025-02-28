# Raspberry Pi Pico W Temperature Logger

This project reads the internal temperature sensor of a Raspberry Pi Pico W and uploads the data to ThingSpeak every 180 seconds.

## Project Structure

The project consists of four Python files:

1. **`secrets.py`** - Stores Wi-Fi credentials and the ThingSpeak API key.
2. **`metrics.py`** - Contains the function to read the internal temperature of the Raspberry Pi Pico W.
3. **`utils.py`** - Contains functions to connect to Wi-Fi and upload data to ThingSpeak.
4. **`main.py`** - Uses the other three files to read the temperature and send it to ThingSpeak periodically.

## Installation & Setup

### Requirements
- Raspberry Pi Pico W
- MicroPython firmware installed
- Wi-Fi connection
- ThingSpeak account and API key

### Steps
1. Clone or copy the files to your Raspberry Pi Pico W.
2. Update `secrets.py` with your Wi-Fi credentials and ThingSpeak API key:
   ```python
   SSID = "YOUR NETWORK NAME"
   PASSWORD = "YOUR NETWORK PASSWORD"
   THINGSPEAK_API_WRITE = "YOUR API KEY"
   ```
3. Upload all files to the Raspberry Pi Pico W using Thonny or a similar tool.
4. Run `main.py` to start logging temperature data to ThingSpeak.

## Code Breakdown

### `secrets.py`
This file holds sensitive information such as:
```python
SSID = "YOUR NETWORK NAME"
PASSWORD = "YOUR NETWORK PASSWORD"
THINGSPEAK_API_WRITE = "YOUR API KEY"
```

### `metrics.py`
This module reads the internal temperature sensor of the Pico W:
```python
import machine
adc = machine.ADC(4)

def read_temperature():
    adc_value = adc.read_u16()
    volt = (3.3/65535) * adc_value
    temperature = 27 - (volt - 0.706)/0.001721
    return temperature
```

### `utils.py`
Contains helper functions for Wi-Fi connectivity and data transmission.

#### Connect to Wi-Fi:
```python
def connect_wifi(ssid, password):
    ...
```

#### Upload temperature to ThingSpeak:
```python
def upload_to_thingspeak(api_key, value):
    ...
```

### `main.py`
The main script initializes Wi-Fi, reads temperature data, and uploads it every 180 seconds.
```python
connect_wifi(secrets.SSID, secrets.PASSWORD)

while True:
    sensor_value = read_temperature()
    upload_to_thingspeak(secrets.THINGSPEAK_API_WRITE, sensor_value)
    utime.sleep(180)
```

## Usage
- The script continuously runs, sending temperature data to ThingSpeak.
- The built-in Pico LED blinks as a visual indicator when data is sent.

## How it looks after running
![image](https://github.com/user-attachments/assets/84aa9d2b-5174-4ff0-b990-d37f6ef89149)

- If configured also the matlab code it should look like this
- ![image](https://github.com/user-attachments/assets/1c4a565d-1250-4609-bb3f-9bdaebb7b8a8)
- ![image](https://github.com/user-attachments/assets/97ee6d7c-8d56-4273-95ea-b131a53c9d7a)

## Troubleshooting
- **Wi-Fi not connecting?** Double-check your SSID and password in `secrets.py`; Also check the network used is 2.4G. 
- **Data not showing on ThingSpeak?** Ensure your API key is correct and your channel is set up to receive data.
- **Script not running properly?** Restart the Pico W and verify all dependencies are installed.
