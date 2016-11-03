//Needed for the wifi module
#include <ESP8266WiFi.h>

//Needed for the motor shield
#include "WEMOS_Motor.h"

//SSID and password
const char ssid[]="ACMBot-09";
const char password[]="uafrocks";

//Webpage we send to clients (PROGMEM causes a segfault of sorts...)
const char webpage[]="<!DOCTYPE html>"
  "<html>"
    "<head>"
      "<title>ACM Desktop Bot Control</title>"
    "</head>"
      "<form action='/motors/forward' method='get'>"
        "<button type='submit' class='button'>Motors Forward</button>"
      "</form>"
      "<form action='/motors/stop' method='get'>"
        "<button type='submit' class='button'>Motors Stop</button>"
      "</form>"
    "</body>"
  "</html>";

//Server is on port 80 (web)
WiFiServer server(80);

//Motor variables
//	I2C Address: 0x30
//	PWM frequency: 1000 Hz
Motor M1(0x30,_MOTOR_A,1000);
Motor M2(0x30,_MOTOR_B,1000);
int speed_l=0;
int speed_r=0;

void setup()
{
  //Start serial port, print out our address
  Serial.begin(250000);
  delay(10);
  Serial.println(F("Waiting for a client to connect to this server..."));
  Serial.println(F("On address 192.168.4.1"));

  //Turn LED on
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  //Start web server
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  server.begin();
}

void loop()
{
  //Set motors to their speed (DO THIS EVERYTIME IN THE LOOP)
  M1.setmotor(_CW,speed_l);
  M2.setmotor(_CW,speed_r);

  //Check for client
  WiFiClient client=server.available();

  //No client, return early
  if(!client)
    return;

  //Got a client, get their request, and print to serial
  String request=client.readStringUntil('\r');
  Serial.println(request);

  //Check for forward command
  if(request.indexOf(F("/motors/forward"))!=-1)
  {
    speed_l=75;
    speed_r=75;
    Serial.println(F("Forward!"));
  }

  //Check for stop command
  else if(request.indexOf(F("/motors/stop"))!=-1)
  {
    speed_l=0;
    speed_r=0;
    Serial.println(F("Stop!"));
  }

  //Send client the webpage
  client.print(webpage);
  client.flush();
}
