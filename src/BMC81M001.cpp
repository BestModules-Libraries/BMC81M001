/*************************************************
File:             BMC81M001.cpp
Author:           BEST MODULES CORP.
Description:      UART communication with the BMC81M001 
version:          V1.0.5  -2025-07-21
**************************************************/

#include "BMC81M001.h"
#include "variant.h"

/**********************************************************
Description: Constructor
Parameters:  *theSerial hardware serial 
             BMduino optional:serial(default) serial1/seria2/seria3/seria4
             UNO optional:serial(default)
Return:      none     
Others:      none
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
Others:      none
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
 Parameters:  baud : Set the Module  baudRate       
 Return:      void    
 Others:   If the hardware UART is initialized, the _softSerial pointer is null, otherwise it is non-null       
 **********************************************************/
 void BMC81M001::begin(uint32_t baud)
 {
    if(_serial!=NULL)
    {   
       _serial->begin(baud);
    }
    
    else
    {
       _softSerial->begin(baud);
    }
 }

/**********************************************************
Description: connect Ap
Parameters:  ssid : wifi name
             password: wifi password     
Return:      Communication status  
               true: SEND_Success 
               false: SEND_FAIL                           
Others:      none
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
Return:      Communication status  
               true: SEND_Success 
               false: SEND_FAIL   
Others:      none  
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
             Dbuffer[] : Storing Data        
Return:      Communication status  
               true: SEND_Success 
               false: SEND_FAIL   
Others:      none  
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
Parameters:  void         
Return:      Data sent by the TCP server   
Others:      none  
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
Parameters:  
             clientlid: Client, user ID
             username: User name
             password: Password
             mqtt_host: Server address
             server_port: Server port      
Return:      Communication status  
               true: SEND_Success 
               false: SEND_FAIL 
Others:     none  
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
  if(sendATCommand(cmd,5000,3) == SEND_SUCCESS )
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
Parameters:  subscribetopic: The default subscribed Topic in the Topic list on the IoT platform    
Return:      Communication status  
               true: SEND_Success 
               false: SEND_FAIL 
Others:      none        
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
Description: Send data to IOT (data type:string)
Parameters:  Dbuffer: The character data to be sent
             topic: MQTT topic Data      
Return:      communication status  
               true: SEND_Success 
               false: SEND_FAIL 
Others:      none        
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
Parameters:  
             Dbuffer[]: Storing Data
             Dlength:   data length
             topic:   PUBLISHTOPIC         
Return:      communication status  
               true: SEND_Success 
               false: SEND_FAIL 
Others:      none      
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
Parameters:  
             *ReciveBuf: The received data  
             *ReciveBufflen: Received data length
             *topic: The received data belongs to the Topic
Return:      void
Others:      none  
**********************************************************/
void BMC81M001::readIotData(String *IotReciveBuff,int *IotReciveBufflen,String *topic)
{
  clearResponse(BMC81M001Response);
  *IotReciveBufflen=0;

    if(_serial!=NULL)
    {
        if(_serial->available())
        {
            delay(10);
            while(_serial->available())
            {
              uint8_t temp;
              temp=_serial->read();
              //Serial.write(temp);
              BMC81M001Response[resLength++] = temp;
              if(resLength == RES_MAX_LENGTH) clearResponse(BMC81M001Response);
            }
        }
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
        }
      }        
    }
    if(resLength>0)    
    {
      char *newstr;
      if(strstr(BMC81M001Response, "+MQTTSUBRECV") != NULL)
      {
        newstr=strtok(BMC81M001Response,",");
        if(newstr==NULL) return;
        newstr=strtok(NULL,",");
        *topic=String(newstr);
        newstr=strtok(NULL,",");
        *IotReciveBufflen=atoi(newstr);
        newstr=strtok(NULL,"\r\n");
        *IotReciveBuff=String(newstr);

        // Serial.print("topic:");
        // Serial.println(*topic);
        // Serial.print("IotReciveBufflen:");
        // Serial.println(*IotReciveBufflen);
        // Serial.print("IotReciveBuff:");
        // Serial.println(*IotReciveBuff);
      }
    }
}
/*void BMC81M001::readIotData(String *IotReciveBuff,int *IotReciveBufflen,String *topic)
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
*/
/**********************************************************
Description: Start entering HTTP get
Parameters:    
            serverURL:Website Domain Name
            port:Website port
            subURL:Path, default value '/'
Return:     Implementation status: 
              0: Parameter format OK  
              -1: Parameter format error                        
Others:     none   
**********************************************************/
int  BMC81M001::http_begin(String serverURL,int port,String subURL)
{
  serverURL.toCharArray(BMC81M001Response, RES_MAX_LENGTH);
  char *token =strtok(BMC81M001Response,"//");
  if(token==NULL) return HTTP_GET_URL_ERROR;

  if(strstr(token,"http:")!=NULL)
  {
    _port=80;
 
    _type="TCP";
  }
  else if(strstr(token,"https:")!=NULL)
  {
    _port=443;
 
    _type="SSL";
  }    
  else return HTTP_GET_URL_ERROR;
  token =strtok(NULL,"//");
  _url=String(token);

  if(subURL=="")
    _suburl="/";
  else  _suburl=subURL;
  _len= subURL.length()+4+9+2;//GET / HTTP/1.1
  _host= "Host: ";
  _host+=_url;
  if(port!=80&&port!=443)
  {
      _port= port;
      _host+=":";
      _host+= _port;
      _host+="\r\n\r\n";
  }
  else
      _host+="\r\n\r\n";
   _len+=_host.length();  
  return HTTP_GET_BEGIN_SUCCESS;
}


   bool is_blank_line(const char *line) 
   {
    while (*line != '\0') 
    {
      if (*line != '\r' && *line != '\n') 
      {
        return false;
      }
      line++;
    }
    return true;
}

