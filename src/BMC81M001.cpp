/*************************************************
File:             BMC81M001.cpp
Author:           BESTMODULES
Description:      UART communication with the BMC81M001 
History：          
V1.0.1   -- initial version；2023-04-14；Arduino IDE :v1.8.13
**************************************************/

#include "BMC81M001.h"

/**********************************************************
Description: Constructor
Parameters:  *theSerial：hardware serial 
             BMduino optional:serial(default) serial1/seria2/seria3/seria4
             UNO optional:serial(default)
Return:      none     
Others:     
**********************************************************/
  BMC81M001::BMC81M001(HardwareSerial *theSerial)
  {
    _softSerial = NULL;
    _serial = theSerial;
  }

/**********************************************************
Description: Constructor
Parameters:  rxPin : Receiver pin of the UART
             txPin : Send signal pin of UART  
Return:      none    
Others:   
**********************************************************/
  BMC81M001::BMC81M001(uint16_t rxPin,uint16_t txPin)
{
  _serial = NULL;
  _rxPin = rxPin;
  _txPin = txPin;
  _softSerial = new SoftwareSerial(_rxPin, _txPin);
}
/**********************************************************
 Description: Module serial Initial
 Input:       baudRate : Set the Module  baudRate(Unique Value 9600)
 Output:         
 Return:          
 Others:   If the hardware UART is initialized, the _softSerial 
           pointer is null, otherwise it is non-null       
 **********************************************************/
 void BMC81M001::begin(uint32_t baud)
 {
    if(_serial!=NULL)
    {    _serial->begin(baud);}
    
    else
    {
          _softSerial->begin(baud);
    }

 }

