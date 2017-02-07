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

const char webpage[] PROGMEM="<!DOCTYPE html>"
  "<html>"
    "<style> div.wm { position: relative; left: 36px; height: 1px; }</style>"
    "<head>"
      "<title>Bumper Bot Controls</title>"
      "<script>"
        //drive is the function that sends all controls 
        //  drive with 2 parameters sends motor speeds negative speeds with put a motor in reverse
        //  drive with 3 parameters sends commands and keeps current speed command is a number as third parameter
        "function drive(L,R,C=0){if(C==0){var req='?'+L+','+R+'!';}else{var req ='?C,C!'+C+'&'}var xhr=new XMLHttpRequest();xhr.open('GET',req,!0);xhr.send();}"

        //"function addKey(f){console.log(f.k.value+' '+f.a.value+'\n'+cons);}"

        "function changeVal(id,val){document.getElementById(id).innerHTML=val;}"
        "var lm=0;"
        "var rm=0;"
        "var cons={"//add more functions here, the keys should be the js keycode
          "' ':function() {drive(0,0)},"
          "stop:function() {drive(0,0)},"
          "w:function() {drive(75,75)},"
          "s:function() {drive(-40,-40)},"
          "d:function() {drive(-35,35)},"
          "a:function() {drive(35,-35)},"
          "f:function() {drive(0,0,1)},"
          "g:function() {drive(0,0,2)}"
        "};"
        "window.addEventListener('keydown', "
            "function (event) {"
              "if (event.defaultPrevented){return;}"
              "var k=event.key;"
              "if(k in cons){"
              "cons[k]();"
              "}else{"
                "console.log('nokey: '+k);"
              "}"
                "event.preventDefault();},"
                "true);"
                
      "</script>"
    "</head>"
    "<body>"
    "<div class='wm'><input id='bfw' type='button' value='w' onclick='cons.w();'/></div><br>"
    "<input id='bl' type='button' value='a' onclick='cons.d();'/>"
    "<input id='bbw' type='button' value='s' onclick='cons.s();'/>"
    "<input id='br' type='button' value='d' onclick='cons.a();'/><br>"
    
    "<input id='bstp' type='button' value='space' onclick='cons.stop();'/>"

    //"<form>Key code:<input type='text' name='k'><br>"
    //"Action:<input type='text' name='a'><br>"
    //"<input type='button' value='add' onclick=''></form>"
    "</body>"
  "</html>";

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

//
void doRequest(String r){
  String m=r.substring(r.indexOf("?")+1,r.indexOf("!"));
  if(m.substring(0,m.indexOf(","))!="C"){
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
 
  Serial.print(m);
}

