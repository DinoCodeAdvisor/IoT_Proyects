#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <time.h>
#include "secrets.h"
#include "utils.h"

/**
 * Routes used for poll
 */
const char* poll_route = "/poll/makevote";
const char* data_route = "/data/";

/**
 * Other devices IP Addresses
 */
IPAddress devicesIPs[6] = {
    IPAddress(192, 168, 3, 100),
    IPAddress(192, 168, 3, 101),
    IPAddress(192, 168, 3, 102),
    IPAddress(192, 168, 3, 103),
    IPAddress(192, 168, 3, 104),
    IPAddress(192, 168, 3, 105)
};

/**
 * Device current statuses
 */
String deviceStatus[6] = {"disconnected","disconnected","disconnected","disconnected","disconnected","disconnected"};
String lastPingSuccessful[6] = {"Never","Never","Never","Never","Never","Never"};
String lastPingAttempt[6] = {"Never","Never","Never","Never","Never","Never"};
int deviceValues[6] = { -1, -1, -1, -1, -1, -1 };

/**
 * NTP Setup
 */
WiFiUDP udp;
NTPClient timeClient(udp, "pool.ntp.org", 0, 60000);

/**
 * Functions calls
 */
String getCurrentDateTime();
bool pingDevices();
bool retrieveData();
bool uploadData();

/**
 * Function to ping all the IPs to make a Poll
 */
bool pingDevices(WiFiClient client)
{
    String timeString = getCurrentDateTime();
    HTTPClient http;
    int i = 0;
    for (int i = 0; i < 6; i++) {
        
        String deviceIP = "http://" + devicesIPs[i].toString() + poll_route;

        Serial.println("Requesting to route: " + deviceIP);

        http.begin(deviceIP);

        int httpCode = http.GET();

        Serial.println(httpCode);

        if(httpCode != HTTP_CODE_OK)
        {
            Serial.println("Failed to get response from device: " + String(i+1));

            deviceStatus[i] = "disconnected";
            lastPingAttempt[i] = timeString;
        }

        if(httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            
            Serial.print("Payload:");
            Serial.println(payload);

            if(payload.indexOf(WATCHDOG_KEY) != -1)
            {
                deviceStatus[i] = "connected";
                lastPingSuccessful[i] = timeString;
                lastPingAttempt[i] = timeString;

                Serial.println("Device " + String(i + 1) + " connected at " + lastPingSuccessful[i]);
            }
            else
            {
                deviceStatus[i] = "disconnected";
                lastPingAttempt[i] = timeString;

                Serial.println("Device " + String(i + 1) + " did not contain the correct WATCHDOG_KEY. ");
            }
        }

        http.end();
    }

    return true;
}

bool retrieveData(WiFiClient client)
{
    HTTPClient http;
    for (int i = 0; i < 6; i++) 
    {
        
        String deviceIP = "http://" + devicesIPs[i].toString() + data_route + WATCHDOG_KEY;

        Serial.println("Requesting to route: " + deviceIP);

        http.begin(deviceIP);

        int httpCode = http.GET();

        Serial.println(httpCode);

        if(httpCode != HTTP_CODE_OK)
        {
            Serial.println("Failed to get response from device: " + String(i+1));

        }

        if(httpCode == HTTP_CODE_OK)
        {
            String payload = http.getString();
            
            Serial.print("Payload:");
            Serial.println(payload);

            if(payload.indexOf(WATCHDOG_KEY) != -1 && payload.indexOf("data") != -1)
            {

                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, payload);

                deviceValues[i] = doc["data"];

                Serial.println("Device " + String(i + 1) + " connected at " + lastPingSuccessful[i]);
            }
            else
            {

                Serial.println("Device " + String(i + 1) + " did not contain the correct WATCHDOG_KEY. ");
            }
        }

        http.end();
    }

    return true;
}

void uploadData(WiFiClient client)
{
    for (int i = 0; i < 6; i++) 
    {
        
        if(deviceStatus[i] == "connected" && deviceValues[i] != -1)
        {
            uploadToThingSpeak(deviceValues[i]);

            Serial.println("Device " + String(i + 1) + " data uploaded. Halting server for cooldown.");

            delay(15000);
        }

        if(deviceStatus[i] != "connected")
        {
            Serial.println("Device " + String(i + 1) + " not connected, skipping.");
        }
    }
}

// Function to get the current date and time as a formatted string
String getCurrentDateTime() {
    // Ensure NTP time is updated
    timeClient.update();
    
    // Get the current time in seconds since Epoch (as unsigned long)
    unsigned long epochTime = timeClient.getEpochTime();
    
    // Cast epochTime to time_t
    time_t rawTime = (time_t)epochTime;
    
    // Convert epoch time to a tm struct
    struct tm* timeinfo = localtime(&rawTime);

    // Get today's date and time components
    int year = timeinfo->tm_year + 1900; // tm_year gives years since 1900
    int month = timeinfo->tm_mon + 1;    // tm_mon gives months since January (0-11)
    int day = timeinfo->tm_mday;         // tm_mday gives the day of the month (1-31)
    int hour = timeinfo->tm_hour;        // tm_hour gives hours (0-23)
    int minute = timeinfo->tm_min;       // tm_min gives minutes (0-59)
    int second = timeinfo->tm_sec;       // tm_sec gives seconds (0-59)

    // Format the date and time as a string: "YYYY-MM-DD HH:MM:SS"
    String formattedDateTime = String(year) + "-";
    if (month < 10) {
        formattedDateTime += "0";
    }
    formattedDateTime += String(month) + "-";
    if (day < 10) {
        formattedDateTime += "0";
    }
    formattedDateTime += String(day) + " ";
    if (hour < 10) {
        formattedDateTime += "0";
    }
    formattedDateTime += String(hour) + ":";
    if (minute < 10) {
        formattedDateTime += "0";
    }
    formattedDateTime += String(minute) + ":";
    if (second < 10) {
        formattedDateTime += "0";
    }
    formattedDateTime += String(second);

    return formattedDateTime;
}


