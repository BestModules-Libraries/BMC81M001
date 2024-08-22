#ifndef _BMC81M001_H__
#define _BMC81M001_H__

//*********************************************//
//****************** Header *******************//
//*********************************************//
#include "ONENET.h"
#include "BMC81M001.h"

//*********************************************//
//*********** wifi information ****************//
//*********************************************//

#define WIFI_SSID "Hun"
#define WIFI_PASS "kjxxcr520"

#define CLIENTLID "wifi" 
#define USERNAME "cekQZv1EU5"
#define PASSWORD "version=2018-10-31&res=products%2FcekQZv1EU5%2Fdevices%2Fwifi&et=1748654171&method=md5&sign=3Ywcz76x73Hfuhe0%2FrE8hA%3D%3D"
#define MQTT_HOST "mqtts.heclouds.com"             
#define SERVER_PORT 1883
#define ONENET_TOPIC_PROP_POST "$sys/cekQZv1EU5/wifi/thing/property/post"
//设备属性上报请求,设备---->OneNET
#define ONENET_TOPIC_PROP_SET "$sys/cekQZv1EU5/wifi/thing/property/set"
//设备属性设置请求,OneNET---->设备
#define ONENET_TOPIC_PROP_POST_REPLY "$sys/cekQZv1EU5/wifi/thing/property/post/reply"
//设备属性上报响应,OneNET---->设备
#define ONENET_TOPIC_PROP_SET_REPLY "$sys/cekQZv1EU5/wifi/thing/property/set_reply"
//设备属性设置响应,设备---->OneNET
#define ONENET_TOPIC_PROP_GET "$sys/cekQZv1EU5/wifi/thing/property/get"
#define ONENET_TOPIC_PROP_GET_REPLY "$sys/cekQZv1EU5/wifi/thing/property/get_reply"
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
int ID=123;
String SET_BUF ;                     //
String GET_BUF ;  
String topic ;                        //MQTT_Topic

#endif