/**********************************************************
Description: Http_get operation
Parameters:  void
Return:      Implementation status: 
               0: get successful  
              -1: The time limit was exceeded and no data was obtained             
Others:       The time limit was exceeded and no data was obtained: It might be caused by reasons such as incorrect website address, 
                                                                    incorrect port, incorrect path, or the server not responding  
**********************************************************/
int BMC81M001::http_get(void)
 {
  String cmd;
  int result=HTTP_GET_OP_SUCCESS;

  cmd="AT+CIPSTART=\"";
  cmd+=_type;
  cmd+="\",\"";
  cmd+=_url;
  cmd+="\",";
  cmd+=_port;
  // AT+CIPSTART="TCP","iot.arduino.org.tw",8888
  if(sendATCommand(cmd, 10000, 3)==SEND_SUCCESS)
  {
      // sendATCommand("AT+CIPMODE=1", 1000, 3);//transparent transmission
        cmd="AT+CIPSEND=";
        cmd+=_len;
        sendATCommand(cmd, 1000, 3);
        delay(100);//wait for ">"
        readResponse();
        cmd="GET ";
        cmd+=_suburl;
        cmd+=" HTTP/1.1\r\n";
        if (_softSerial != NULL)
        {
          _softSerial->print(cmd);
        }
        else
        { _serial->print(cmd); }
        delay(500);       

        if (_softSerial != NULL)
        {
          _softSerial->print(_host);
        }
        else
        { _serial->print(_host); }
        int recive_index=0;
        int delay_count;
        resLength=0;
        delay_count=0;
        int blank_line_count=0;
        while(1)
        {
          if(_serial->available())
          {
            uint8_t temp;
            delay_count=0;
            temp = _serial->read();
            if(blank_line_count<3)
            {
              BMC81M001Response[resLength++] = temp;
              if(temp==0x0A)//读入一行
              {
                if(is_blank_line(BMC81M001Response)==true)
                {
                  blank_line_count++;             
                }
                clearResponse(BMC81M001Response);
                resLength=0;
              }          
            }
            else
            {
              if(resLength < RES_MAX_LENGTH) 
                BMC81M001Response[resLength++] = temp;
            }
          }
          else
          {
            delay(1);
            delay_count++;
            if(delay_count>3000)
            {
              if(blank_line_count==3)
              {
                result=HTTP_GET_OP_SUCCESS ;         
              }
              else
              {
                result=HTTP_GET_OP_TIMEOUT ;
              }
              break;
            }
          }
        }
    }
    else
    {
     result=COMMUNICAT_ERROR;
    }
    return HTTP_GET_OP_SUCCESS;
 }


