import network
import utime
import urequests

# Initialize Wi-Fi
wlan = network.WLAN(network.STA_IF)

# Function to connect to Wi-Fi
def connect_wifi(ssid, password):
    if not wlan.isconnected():
        print("Connecting to Wi-Fi...")
        wlan.active(True)
        wlan.connect(ssid, password)

        if wlan.isconnected():
            print("Connected to Wi-Fi:", wlan.ifconfig())
        else:
            print("Failed to connect. Retrying...")
            utime.sleep(1)
            connect_wifi(ssid, password)  # Recursive retry
    else:
        print("Connected to Wi-Fi:", wlan.ifconfig())
    
# Function to upload data to ThingSpeak
def upload_to_thingspeak(api_key, value):
    if not wlan.isconnected():
        print("Wi-Fi lost! Reconnecting...")
        connect_wifi()

    try:
        url = f"https://api.thingspeak.com/update?api_key={api_key}&field1={value}"
        response = urequests.get(url)
        print(f"Data sent: {value}. Response: {response.text}")
        response.close()
    except Exception as e:
        print("Error sending data:", e)
        utime.sleep(10)  # Wait before retrying
        upload_to_thingspeak(api_key, value)  # Retry sending
