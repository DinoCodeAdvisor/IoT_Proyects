#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "utils.h"
#include "secrets.h"
#include "ip_address_manager.h"

/**
 * Function declarations
 */
void handle_OnConnect();
void handle_NotFound();
String LoadHomePageHTML(IPAddress devicesIPs[6], String deviceStatus[6], String lastPingSuccessful[6], String lastPingAttempt[6]);

/**
 * Web server declarations
 */
WebServer server(80);
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

void setup()
{
    Serial.begin(921600);
    connectToWiFi();

    // Start NTP client
    timeClient.begin();
    timeClient.update();  // Synchronize time

    /**
     * Server routes
     */
    server.on("/", handle_OnConnect);
    server.onNotFound(handle_NotFound);

    /**
     * Start server
     */
    server.begin();
    Serial.println("[ESP32 - AcessPoint] HTTP Server started");
}

void loop()
{
    server.handleClient();
    

    WiFiClient client;

    pingDevices(client);
    retrieveData(client);
    uploadData(client);
}

/**
 * Custom functions
 */
void handle_OnConnect()
{
    Serial.println("[ESP32 - AccessPoint] A user logged into the homepage");
    server.send(200, "text/html", LoadHomePageHTML(devicesIPs, deviceStatus, lastPingSuccessful, lastPingAttempt));
}

void handle_NotFound(){
    server.send(404, "text/plain", "Not found");
}

/**
 * HTML Functions
 */
String LoadHomePageHTML(IPAddress devicesIPs[6], String deviceStatus[6], String lastPingSuccessful[6], String lastPingAttempt[6]) {
    String html = "<html><body><h1>Device Status</h1><ul>";

    for (int i = 0; i < 6; i++) {
        html += "<li>IP: " + devicesIPs[i].toString() + "<br>";
        html += "Status: " + deviceStatus[i] + "<br>";
        html += "Last Successful Vote: " + lastPingSuccessful[i] + "<br>";
        html += "Last Attempted Vote: " + lastPingAttempt[i] + "<br></li>";
    }

    html += "</ul></body></html>";
    return html;
}