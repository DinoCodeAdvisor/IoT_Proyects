import network
import socket
import time
import json

# WiFi settings
SSID = 'HERE GOES YOUR SSID'  # Replace with your WiFi SSID
PASSWORD = 'HERE GOES YOUR PASSWORD'  # Replace with your WiFi password

# Credential
WATCHDOG_KEY = "HERE GOES YOUR WATCHDOG_KEY" # Use the same as you placed in your secrets.h in your ESP32

# Function to connect to WiFi
def connect_to_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(SSID, PASSWORD)

    while not wlan.isconnected():
        time.sleep(1)
    
    wlan.ifconfig(('192.168.3.100', '255.255.255.0', '192.168.3.100', '8.8.8.8'))
    
    print("Connected to WiFi, IP:", '192.168.3.100')

# Function to serve the HTML page with the current LED status
def send_html(client):
    html = """<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>IoT Star Network</title>
    <style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}
    body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}
    .button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}
    .button-on {background-color: #3498db;}
    .button-on:active {background-color: #2980b9;}
    .button-off {background-color: #34495e;}
    .button-off:active {background-color: #2c3e50;}
    p {font-size: 14px;color: #888;margin-bottom: 10px;}
    </style>
</head>
<body>
<h1>DinoRPi Pico W Web Server (Node 1)</h1>
<h3>Using Access Point(AP) Mode</h3>"""
    
    html += "</body></html>"
    client.send(html)

# Main web server loop
def start_server():

    # Create a socket
    addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind(addr)
    s.listen(1)

    print('Listening on', addr)

    while True:
        cl, addr = s.accept()
        print('Client connected from', addr)
        request = cl.recv(1024)
        request = str(request)

        
        if '/' in request and not '/poll' in request and not '/data' in request:
            send_html(cl)
        elif '/poll/makevote' in request:
            print("Make poll called");
            response = json.dumps({f"{WATCHDOG_KEY}": "Vote submited"})
            cl.send("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + response)
        elif f'/data/{WATCHDOG_KEY}' in request:
            print("Retrieving and sending data");
            response = json.dumps({"data": 10, f"{WATCHDOG_KEY}": "authenticated"})
            cl.send("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n" + response)
            
            
            
            
        cl.close()

# Connect to WiFi
connect_to_wifi()

# Start the server
start_server()
