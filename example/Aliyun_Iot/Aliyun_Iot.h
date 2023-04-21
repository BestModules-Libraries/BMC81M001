#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "Aliyun_Iot.h"
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

#define WIFI_SSID "iQOO_Neo_855"
#define WIFI_PASS "12345678.!"

#define CLIENTLID  "mytest|securemode=3\\,signmethod=hmacsha1\\,timestamp=6789|"        
#define USERNAME  "Arduino_Wifi&gqzn81RWZC2"                                           
#define PASSWORD  "9CF1D3420F07ECC02250EF829D9EAC8529A8D672"                              
#define MQTT_HOST "gqzn81RWZC2.iot-as-mqtt.cn-shanghai.aliyuncs.com"  
#define SERVER_PORT 1883

#define PUBLISHTOPIC "gqzn81RWZC2/Arduino_Wifi/user/update"                    
#define SUBSCRIBERTOPIC "gqzn81RWZC2/Arduino_Wifi/user/get"
#define CUSTOMTOPIC "gqzn81RWZC2/Arduino_Wifi/user/ardunio"  

//*********************************************//
//************* IO_Port Define ***************//
//*********************************************//
int LED = 13;                         // LED port
//*********************************************//
//************* Variable Define ***************//
//*********************************************//
#define DEB_CNT     80                //50ms
#define RES_MAX_LENGTH 200            //max buffer length

char  SerialBuff[RES_MAX_LENGTH];   //serial buffer
char  data[30];                       //key data buffer
int   resLen;                         //serial buffer use length
String aliyunReciveBuff;
int aliyunReciveBufflen;

String DATA_BUF ;                     //
String topic ;                        //MQTT_Topic

#endif