/**********************************************************
Description: read data after http get
Parameters:  void       
Return:      The data returned from the web page  
Others:      none
**********************************************************/
String BMC81M001::http_getString(void)
{
  if(resLength<RES_MAX_LENGTH)BMC81M001Response[resLength]='\0';
  return String(BMC81M001Response);

}

/**********************************************************
Description: end http opration  
Parameters:  void       
Return:      void   
Others:      To perform the next http get, end must be called first
**********************************************************/
void BMC81M001::http_end(void)
{
  sendATCommand("AT+CIPCLOSE", 1000, 3);
}

/**********************************************************
Description: send <AT+RST> command to softreset the module
Parameters:  void         
Return:      Implementation status: 
               0: get successful  
               -1: The time limit was exceeded and no data was obtained                         
Others:      none
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
Parameters:  StringstrCmd: AT command
             timeout: Resend message after timeout
             reTry: retransmission number      
Return:      Implementation status: 
               0: get successful  
               -1: The time limit was exceeded and no data was obtained                         
Others:      none      
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
Description: Send AT command  to moudle
Parameters:  StringstrCmd:AT command
             *response :receiving the response indicates success
             timeout Resend message after timeout
             reTry :retransmission number      
Return:      AT command Ack  
Others:        
**********************************************************/
String BMC81M001::sendATCmd(String StringstrCmd,int timeout,uint8_t reTry)
{
  unsigned long t;
  char response[]="OK";
  String  AckString;
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
        AckString=String(BMC81M001Response);//arrayToString(BMC81M001Response, response);
        return AckString;
      }
      if((t + timeout) < millis())
      {
        AckString="TimeOut";
        break;
      }      
    }

  }
  return AckString;
}

/**********************************************************
  Description: Get surrounding WiFi information
  Parameters:  void
  Return:      The detectable Wifi information around
  Others:      It supports a maximum storage space of 1000 bytes
**********************************************************/
String BMC81M001::SSID(void)
{
  String result="AT error";
  if(sendATCommand("AT+CWLAP",5000,3)==SEND_SUCCESS)
  {
      result=String(BMC81M001Response);
  }
  return result;
}

/**********************************************************
Description: get wifi status
Parameters:  void   
Return:      recived wifi status
                2: The AP has been connected
                3: The AP has been connected and the TCP has also been connected
                4: The AP has been connected, but the TCP connection is disconnected
                5: AP not connected
Others:      
**********************************************************/
int BMC81M001::getStatus(void)
{
  int result=COMMUNICAT_ERROR;
  if(sendATCommand("AT+CIPSTATUS",1000,3)==SEND_SUCCESS)
  {
      char *pos=strchr(BMC81M001Response,':');
     // Serial.println(pos);
      if(pos!=NULL)
       result = atoi(&pos[1]);
      else
        result=AT_ACK_ERROR;
  }
  else
    result=COMMUNICAT_ERROR;
  return result;
}

/**********************************************************
Description: Get current connected SSID.
Parameters:  void   
Return:      The name of the currently connected WIFI
Others:      none
**********************************************************/
String BMC81M001::getSSID(void)
 {
  char *token;
   String result="AT error";
    if(sendATCommand("AT+CWJAP?",1000,3)==SEND_SUCCESS)
    {
      token = strtok(BMC81M001Response, "\"");// strchr(BMC81M001Response,'\"');
      if(token!=NULL)
      {
          token = strtok(NULL, "\"");// strchr(BMC81M001Response,'\"');
          if(token!=NULL)
          {
            result=String(token);
          }
           else {
              result="No wifi connected";
          }
      }
    }
    return result;
 }

/**********************************************************
Description: Get the IP address of the currently connected AP
Parameters:  void   
Return:      IP address
Others:      none
**********************************************************/
String BMC81M001::getIP(void)
{
  String AckString="AT error";
  char *token;
  char sourceStr[20]="\0";
  char s[2]=":";
  uint8_t i;
  if(sendATCommand("AT+CIPSTA?",1000,3)==SEND_SUCCESS)
  {
     token = strtok(BMC81M001Response, "\"");// strchr(BMC81M001Response,'\"');
      if(token!=NULL)
      {
          {
            token = strtok(NULL, "\""); 
            if(token!=NULL)
            {
              AckString=String(token);
            }
          }
      }  
   }
  return AckString;
}

