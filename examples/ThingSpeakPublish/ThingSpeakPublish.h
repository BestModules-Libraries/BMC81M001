#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "ThingSpeakPublish.h"
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

#define WIFI_SSID "Hun"
#define WIFI_PASS "kjxxcr520"

#define CLIENTLID  "Jw8YCBY4Bx8NDiUmACYIGy8"  
#define USERNAME  "Jw8YCBY4Bx8NDiUmACYIGy8"                            
#define PASSWORD  "UmiDfeoRbVkceoY4RHDxVcBj"                   
#define MQTT_HOST "mqtt3.thingspeak.com"             
#define SERVER_PORT 1883

#define PUBLISHTOPIC "channels/2181120/publish"                 
#define SUBSCRIBERTOPIC1 "channels/2181120/subscribe" 
#define SUBSCRIBERTOPIC2 "channels/2181120/subscribe/fields/field1" 
#define CUSTOMTOPIC ""  //Custom Topic

//*********************************************//
//************* IO_Port Define ***************//
//*********************************************//
int LED = 13;                         // LED port
//*********************************************//
//************* Variable Define ***************//
//*********************************************//
#define DEB_CNT     80                //50ms
#define RES_MAX_LENGTH 200            //max buffer length

char  OledBuff[RES_MAX_LENGTH];   //serial buffer
char  data[30];                       //key data buffer
int   resLen;                         //serial buffer use length
String ReciveBuff;
int ReciveBufflen;

String DATA_BUF ;                     //
String topic ;                        //MQTT_Topic

#endif
