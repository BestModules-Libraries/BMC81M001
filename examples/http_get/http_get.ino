/*****************************************************************
File:         http_get.ino
Description: 1. Hardware serial port 1 is used for communication with BMC81M001, with a communication rate of 115200bps
             2. The serial port is used as a monitoring serial port to print and output some status of BMC81M001
             3. Automatically complete BMC81M001 networking action when powered on
             4. When get http data command, its will return http output data.
******************************************************************/
#include "BMC81M001.h"

#define  HTTPGET_PORT_HTTP 80
#define  HTTPGET_PORT_HTTPS 443

#define PORT 8888
#define URL "http://iot.arduino.org.tw"
#define SUBURL "/bigdata/dhtdata/dhDatatadd.php?MAC=4417933E9BEC&T=21.5&H=83.4"

// #define PORT HTTPGET_PORT_HTTP
// #define URL "http://worldclockapi.com"
// #define SUBURL "/api/json/est/now"

BMC81M001 Wifi(&Serial1);//Please uncomment out this line of code if you use HW Serial1 on BMduino

void setup() 
{
  Serial.begin(115200);           
  Wifi.begin();

  Wifi.connectToAP("zengdebin","123456789");//connect to AP 
  
  Wifi.http_begin(URL,PORT,SUBURL);//begin http get 


}

void loop() {
  Wifi.http_get();//http get opration

  Serial.println(Wifi.http_getString());//get http result

  Wifi.http_end(); //end of http get

  delay(10000);
}

