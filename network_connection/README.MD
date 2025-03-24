# Network Connection

This project allows the user to connect to an ESP32 server, interact with its internal LED, and upload the data to a ThingSpeak chart. The user can control the internal LED via a web interface, and the status of the LED is uploaded to ThingSpeak.

## Project Structure

The project consists of the following files:

1. **`main.cpp`** - The main code that creates an access point on the ESP32, allowing remote interaction with the internal LED via a web server.
2. **`secrets-example.h`** - Contains placeholders for Wi-Fi credentials and the ThingSpeak API key. Rename this to `secrets.h` and provide your actual credentials and API key.
3. **`utils.h`** - Contains functions to handle Wi-Fi connection and uploading data to ThingSpeak.
4. **`VSCodeProjectProfile`** - Contains the necessary configurations for running the project in VS Code using PlatformIO.

## Installation & Setup

### Requirements
- ESP32 development board
- PlatformIO IDE installed
- Wi-Fi connection
- ThingSpeak account and API key

### Steps

1. Clone the repository to your local machine:
    ```bash
    git clone https://github.com/your-username/IoT_Projects.git
    ```

2. Navigate to the network_connection folder:
    ```bash
    cd IoT_Projects/network_connection
    ```

3. Install PlatformIO and set up the environment:

    * Open the project in PlatformIO (from VS Code or another IDE that supports PlatformIO).

    * Ensure you have the necessary libraries for ESP32 development. If using PlatformIO, it should install dependencies automatically.

4. Configure the credentials:

    * Copy secrets-example.h to secrets.h.

    * Edit secrets.h to include your Wi-Fi credentials and ThingSpeak API key:

    ```cpp
    #define WIFI_SSID "YOUR NETWORK NAME"
    #define WIFI_PASSWORD "YOUR NETWORK PASSWORD"
    #define ThingSpeak_API_KEY "YOUR API KEY"
    ```

5. VSCode Project Profile Setup:

    * The VSCodeProjectProfile file includes the necessary configurations for PlatformIO. Make sure this file is present in the project directory. It helps VSCode and PlatformIO understand the environment and dependencies needed for the project to run successfully.

    * You don’t need to modify this file unless you want to adjust project settings like board type, upload speed, etc.

6. Upload the code to the ESP32:

* Connect your ESP32 to your computer via USB.

* Use PlatformIO to upload the code to your ESP32 board.

7. After uploading, the ESP32 will start a local web server. Open a web browser and enter the IP address of the ESP32 (usually displayed in the console output of PlatformIO).

## Code Breakdown

### `secrets-example.h`
This file holds sensitive information such as your Wi-Fi credentials and ThingSpeak API key. Rename this file to secrets.h to use it with your actual credentials.

```cpp
#define WIFI_SSID "YOUR NETWORK NAME"
#define WIFI_PASSWORD "YOUR NETWORK PASSWORD"
#define ThingSpeak_API_KEY "YOUR API KEY"
```

### `utils.h`
Contains helper functions for Wi-Fi connectivity and data transmission.

```cpp
void connectToWiFi()
{
    //
}

void uploadToThingSpeak(int value)
{
    //
}
```

### `main.cpp`
This is the main code for the ESP32, which sets up a web server and handles communication with the internal LED. The ESP32 operates in Access Point (AP) Mode, allowing other devices to connect directly to it.

#### Functions in `main.cpp`:

1. **`handle_OnConnect()`**
   - Called when the root URL ("/") is accessed.
   - Sets the internal LED status to OFF and serves an HTML page showing the LED status.

2. **`handle_internalLedOn()`**
   - Called when the "/ledon" URL is accessed.
   - Turns the internal LED ON, uploads the status to ThingSpeak, and serves the updated HTML page with the LED status.

3. **`handle_internalLedOff()`**
   - Called when the "/ledoff" URL is accessed.
   - Turns the internal LED OFF, uploads the status to ThingSpeak, and serves the updated HTML page with the LED status.

