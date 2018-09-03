/**
  **************************************************************************************************
  * @file    Nanofox.h
  * @author  Edson Jeske / Gridya Tecnologia
  * @brief   NANOFOX IoT Kit Library header file. 
  * 	
  *		Provides access to Wisol Sigfox Modem using Arduino Nano board in
  *		Nanofox IoT Kit.
  *      
	*						
  * License: Public Domain
  **************************************************************************************************
  */
#ifndef _Nanofox_H
#define _Nanofox_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <SoftwareSerial.h>


/*------#defines-----------------------------------------------------------------------------------*/
//WISOL AT Commands (see module datasheet for full command set)
#define WISOL_AT_Reset    "AT$P=0\r"      //Modem reset
#define WISOL_AT_Sleep       "AT$P=1\r" 	  // Enter Sleep mode
#define WISOL_AT_DeepSleep   "AT$P=2\r" 	  // Enter Deep Sleep mode
#define WISOL_AT          "AT\r"          //Test communication
#define WISOL_AT_DeviceID "AT$I=10\r"     //Get device ID 
#define WISOL_AT_PAC      "AT$I=11\r"     //Get PAC
#define WISOL_AT_ATGI     "AT$GI?\r"      //Send data - step1  
#define WISOL_AT_ATRC     "AT$RC\r"       //Send data - step2
#define WISOL_AT_ATSF     "AT$SF="        //Send data - step3


//WISOL AT Commands for Region configuration
#define WISOL_AT_DR_RC4     "AT$DR=922300000\r"
#define WISOL_AT_S400_RC4   "ATS400=<00000000><F0000000><0000001F>,63\r"
#define WISOL_AT_DR_RC2     "AT$DR=905200000\r"
#define WISOL_AT_S400_RC2   "ATS400=<000001FF><00000000><00000000>,1\r"
#define WISOL_AT_WR         "AT$WR\r"

//Buffer size
#define Up_Buffer_Size_Ct 24
#define Down_Buffer_Size_Ct 16
#define Com_Buffer_Size_Ct 50	//Auxiliary buffer 

//Power Modes
#define Wisol_Reset			0
#define Wisol_Sleep			1
#define Wisol_Deep_Sleep	2


//Sigfox Regions
#define RC2   2   //Brazil, Mexico, USA

#define RC4   4   //Australia, New Zealand, Singapore, Taiwan, Hong Kong, Colombia, Argentina, 
                  //Costa Rica, Thailand, Malaysia, Ecuador, Panama, El Salvador

//Arduino IOs
#define Buttom   4        //General purpose on board button
#define LED      9         //General purpose on board LED
#define Wake_UP  5         //Wake up modem from Sleep


class Nanofox {
 public:
  Nanofox();
  void Init_ArduinoNano_IO(void);
  void Init_Modem_WISOL(unsigned int Region);
  int Send_Payload_Sigfox(uint8_t *in_pt, uint8_t size, uint8_t *out_pt, uint8_t downlink);
  void Print_PAC_WISOL(void);
  void Print_DEVICE_ID_WISOL(void);
  void Set_Powermode_Modem_WISOL(uint8_t mode);
  void Wakeup_From_Deep_Sleep_Modem_WISOL(void);
  
  
 private:
	void Print_Wisol_Response(unsigned long pre_delay_ms, unsigned long timeout_ms);
	void Read_Wisol_Response(unsigned long pre_delay_ms, unsigned long timeout_ms, uint8_t *out_pt);
	int String_Compare(uint8_t *in1, uint8_t *in2, uint16_t len);
	uint8_t ASCII2Byte(uint8_t* input);
	
  
};

#endif
