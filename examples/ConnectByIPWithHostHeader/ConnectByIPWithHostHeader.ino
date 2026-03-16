/*
  Connect By IP, Override Host Header
  for ArduinoHttpClient library

  Some hosting providers serve multiple sites from the same IP.
  In that case you may need to connect to an IP address but still send the
  correct HTTP Host header (virtual host), e.g. "mypageservices.com".

  created 8 Jan 2026

  this example is in the public domain
*/

#include <ArduinoHttpClient.h>
#include <WiFi101.h>

#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Connect by IP address (string form)
char serverAddress[] = "93.184.216.34"; // example.com
int port = 80;

// But send this virtual-host name in the Host header
char hostName[] = "example.com";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // IMPORTANT: override Host header to target the right site on shared hosting
  client.setHostHeader(hostName);
}

void loop() {
  Serial.println("making GET request");
  client.get("/");

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}
