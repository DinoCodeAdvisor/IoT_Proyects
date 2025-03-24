#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"

void connectToWiFi()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Connecting to Wi-Fi...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        int attempts = 0;

        while(WiFi.status() != WL_CONNECTED && attempts < 20)
        {
            delay(1000);
            Serial.print(".");
            attempts++;
        }

        if(WiFi.status() == WL_CONNECTED)
        {
            Serial.println("Connected to WiFi");
            Serial.print("IP Address: ");
            Serial.println(WiFi.localIP());
        }
        else
        {
            Serial.println("Failed to connect. Retrying...");
            connectToWiFi();
        }
    }
    else
    {
        Serial.println("Already connected to Wi-Fi");
        Serial.print("IP Address:");
        Serial.println(WiFi.localIP());
    }
}

void uploadToThingSpeak(int value)
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wi-Fi lost! Reconnecting...");
        connectToWiFi();
    }

    HTTPClient http;
    String url = "https://api.thingspeak.com/update?api_key=" + String(ThingSpeak_API_KEY) + "&field1=" + String(value);

    Serial.println("Sending data to ThingSpeak: " + url);

    http.begin(url);
    int httpCode = http.GET();

    if(httpCode > 0)
    {
        String response = http.getString();
        Serial.println("Data sent successfully. Response: " + response);
    }
    else
    {
        Serial.println("Error sending data. HTTP Code: " + String(httpCode));
        delay(10000);
        uploadToThingSpeak(value);
    }

    http.end();
}