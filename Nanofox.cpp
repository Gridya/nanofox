/**
  **************************************************************************************************
  * @file    Nanofox.cpp
  * @author  Edson Jeske / Gridya Tecnologia
  * @brief   NANOFOX IoT Kit Library source file. 
  * 	
  *		Provides access to Wisol Sigfox Modem using Arduino Nano board in
  *		Nanofox IoT Kit.      
	*						
  * License: Public Domain
  **************************************************************************************************
  */
#include "Nanofox.h"

SoftwareSerial WisolSer(7, 6);          //Pin D7 as RX, Pino D6 as TX

Nanofox::Nanofox() {
	
}

/** @brief  Init Arduino IO hardware
  *         
  * @param  None
  * @retval None
  */
void Nanofox::Init_ArduinoNano_IO(void) {
 
	  // I/O pins setup
	digitalWrite(Wake_UP, HIGH);
	digitalWrite(LED, LOW);
	pinMode(LED, OUTPUT); //LED
	pinMode(Buttom, INPUT);  //Button
	pinMode(Wake_UP, OUTPUT);  //Wakeup modem pin
	WisolSer.begin(9600); //Serial for modem interface
}

/** @brief  Init Wisol Modem
  *         
  * @param  Region - Region of Sigfox: RC2 or RC4 
  * @retval None
  */
void Nanofox::Init_Modem_WISOL(unsigned int Region)
{
  
  //Software reset modem WISOL
  Serial.println("Configuring Modem...");
  
  WisolSer.print(WISOL_AT_Reset);
  Print_Wisol_Response(1000, 5000);
  
  if(Region == RC2)
  {
    WisolSer.print(WISOL_AT_DR_RC2);
    Print_Wisol_Response(100, 1000);
    WisolSer.print(WISOL_AT_S400_RC2);
    Print_Wisol_Response(100, 1000);
    WisolSer.print(WISOL_AT_WR);
    Print_Wisol_Response(100, 1000);
    WisolSer.print(WISOL_AT_ATRC);
    Print_Wisol_Response(100, 1000);
    
    
  }else if (Region == RC4)
    {
      WisolSer.print(WISOL_AT_DR_RC4);
      Print_Wisol_Response(100, 1000);
      WisolSer.print(WISOL_AT_S400_RC4);
      Print_Wisol_Response(100, 1000);
      WisolSer.print(WISOL_AT_WR);
      Print_Wisol_Response(100, 1000);
      WisolSer.print(WISOL_AT_ATRC);
      Print_Wisol_Response(100, 1000);
    
    }else 
      {
        Serial.println("Not supported region.");
        while(1){} //Fail - Not supported region
      }
  
}

/** @brief  Prints the responses from Wisol Modem
  *         
  * @param  pre_delay_ms - Time in ms of delay before reading reception buffer
  * @param 	timeout_ms - Timeout after start reading buffer to prevent deadlock   
  * @retval None
  */
void Nanofox::Print_Wisol_Response(unsigned long pre_delay_ms, unsigned long timeout_ms)
 {
    delay(pre_delay_ms); //waits before reading serial data
    
    unsigned long timeref_lc_ms = millis();
    
    while ((WisolSer.available() > 0) && ((millis() - timeref_lc_ms) < timeout_ms))
    {
      Serial.write(WisolSer.read()); //Prints received data from Modem
    }
   
 }
 

/** @brief  Reads data received from Wisol Modem and send to external buffer. Used for reading
  *			Downlink messages 
  *         
  * @param  pre_delay_ms - Time in ms of delay before reading reception buffer
  * @param 	timeout_ms - Timeout after start reading buffer to prevent deadlock 
  * @param 	out_pt - Pointer to external buffer where data will be sent 
  * @retval None
  */
void Nanofox::Read_Wisol_Response(unsigned long pre_delay_ms, unsigned long timeout_ms, uint8_t *out_pt)
{
	uint8_t temp_data;
	uint8_t couter = 0;
	unsigned long timeref_lc_ms = 0;
	
	timeref_lc_ms = millis();
	while (((millis() - timeref_lc_ms) < pre_delay_ms))
    {
		delay(500);
		Serial.print(".");
    }
	Serial.println("");
    
    timeref_lc_ms = millis();
    
    while ((WisolSer.available() > 0) && ((millis() - timeref_lc_ms) < timeout_ms))
    {
		temp_data = WisolSer.read();
		*out_pt = temp_data;
		out_pt++;
		Serial.write(temp_data); //Prints received data from Modem
		couter++;
		
		if(couter >= Com_Buffer_Size_Ct)
		{
			Serial.print("Buffer Overflow error");
				break;
		}
    }
	
}