/**********************************************************
Description: Get the Gateway address of the currently connected AP
Parameters:  void   
Return:      Gateway string
Others:      Information will only be available after connecting to the AP; otherwise, it will all be 0. Such as "172.20.10.1"
**********************************************************/
String BMC81M001::getGateway(void)
{
  String AckString="AT error";
  char *token;
  char sourceStr[20]="\0";
  char s[2]=":";
  uint8_t i;
  if(sendATCommand("AT+CIPSTA?",1000,3)==SEND_SUCCESS)
  {
     token = strtok(BMC81M001Response, "\"");// strchr(BMC81M001Response,'\"');
      if(token!=NULL)
      {
          for(i=0;i<3;i++)
          {
            token = strtok(NULL, "\""); 

          }
           if(token!=NULL)
            {
              AckString=String(token);
            }
      }  
   }
  return AckString;
}

/**********************************************************
Description: Get the Mask address of the currently connected AP
Parameters:  void   
Return:      Mask string
Others:      Information will only be available after connecting to the AP; otherwise, it will all be 0. Such as "255.255.255.0"
**********************************************************/
String BMC81M001::getMask(void)
{
  String AckString="AT error";
  char *token;
  char sourceStr[20]="\0";
  char s[2]=":";
  uint8_t i;
  if(sendATCommand("AT+CIPSTA?",1000,3)==SEND_SUCCESS)
  {
     token = strtok(BMC81M001Response, "\"");// strchr(BMC81M001Response,'\"');
      if(token!=NULL)
      {
          for(i=0;i<5;i++)
          {
            token = strtok(NULL, "\""); 
          }            
          if(token!=NULL)
          {
            AckString=String(token);
          }
      }  
   }
  return AckString;
}

/**********************************************************
Description: get MAC address
Parameters:  void   
Return:      MAC address
Others:      none  
**********************************************************/
String BMC81M001::getMacAddress(void)
{
  String AckString="AT error";
  char *token;
  char sourceStr[20]="\0";
  char s[2]=":";
  uint8_t i;
  if(sendATCommand("AT+CIPSTAMAC?",1000,3)==SEND_SUCCESS)
  {
     token = strtok(BMC81M001Response, "\"");// strchr(BMC81M001Response,'\"');
      if(token!=NULL)
      {
          for(i=0;i<6;i++)
          {
            token = strtok(NULL, ":"); 
            if(token!=NULL)
            {
              strcat(sourceStr,token);
                for(uint8_t i=0;i<12;i++)
              {
                sourceStr[i]=toupper(sourceStr[i]); 
              }
            }
          }
          sourceStr[12]='\0';
          AckString=String(sourceStr);
      }  
   }
  return AckString;
}

/**********************************************************
Description: get AT command  version
Parameters:  void   
Return:     AT command  version
Others:     none   
**********************************************************/
String BMC81M001::getATVersion(void)
{
  String AckString="AT error";
  char sourceStr[20];
  char *pos1,*pos2;
  if(sendATCommand("AT+GMR",1000,3)==SEND_SUCCESS)
  {
      pos1=strchr(BMC81M001Response,':');
      pos2=strchr(BMC81M001Response,'-');
      memcpy(sourceStr,pos1+1,pos2-pos1-1); 
      AckString=String(sourceStr);
   }
  return AckString;
}

/**********************************************************
Description: read data from module ,Send data to buffer
Parameters:  void
Return:      void
Others:      1. read data to buffer
             2. clear data buffer if the length is overflow
**********************************************************/
void BMC81M001::readResponse(void)
{
  if(_serial != NULL)
  {
  while(_serial->available())
  {
    uint8_t temp;
    temp = _serial->read();
    BMC81M001Response[resLength++] = temp;
    //Serial.write(temp);
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
Description: clear data buffer
Parameters:  Dbuffer:clear data buffer
Return:      void 
Others:      none  
**********************************************************/
void BMC81M001::clearResponse(char Dbuffer[])
{
  memset(Dbuffer,'\0',RES_MAX_LENGTH);
  resLength = 0;
}
