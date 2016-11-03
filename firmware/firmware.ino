// Original wireless AP setup code from:
//   https://www.hackster.io/rayburne/esp8266-access-point-using-arduino-ide-19f632

#include "WEMOS_Motor.h"
#include <Wire.h>
#include <ESP8266WiFi.h>

//#pragma GCC diagnostic ignored "-Wwrite-strings"

const char *ssid = "ACMBot-00";
const char *password = "uafrocks";
boolean    Lflag       = true;

WiFiServer server(80); // create object


//Motor shield I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30,_MOTOR_A, 1000);//Motor A
Motor M2(0x30,_MOTOR_B, 1000);//Motor B


void setup() {
  Serial.begin(250000);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // LED is active low, so set high to turn off.
  setupWiFi();
  server.begin();
}
int speed_l=0;
int speed_r=0;

void loop() 
{
    M1.setmotor(_CW, speed_l);
    M2.setmotor(_CW, speed_r);
    Serial.println(millis());
  WiFiClient client = server.available();  // Check if a client has connected
  if (!client)
  {
    if (Lflag)
    {
      Serial.println(F("Waiting for a client to connect to this server..."));
      Serial.println(F("On address 192.168.4.1"));
      delay(200);
      Lflag = false;
    }
      return;
  }

  String req = client.readStringUntil('\r');// Read the first line of the request
  Serial.println(req);
  client.flush();

  // Match the request
  int val = -1;                            // 'val' to keep track of both the
                                           // request type (read/set) and value if set.
//  if (req.indexOf("/led/0") != -1)
//    val = 0; // Will write LED low
//  else if (req.indexOf("/led/1") != -1)
//    val = 1; // Will write LED high
//  else if (req.indexOf("/read") != -1)
//    val = -2;                              // Will print pin reads
//                                           // Otherwise request is invalidt
//  if (val >= 0)                            // Set GPIO5 according to input
//    digitalWrite(LED_BUILTIN, val);
  if(req.indexOf("/motors/forward") != -1) {
    speed_l=75;
    speed_r=75;
    delay(10);
    Serial.println("Spinning motors forward");
  }
  else if(req.indexOf("/motors/stop") != -1) {
    speed_l=0;
    speed_r=0;
    delay(10);
    Serial.println("Stopping motors");
  }
  

  client.flush();

  static const String ui = "<!DOCTYPE html>\
    <html>\
      <head>\
        <title>ACM Desktop Bot Control</title>\
      </head>\
        <form action='/motors/forward' method='get'>\
          <button type='submit' class='button'>Motors Forward</button>\
        </form>\
        <form action='/motors/stop' method='get'>\
          <button type='submit' class='button'>Motors Stop</button>\
        </form>\
      </body>\
    </html>";

//    <form action='/motors/forward' method='get'>\
//          <button type='submit' class='button'>Motors off</button>\
//        </form>\
//        <form action='/motors/stop' method='get'>\
//          <button type='submit' class='button'>Motors Off</button>\
//        </form>\   
//
//      <style>\
//        .button {\
//          background-color: #4CAF50;\
//          border: none;\
//          color: white;\
//          padding: 15px 32px;\
//          text-align: center;\
//          text-decoration: none;\
//          display: inline-block;\
//          font-size: 16px;\
//        }\
//      </style>\

  client.print(ui);                         // Send the response to the client
  delay(1);
  Serial.println(F("Client disonnected"));

  // The client will actually be disconnected 
  // when the function returns and 'client' object is destroyed
}

void setupWiFi()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
}