/** @brief  Prints PAC number of Wisol Modem
  *         
  * @param  None  
  * @retval None
  */
void Nanofox::Print_PAC_WISOL(void)
{
    WisolSer.print(WISOL_AT_PAC); //Send PAC Command
    delay(30);
    Serial.print("PAC: ");
    Print_Wisol_Response(200, 1000); 
}

/** @brief  Prints DEVICE ID number of Wisol Modem
  *         
  * @param  None  
  * @retval None
  */
void Nanofox::Print_DEVICE_ID_WISOL(void)
{
    WisolSer.print(WISOL_AT_DeviceID); //Send DEVICE ID Command
    delay(30);
    Serial.print("Device ID: ");
    Print_Wisol_Response(200, 1000);
}

/** @brief  Sends Payload to Sigfox Network and receives Downlink Payload 
  *         
  * @param  in_pt - Pointer to Input Buffer with data to be sent
  * @param 	size - Size in bytes of the data to be sent 
  * @param 	out_pt - Pointer to output buffer to store received Downlink data. Must be >= 8 bytes 
  * @param 	downlink - Indicates if a Downlink will be requested. 1 = Yes ; 0 = No  
  * @retval -1 Indicates fail; 1 Indicates success
  */
int Nanofox::Send_Payload_Sigfox(uint8_t *in_pt, uint8_t size, uint8_t *out_pt, uint8_t downlink)
{
	int status_lc;
	uint8_t i;
	int X = 0;
	int Y = 0;
	uint8_t WISOL_Data[Com_Buffer_Size_Ct];      //Data reception buffer
	uint8_t *in_pt_aux = in_pt;
    		
	if(size > 12)
	{
		status_lc = -1;
		Serial.println("Error - Payload must be <= 12 bytes");
	}
	else
	{	
		if(downlink == 1)
		{
			Serial.println("Sending Sigfox Message with Downlink Request.");
		}else
		{
			Serial.println("Sending Sigfox Message.");
		}
	
		WisolSer.print(WISOL_AT_ATGI);
		delay(200);
		
		//reset buffer
		WISOL_Data[0] = 0;
		WISOL_Data[1] = 0;
		WISOL_Data[2] = 0;
		i = 0;
	  
		while (WisolSer.available() > 0)
		{
		  // Get Modem response
		  WISOL_Data[i] = (char)WisolSer.read();
		  i++;
		}
	  
		//Convert to decimal
		X = WISOL_Data[0] - 48;
		Y = WISOL_Data[2] - 48;
	  
		//This procedure is necessary based on modem datasheet
		if((X == 0) || (Y < 3))
		{
		  //Serial.println("Sending AT$RC command to Modem");
		  WisolSer.print(WISOL_AT_ATRC);
		  Print_Wisol_Response(200, 1000);
		}

		//Send payload 
		WisolSer.print(WISOL_AT_ATSF);				
		
		for(i = 0;i < size;i++)
		{
			WISOL_Data[i] = *in_pt;
			
			if(WISOL_Data[i] < 16)
			{
				WisolSer.print("0");
			}			
			WisolSer.print(WISOL_Data[i], HEX);
		
			in_pt++;
		}
				
		for(i = 0;i < Com_Buffer_Size_Ct;i++)
		{
			WISOL_Data[i] = '\0'; //apaga buffer Sigfox
		}
		
		if(downlink == 1)
		{
			WisolSer.print(",1\r");
			Read_Wisol_Response(46000, 5000, &WISOL_Data[0]);
			
			if (String_Compare(&WISOL_Data[0], "OK", 2) == 1){
			
				int index = 7;
				for(i = 0;i < 8;i++)
				{
					*out_pt = ASCII2Byte(&WISOL_Data[index]);
					out_pt++;
					index = index+3;
				}
				
				status_lc = 1;
			
			}else {
				status_lc = -1;
			}
			
		}else{
			
			WisolSer.print("\r");
			Read_Wisol_Response(10000, 5000, &WISOL_Data[0]);
			
			if (String_Compare(&WISOL_Data[0], "OK", 2) == 1){
				status_lc = 1;
			}else {
				status_lc = -1;
			}
		}
	
	}
	
	if(status_lc == 1)
	{ //debug only
		Serial.print("Sent Payload: ");
		for(i = 0;i < size;i++)
		{
			WISOL_Data[i] = *in_pt_aux;
			
			if(WISOL_Data[i] < 16)
			{
				Serial.print("0");
			}
			Serial.print(WISOL_Data[i], HEX);
		
			in_pt_aux++;
		}
		Serial.print("\n");
	}else
	{
		Serial.print("Communication Error");
	}
	
	return status_lc;
	
}

