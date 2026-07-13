#include <Arduino.h>
#include "BluetoothSerial.h"
#include <WiFi.h>
#include <WebServer.h>

BluetoothSerial SerialBT;
WebServer server(80);

#define RELAY 23

const char* AP_SSID = "ESP32_Relay_AP";
const char* AP_PASSWORD = "12345678";

void relayOn() {
  digitalWrite(RELAY, HIGH);
  server.send(200, "text/plain", "Relay ON");
  Serial.println("Relay ON (WiFi)");
}

void relayOff() {
  digitalWrite(RELAY, LOW);
  server.send(200, "text/plain", "Relay OFF");
  Serial.println("Relay OFF (WiFi)");
}

void handleRoot() {
  String html = "<html><body>"
                "<h2>ESP32 Relay Control</h2>"
                "<a href=\"/on\"><button style='font-size:20px;'>Turn ON</button></a><br><br>"
                "<a href=\"/off\"><button style='font-size:20px;'>Turn OFF</button></a>"
                "</body></html>";
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);

  SerialBT.begin("ESP32_Relay");
  Serial.println("Bluetooth Ready");

  WiFi.softAP(AP_SSID, AP_PASSWORD);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("WiFi AP started. Connect to network: ");
  Serial.println(AP_SSID);
  Serial.print("Then open this IP in your browser: ");
  Serial.println(IP);

  server.on("/", handleRoot);
  server.on("/on", relayOn);
  server.on("/off", relayOff);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    if (cmd == '1') {
      digitalWrite(RELAY, HIGH);
      Serial.println("Relay ON (Bluetooth)");
    }
    if (cmd == '0') {
      digitalWrite(RELAY, LOW);
      Serial.println("Relay OFF (Bluetooth)");
    }
  }
}