/**********************************************************
Description: Module Initial 
Input:       ssid : wifi name
             password: wifi password
Output:         
Return:     Communication status  1:SEND_Success 0:SEND_FAIL  
                                     
Others:     
**********************************************************/
bool BMC81M001::connectToAP( String  ssid,  String pass)
{
  /* AT command to check moudule is ready  */
  if(sendATCommand("AT",1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  }      
  /* <AT+CWMODE=1> command to Set station mode   */
  if(sendATCommand("AT+CWMODE=1",1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  /* <AT+CWJAP="ssid","password"> add to AP  */
  String cmd="AT+CWJAP=\"";
  cmd+=ssid;
  cmd+="\",\"";
  cmd+=pass;
  cmd+="\"";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  return SEND_SUCCESS;
}
/**********************************************************
Description: Connect to TCP server
Input:       ip: TCP sever IP address
             port: TCP sever port number
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::connectTCP( String ip,  int port)
{  
  String  cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += ip;
  cmd += "\",";
  cmd += port;  
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS )
  {
       return SEND_SUCCESS; 
  }
  else 
  {
    return SEND_FAIL;
  } 
}

/**********************************************************
Description: Send data to TCP server
Input:       Dlength: data length
             *Dbuffer : Storing Data
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::writeDataTcp(int Dlength,char Dbuffer[])
{
  boolean found = SEND_SUCCESS;
  String  cmd = "AT+CIPSEND=";
  cmd += Dlength; 
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
   found =SEND_FAIL;
  } 
  delay(200);
  String S_Dbuffer = Dbuffer;
  if(sendATCommand(S_Dbuffer,1000,1) == SEND_SUCCESS ) ;
  else 
  {
    found =SEND_FAIL;
  } 
  return found;
}
/**********************************************************
Description: read data from module connect TCP server
Input:       
Output:         
Return:      
Others:        
**********************************************************/
String  BMC81M001::readDataTcp()
{
  int commaPosition;  
  String tcpBuf;
  clearResponse(BMC81M001Response);
  if(_serial!=NULL)
  {
  if(_serial->available())
  {
    delay(10);
    while(_serial->available())
    {
      BMC81M001Response[resLength++] = _serial->read();
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }}}
    else
    {
    if(_softSerial->available())
    {
      delay(10);
    while(_softSerial->available())
    {
      BMC81M001Response[resLength++] = _softSerial->read();
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }}}
    if(resLength>0)    
    {
      if(strstr(BMC81M001Response, "+IPD") != NULL)
      {
        String S_Dbuffer = BMC81M001Response;
        do
        { 
          commaPosition = S_Dbuffer.indexOf(':');
          if(commaPosition != -1)
          {
            S_Dbuffer = S_Dbuffer.substring(commaPosition+1, S_Dbuffer.length());
          }
          else
          {  
            if(S_Dbuffer.length() > 0)
             tcpBuf =(S_Dbuffer);                  
          }
         }
         while(commaPosition >=0); 
      }
    }
  return tcpBuf;
}
/**********************************************************
Description: Send data to TCP server
Input:       Dlength: data length
             *Dbuffer : Storing Data
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::connectAliyunIot(String clientlid,String username,String password,String mqtt_host,int server_port, String publishtopic,String ubscribetopic)
{
  //Enable SNTP server and set China time zone (utc+08:00)
  if(sendATCommand("AT+CIPSNTPCFG=1,8,\"ntp1.aliyun.com\"", 1000, 2) == SEND_SUCCESS );
  else 
  {
    return SEND_FAIL;
  }
/**********************************************************  
Description: Set mqtt user properties
command:     At+mqttusercfg=<linkid>, <scheme>, < "client\u ID" >, <username ">, <password" >, <cert_ Key_ Id>, <ca_ Id>, < "path" >
**********************************************************/
  String  cmd="AT+MQTTUSERCFG=0,1,\"NULL\",\"NULL\",\"NULL\",0,0,\"\"";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 

/**********************************************************
Description: Set mqtt client ID
command:     AT+MQTTCLIENTID=<LinkID>,<"client_id">
**********************************************************/
  cmd="AT+MQTTCLIENTID=0,\"";
  cmd+=clientlid;
  cmd+="\"";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
/**********************************************************
Description: Set mqtt username
command:     AT+MQTTUSERNAME=<LinkID>,<"username">
**********************************************************/
  cmd="AT+MQTTUSERNAME=0,\"";
  cmd+=username;
  cmd+="\"";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
/**********************************************************
Description: Set mqtt userpassword
command:     AT+MQTTPASSWORD=<LinkID>,<"password">
**********************************************************/
  cmd="AT+MQTTPASSWORD=0,\"";
  cmd+=password;
  cmd+="\"";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 

/**********************************************************
Description: Connect  MQTT Broker
command:     AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
**********************************************************/
  cmd="AT+MQTTCONN=0,\"";
  cmd+=mqtt_host;
  cmd+="\",";
  cmd+= server_port;
  cmd+= ",1";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
    
/**********************************************************
Description: aliyun iot subscription topic
command:     AT+MQTTCONN=<LinkID>,<"host">,<port>,<reconnect>
**********************************************************/
  cmd="AT+MQTTSUB=0,\"";
  cmd+=publishtopic;
  cmd+="\",1";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 

  cmd="AT+MQTTSUB=0,\"";
  cmd+=ubscribetopic;
  cmd+="\",1";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  
  return SEND_SUCCESS;
    
}

/**********************************************************
Description: Send data to aliyun IOT (data type:string)
Input:       Dlength: data length
             topic : MQTT topic Data
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::setTopic(String topic)
{
  String cmd;
  cmd="AT+MQTTSUB=0,\"";
  cmd+=topic;
  cmd+="\",1";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  return SEND_SUCCESS;
}
/**********************************************************
Description: Send data to aliyun IOT (data type:string)
Input:       Dlength: data length
             topic : MQTT topic Data
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::writeStringAliyun(String Dbuffer,String topic)
{
  String  cmd = "AT+MQTTPUB=0,\"";
  cmd+=topic;
  cmd+="\",\"";
  cmd+=Dbuffer;
  cmd+="\",0,0";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  return SEND_SUCCESS;
}

/**********************************************************
Description: Send data to aliyun IOT (data type:byte)
Input:       Dlength:   data length
             *Dbuffer : Storing Data
             topic:     
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::writeBytesAliyun(char Dbuffer[],int Dlength,String topic)
{
  String  cmd = "AT+MQTTPUBRAW=0,\"";
  cmd+=topic;
  cmd+="\",";
  cmd+=Dlength;
  cmd+=",0,0";
  if(sendATCommand(cmd,1000,3) == SEND_SUCCESS ) ;
  else 
  {
    return SEND_FAIL;
  } 
  for (uint8_t i = 0; i < Dlength; i++)
  {
    if (_softSerial != NULL)
  {
    _softSerial->print(Dbuffer[i]);
  }
  else
  {     _serial->print(Dbuffer[i]); 
  }
  }
  return SEND_SUCCESS;
}
/**********************************************************
Description: read data from module connect TCP server
Input:       
Output:         
Return:      
Others:        
**********************************************************/
void BMC81M001::readIotDataAliyun(String aliyunReciveBuff,int *aliyunReciveBufflen)
{
  String aliyunReciveTopic; 
  int commaPosition;
  char  data_div=0;
  String S_Dbuffer, S_Dbuffer1;
  clearResponse(BMC81M001Response);
  int ReciveBufflen=0;
  if(_serial!=NULL)
  {
  if(_serial->available())
  {
    delay(10);
    while(_serial->available())
    {
      BMC81M001Response[resLength++] = _serial->read();
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }}}
    else
    {
      if(_softSerial->available())
    {
      delay(10);
    while(_softSerial->available())
    {
      BMC81M001Response[resLength++] = _softSerial->read();
      if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
    }}
    }
    if(resLength>0)    
    {
      if(strstr(BMC81M001Response, "+MQTTSUBRECV") != NULL)
      {
        S_Dbuffer = BMC81M001Response;
        do
        { 
          commaPosition = S_Dbuffer.indexOf('"');
          if(commaPosition != -1)
          {
            data_div++;
            if(data_div== 2)
            {
              aliyunReciveTopic = S_Dbuffer1.substring(0, commaPosition);
              S_Dbuffer = S_Dbuffer.substring(commaPosition+1, S_Dbuffer.length());
              data_div=0;
            }
            else
            {
              S_Dbuffer = S_Dbuffer.substring(commaPosition+1, S_Dbuffer.length());
              S_Dbuffer1 =S_Dbuffer;
            }
          }       
         }while(commaPosition >=0); 
          S_Dbuffer = S_Dbuffer.substring(commaPosition+2, S_Dbuffer.length());
         for (uint8_t i=0;i<(S_Dbuffer.length()-2);i++)
         {
            if(S_Dbuffer[i]==0x2c)
            {
              break;
            }
            else
            {
              *aliyunReciveBufflen = ReciveBufflen *10  + (S_Dbuffer[i]-48);
            }
         }         
        commaPosition= 0;
        do
        { 
          commaPosition = S_Dbuffer.indexOf(',');
          if(commaPosition != -1)
          {
              aliyunReciveBuff = S_Dbuffer.substring(commaPosition+1,commaPosition+1+ *aliyunReciveBufflen);
              break;
          }       
         }while(commaPosition >=0);  
      }

    }
}
/**********************************************************
Description: send <AT+RST> command to softreset the module
Input:       
Output:         
Return:     Communication status  1:SEND_Success 0:SEND_FAIL  
                                     
Others:     
**********************************************************/
bool BMC81M001::reset(void)
{
  boolean found = SEND_SUCCESS;   
  if(sendATCommand("AT+RST",100,3) == SEND_SUCCESS )
  {
    clearResponse(BMC81M001Response);
    delay(1000) ;        // Issue soft-reset command
  }
  else 
  {
    found = SEND_FAIL;
  }
  return found;
}
/**********************************************************
Description: Send AT command  to moudle
Input:       StringstrCmd:AT command
             *response :receiving the response indicates success
             timeout：Resend message after timeout
             reTry :retransmission number
Output:         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
int BMC81M001::sendATCommand(String StringstrCmd, int timeout, uint8_t reTry)
{
  unsigned long t;
  char response[]="OK";
  String  cmd = "AT+MQTTPUB=0,\"";
  /* The message is sent repeatedly until it is received  */
  for(unsigned char tryTime = 0; tryTime < reTry; tryTime++)
  {
    clearResponse(BMC81M001Response);
    if (_softSerial != NULL)
  {
    _softSerial->println(StringstrCmd);
  }
  else
  { _serial->println(StringstrCmd); }
    t = millis();
  /* check the reply message within the timeout period. If the reply message contains preset characters,  return success  */
    for(uint8_t i=1;i>0;)
    {
      readResponse();
      if(strstr(BMC81M001Response,response) != NULL)
      {
        return SEND_SUCCESS;
      }
      if((t + timeout) < millis())
      {
        break;
      }      
    }
  }
  return SEND_FAIL;
}
/**********************************************************
Description: read data from module ,Send data to buffer
Input:      
Output:         
Return:        
Others:      1. read data to buffer
             2. clear data buffer if the length is overflow
**********************************************************/
void BMC81M001::readResponse(){
  if(_serial != NULL)
  {
  while(_serial->available())
  {
    BMC81M001Response[resLength++] = _serial->read();
    if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
  }
  }
  else
  {
  while(_softSerial->available())
  {
    BMC81M001Response[resLength++] = _softSerial->read();
    if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
  }
  }
}

/**********************************************************
Description: clear  data buffer
Input:       
Output:         
Return:      
Others:        
**********************************************************/
void BMC81M001::clearResponse(char Dbuffer[])
{
  memset(Dbuffer,'\0',RES_MAX_LENGTH);
  resLength = 0;
}
