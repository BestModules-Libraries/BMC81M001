/*************************************************
File:             BMC81M001.cpp
Author:           BEST MODULES CORP.
Description:      UART communication with the BMC81M001 
version:          V1.0.4-2024-8-22
**************************************************/
#include "BMC81M001.h"

/**********************************************************
Description: Constructor
Parameters:  *theSerialÔºöhardware serial 
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
 Parameters:  baudRate : Set the Module  baudRate       
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
Description: connect Ap
Parameters:  ssid : wifi name
             password: wifi password     
Return:     Communication status  1:SEND_Success 0:SEND_FAIL  
                                     
Others:     
**********************************************************/
bool BMC81M001::connectToAP( String  ssid,  String pass)
{  
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
Description: Connect to TCP server
Parameters:  ip: TCP sever IP address
             port: TCP sever port number      
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
Parameters:  Dlength: data length
             *Dbuffer : Storing Data        
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
  if(sendATCommand(S_Dbuffer,1000,1) == SEND_SUCCESS )  
  {  
    found =SEND_SUCCESS;
  }
  else 
  {
    found =SEND_FAIL;
  } 
  return found;
}
/**********************************************************
Description: read data from module connect TCP server
Parameters:           
Return:   String   
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
Description: Configure MQTT parameters
Parameters:  clientlid?Åusername?Åpassword?Åmqtt_host?Åserver_port      
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::configMqtt(String clientlid,String username,String password,String mqtt_host,int server_port)
{
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
  cmd+= ",0";
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
Description: set PublishTopic
Parameters:       
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::setPublishTopic(String publishtopic)
{
  String cmd;
  cmd="AT+MQTTSUB=0,\"";
  cmd+=publishtopic;
  cmd+="\",0";
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
Description: set Subscribetopic
Parameters:       
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::setSubscribetopic(String subscribetopic)
{
  String cmd;
  cmd="AT+MQTTSUB=0,\"";
  cmd+=subscribetopic;
  cmd+="\",0";
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
Description: set custom topic
Parameters:  Dlength: data length
             topic : MQTT topic Data    
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::setTopic(String topic)
{
  String cmd;
  cmd="AT+MQTTSUB=0,\"";
  cmd+=topic;
  cmd+="\",0";
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
Description: Send data to IOT (data type:string)
Parameters:  Dlength: data length
             topic : MQTT topic Data      
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::writeString(String Dbuffer,String topic)
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
Description: Send data to IOT (data type:byte)
Parameters:  Dlength:   data length
             *Dbuffer : Storing Data
             topic:   PUBLISHTOPIC         
Return:      Communication status  1:SEND_Success 0:SEND_FAIL  
Others:        
**********************************************************/
bool BMC81M001::writeBytes(char Dbuffer[],int Dlength,String topic)
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
Parameters:  IotReciveBuffÔºöStoring String data  
             IotReciveBufflenÔº?   String length 
             topicÔºödata form topic
Return:      
Others:        
**********************************************************/
void BMC81M001::readIotData(String *IotReciveBuff,int *IotReciveBufflen,String *topic)
{
  String ReciveTopic; 
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
    }}
          }
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
        OneNetReciveBuff=S_Dbuffer;  
        do
        { 
          commaPosition = S_Dbuffer.indexOf('"');
          if(commaPosition != -1)
          {
            data_div++;
            if(data_div== 2)
            {
              ReciveTopic = S_Dbuffer1.substring(0, commaPosition);
              S_Dbuffer = S_Dbuffer.substring(commaPosition+1, S_Dbuffer.length());
              data_div=0;
            }
            else
            {
              S_Dbuffer = S_Dbuffer.substring(commaPosition+1, S_Dbuffer.length());
              S_Dbuffer1 =S_Dbuffer;
            }
          }    
         }
         while(commaPosition >=0); 
          S_Dbuffer = S_Dbuffer.substring(commaPosition+2, S_Dbuffer.length());
         for (uint8_t i=0;i<(S_Dbuffer.length()-2);i++)
         {
            if(S_Dbuffer[i]==0x2c)
            {
              break;
            }
            else
            {
              *IotReciveBufflen = ReciveBufflen *10  + (S_Dbuffer[i]-48);
            }
         }         
        commaPosition= 0;
        do
        { 
          commaPosition = S_Dbuffer.indexOf(',');
          if(commaPosition != -1)
          {
              *IotReciveBuff = S_Dbuffer.substring(commaPosition+1,commaPosition+1+ *IotReciveBufflen);
              break;
          }       
         }while(commaPosition >=0);  
      }
    }
    *topic=ReciveTopic;
}
/**********************************************************
Description: send <AT+RST> command to softreset the module
Parameters:           
Return:     Communication status  1:SEND_Success 0:SEND_FAIL  
                                     
Others:     
**********************************************************/
bool BMC81M001::reset(void)
{
  boolean found = SEND_SUCCESS;   
  if(sendATCommand("AT+RST",1000,3) == SEND_SUCCESS )
  {
    clearResponse(BMC81M001Response);
    delay(2000) ;        // Issue soft-reset command
  }
  else 
  {
    found = SEND_FAIL;
  }
  return found;
}
/**********************************************************
Description: Send AT command  to moudle
Parameters:  StringstrCmd:AT command
             *response :receiving the response indicates success
             timeoutÔºöResend message after timeout
             reTry :retransmission number      
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
    readResponse();
    clearResponse(BMC81M001Response);
    if (_softSerial != NULL)
  {
    _softSerial->println(StringstrCmd);
  }
  else
  { _serial->println(StringstrCmd); }
    t = millis();
  /* check the reply message within the timeout period. If the reply message contains preset characters,  return success  */
    for(uint8_t i=0;;i++)
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
Parameters:         
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
Parameters:        
Return:      
Others:        
**********************************************************/
void BMC81M001::clearResponse(char Dbuffer[])
{
  memset(Dbuffer,'\0',RES_MAX_LENGTH);
  resLength = 0;
}