4. **`handle_NotFound()`**
   - Called when a non-existent URL is accessed.
   - Sends a 404 "Not Found" response.

5. **`SendHTML(InternalLEDStatus)`**
   - Generates the HTML page that displays the current status of the internal LED and provides buttons to turn it ON or OFF.

## Troubleshooting

If you encounter any issues while using the **Network Connection** project, refer to the following common problems and solutions:

### 1. **Wi-Fi Connection Issues**
- **Problem:** The ESP32 fails to connect to the Wi-Fi network.
- **Solution:** 
   - Double-check the `WIFI_SSID` and `WIFI_PASSWORD` values in the `secrets.h` file to ensure they are correct.
   - Make sure the Wi-Fi network is 2.4 GHz, as the ESP32 does not support 5 GHz networks.
   - Ensure that the ESP32 is within range of the Wi-Fi router.
   - Monitor the serial output to see if there are any connection errors.

### 2. **Unable to Access the Web Server**
- **Problem:** After connecting to the ESP32 Wi-Fi, you cannot access the web server in your browser.
- **Solution:** 
   - Ensure the ESP32 is properly connected to Wi-Fi. Check the serial monitor for the IP address assigned to the ESP32.
   - After connecting to the ESP32 Wi-Fi, open a web browser and type the IP address printed in the serial monitor (e.g., `192.168.x.x`).
   - Make sure that the ESP32 is not overloaded with too many devices connected, as it can limit the number of simultaneous connections.

### 3. **LED Control Not Working**
- **Problem:** The internal LED doesn't respond to the ON/OFF button.
- **Solution:**
   - Verify the LED wiring is correct and that the internal LED is connected to the correct pin (`GPIO2` by default).
   - Ensure that the `InternalLEDPIN` is set to the correct pin in `main.cpp` (it defaults to `2`).
   - Check the web interface for correct HTML buttons and responses.
   - Open the serial monitor and check if any errors or warnings are logged when you attempt to control the LED.

### 4. **Data Not Showing on ThingSpeak**
- **Problem:** The data is not being sent or displayed on ThingSpeak.
- **Solution:**
   - Ensure that the ThingSpeak API key is correctly set in `secrets.h` under `ThingSpeak_API_KEY`.
   - Verify that the ThingSpeak channel is set up correctly to accept data from field 1.
   - Check your network connection to make sure the ESP32 has internet access when uploading data to ThingSpeak.
   - Monitor the serial output for any HTTP errors (e.g., invalid HTTP codes) that could prevent successful data upload.

### 5. **ESP32 Keeps Rebooting**
- **Problem:** The ESP32 repeatedly reboots or resets itself.
- **Solution:** 
   - Check the power supply to the ESP32. Insufficient power can cause the device to reboot unexpectedly.
   - Ensure that the device is not overheating or exposed to environmental conditions outside its operating range.
   - If the problem persists, try commenting out the `uploadToThingSpeak` function in `main.cpp` to see if the issue is related to network operations.

### 6. **ESP32 Fails to Reconnect After Wi-Fi Disconnect**
- **Problem:** The ESP32 doesn't reconnect to Wi-Fi after losing the connection.
- **Solution:**
   - Ensure the `connectToWiFi()` function in `utils.h` handles reconnections correctly. The function should retry connecting to the Wi-Fi network if it fails initially.
   - Double-check that the Wi-Fi network is stable and that the ESP32 is in range.
   - Monitor the serial output for any errors that might indicate why the connection isn't re-established.

### 7. **General Debugging**
- **Problem:** The code is not working as expected, and no clear error message is shown.
- **Solution:**
   - Use the serial monitor to print debug information at various stages in the code (e.g., when connecting to Wi-Fi, when serving a page, etc.).
   - Look for error codes or warnings in the serial output that might provide more context about the issue.
   - Ensure all required libraries are installed and up-to-date in your development environment (PlatformIO/Arduino IDE).

If you continue to experience issues, feel free to create a new issue on the repository with detailed information about the problem and any error messages you encountered.

---