/** @brief  Set power mode of Wisol Mode and can Reset the modem
  *         
  * @param  mode = Wisol_Reset Executes a Software reset
  *				 = Wisol_Sleep Puts the Modem in Sleep Mode			1
  * 			 = Wisol_Deep_Sleep Puts the Modem in Deep Sleep. Can only be wake up using 
  * 			   function Wakeup_From_Deep_Sleep_Modem_WISOL(void)	  
  * @retval None
  */ 
void Nanofox::Set_Powermode_Modem_WISOL(uint8_t mode)
{
	if(mode == Wisol_Reset)
	{
		Serial.print("Resetting...");
		WisolSer.print(WISOL_AT_Reset);
		Print_Wisol_Response(1000, 5000);
		
	}else if(mode == Wisol_Sleep)
	{
		Serial.println("Entering Sleep mode.");
		WisolSer.print(WISOL_AT_Sleep);
		Print_Wisol_Response(1000, 5000);
		
	}else if(mode == Wisol_Deep_Sleep)
	{
		Serial.println("Entering Deep Sleep mode.");
		WisolSer.print(WISOL_AT_DeepSleep);
		Print_Wisol_Response(1000, 5000);
		
	}else
	{
		Serial.println("No action...");
	}
	
}

/** @brief  Wake up the modem from a Deep Sleep State
  *         
  * @param  None  
  * @retval None
  */
void Nanofox::Wakeup_From_Deep_Sleep_Modem_WISOL(void)
{
	
	digitalWrite(Wake_UP, LOW);
   delay(500); 
   digitalWrite(Wake_UP, HIGH);
   delay(500); 
   digitalWrite(Wake_UP, LOW);
   delay(500); 
   digitalWrite(Wake_UP, HIGH);
	delay(500);  
	
	Set_Powermode_Modem_WISOL(Wisol_Reset);
	
}

/** @brief  Compares two strings 
  *         
  * @param  in1 - Pointer to String 1
  * @param 	in2 - Pointer to String 1
  * @param 	len - Size in bytes of the  chars to be compared 
  * @retval 0 Indicates strings are different; 1 Indicates Strings are equal
  */  
int Nanofox::String_Compare(uint8_t *in1, uint8_t *in2, uint16_t len){
	
	for (int i = 0; i<len; i++){
		
		if (*in1 != *in2){
			return 0;
		}
		in1++;
		in2++;
	}
	return 1;
}

/** @brief  Converts a two character ASCII into a Byte 
  *         
  * @param  input - Pointer to the ASCII string 
  * @retval unsigned int 8bit value
  */
uint8_t Nanofox::ASCII2Byte(uint8_t *input){
   
   uint8_t local[2];
   uint8_t result;
   
   local[0] = *input;
   input++;
   local[1] = *input;
   
   if(local[0] > 57)
   {
	   local[0] = 15 - (70 - local[0]); 
   }else
   {
	   local[0] = 9 - (57 - local[0]);
   }
   
   if(local[1] > 57)
   {
	   local[1] = 15 - (70 - local[1]); 
   }else
   {
	   local[1] = 9 - (57 - local[1]);
   }
   
   
   result = ((unsigned char) local[0] << 4) | (unsigned char) local[1];
   
   return(result);
  
}


/*********************************************************************/
