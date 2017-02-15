//for wifi
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Servo.h>
//for motors
#include "WEMOS_Motor.h"

//structure to add interactions
#include "bumperbot_stuff.h"

//SSID and password
const char ssid[]="BB-01";
const char password[]="bumperbot";

#include "webpage.h"

//Server on port 80
WiFiServer server(80);

//Motor  variables
//    I2C Address: 0x30
//    PWM frequency: 1000 Hz
Motor M1(0x30,_MOTOR_A,1000);
Motor M2(0x30,_MOTOR_B,1000);
int speed_l=0;
int speed_r=0;
int turn_l=_CW;
int turn_r=_CW;

void setup() {
  // Start serial port, prints address
  Serial.begin(57600);
  delay(10);
  Serial.println(("Waiting for a clint to connect to this server..."));
  Serial.println(("On address 192.168.4.1"));

  //built in LED
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,LOW);

  pinMode(D3,OUTPUT);
  digitalWrite(D3,LOW);

  //start the web server
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid,password);
  server.begin();

  digitalWrite(D3,HIGH);
}
//make ToggleP pin controllers for the builtin LED and pin D3
ToggleP light(LED_BUILTIN);
ToggleP light2(D3);

void loop() {
  //set motors to their speed
  M1.setmotor(turn_l,speed_l);
  M2.setmotor(turn_r,speed_r);

  //Check for client
  WiFiClient client=server.available();

  //no client, return early
  if(!client)
    return;

  //Got a client, get their request, and print to serial
  String request=client.readStringUntil('\r');
  Serial.println(request);
  doRequest(request);

  //Send client the webpage
  String msg=webpage;
  client.print(String(webpage));
  client.flush();
}

//processes the request sent by from the wweb page
void doRequest(String r){
  //pulls the motor controll info frim the request string
  String m=r.substring(r.indexOf("?")+1,r.indexOf("!"));

  //checks if the motor controll should be ignored
  if(m.substring(0,m.indexOf(","))!="C"){
    //sets motors to the speed in the request
    speed_l=m.substring(0,m.indexOf(",")).toInt();
    if(speed_l<0)
    {
      turn_l=_CCW;
      speed_l*=-1;
    }else if(turn_l!=_CW){turn_l=_CW;}

    speed_r=m.substring(m.indexOf(",")+1).toInt();
     if(speed_r<0)
    {
      turn_r=_CCW;
      speed_r*=-1;
    }else if(turn_r!=_CW){turn_r=_CW;}
  }else{
    //gets the command info and runs its function
    String c=r.substring(r.indexOf("!")+1,r.indexOf("&"));
    switch(c.toInt()){//add command functions here
      case(1):
        light.tog();
        break;
      case(2):
        light2.tog();
        break;
      default:
        break;
    }
  }
}

