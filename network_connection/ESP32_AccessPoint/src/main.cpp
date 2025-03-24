/**
 * This code creates an access point (AP) that allows for other devices to connect to it.
 * 
 * To connect to the ESP32 you need to do the following:
 * 1. Connect to the ESP32 WiFi (set in the secrets.h - WIFI_SSID , password - WIFI_PASSWORD)
 * 2. Go to any web browser and put the given IP Address (given when running the server)
 * 3. Interact with the page to turn on and off the button.
 * 4. See the data in the ThingSpeak Channel
 * 
 * Notes:
 * - Set the ThingSpeak_API_KEY in the secrets.h file
 * - Update the logic for the uploadToThingSpeak if needed. (For now it only allows for one value at a time)
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "utils.h"
#include "secrets.h"

// put function declarations here:
void handle_OnConnect();
void handle_NotFound();
void handle_internalLedOn();
void handle_internalLedOff();
String SendHTML(uint8_t InternalLEDStatus);

/**
 * Web server declarations
 */
WebServer server(80);

/**
 * Testing with internal LED
 */
uint8_t InternalLEDPIN = 2;
bool InternalLEDStatus = LOW;

void setup() {
  Serial.begin(921600);

  pinMode(InternalLEDPIN, OUTPUT);

  connectToWiFi();

  delay(100);

  /**
   * Server routes
   */
  server.on("/", handle_OnConnect);
  server.on("/ledon", handle_internalLedOn);
  server.on("/ledoff", handle_internalLedOff);
  server.onNotFound(handle_NotFound);

  /**
   * Start server
   */
  server.begin();
  Serial.println("[AccessPoint] HTTP Server started - Max connections 5");
}

void loop() {
  server.handleClient();

  if(InternalLEDStatus)
  {
    digitalWrite(InternalLEDPIN, HIGH);
  }
  else
  {
    digitalWrite(InternalLEDPIN, LOW);
  }
}


/**
 * Custom functions
 */
void handle_OnConnect()
{
  InternalLEDStatus = LOW;
  Serial.println("Internal LED status: OFF");
  server.send(200, "text/html", SendHTML(InternalLEDStatus));
}

void handle_internalLedOn()
{
  InternalLEDStatus = HIGH;
  Serial.println("Internal LED status: ON");
  uploadToThingSpeak(1);
  server.send(200, "text/html", SendHTML(true));
}

void handle_internalLedOff()
{
  InternalLEDStatus = LOW;
  Serial.println("Internal LED status: OFF");
  uploadToThingSpeak(0);
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t InternalLEDStatus)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>DinoESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";

  if(InternalLEDStatus)
  {
    ptr +="<p>Internal LED Status: ON</p><a class=\"button button-off\" href=\"/ledoff\">OFF</a>\n";
  }
  else
  {
    ptr +="<p>Internal LED Status Status: OFF</p><a class=\"button button-on\" href=\"/ledon\">ON</a>\n";
  }

  return ptr;
}
