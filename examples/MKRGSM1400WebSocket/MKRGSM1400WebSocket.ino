/*
  MKRGSM1400 WebSocket client for ArduinoHttpClient library
  Connects to the Echo WebSocket server, and sends a hello
  message every 5 seconds.

  created 19 Dic 2020
  by Fernando Hidalgo

  This example is in the public domain
*/
#include <ArduinoHttpClient.h>
#include <MKRGSM.h>
#include "arduino_secrets.h"

char       PINNUMBER[]     = SECRET_PINNUMBER;
const char GPRS_APN[]      = SECRET_GPRS_APN;
const char GPRS_LOGIN[]    = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

char serverAddress[] = "echo.websocket.org";  // server address
int  port            = 80;  // To make a secure connection use 443

GSM             gsmAccess;  // pass true as param to enable debug mode: GSM gsmAccess(true);
GPRS            gprs;
GSMClient       gsmClient;  // To make a secure connection use GSMSSLClient
WebSocketClient client = WebSocketClient(gsmClient, serverAddress, port);
int             count  = 0;

void setup() {
  bool connected = false;
  
  Serial.begin(9600);
  
  while (!connected) {
    if ((gsmAccess.begin(PINNUMBER) == GSM_READY) &&
        (gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY)) {
      connected = true;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    Serial.println("Starting WebSocket client");
    client.begin();
  } else {
    startEchoMessages();
  }
}

void startEchoMessages() {
  sendMessage(); // send a message to receive an echo replay from the server
    
  while (client.connected()) {
    // check if a message is available to be received
    int messageSize = client.parseMessage();

    if (messageSize > 0) {
      Serial.print("Received message: ");
      Serial.println(client.readString());

      // increment count for next message
      count++;
      sendMessage();
    }

    delay(5000);
  }

  Serial.println("Disconnected...");
}

void sendMessage() {
   Serial.print("Sending: hello ");
   Serial.println(count);

   client.beginMessage(TYPE_TEXT);
   client.print("hello ");
   client.print(count);
   client.endMessage();
}
