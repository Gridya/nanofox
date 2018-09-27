/**
  **************************************************************************************************
  * @file    NANOFOX_Activate_v0.ino
  * @author  Edson Jeske / Gridya Tecnologia
  * @brief   NANOFOX IoT Kit - Factory default software. 
  * 
  *     - Prints PAC and DEVICE ID of the WISOL module through serial port. 
  *       (Necessary for activating the kit in backend.sigfox.com/activate)
  * 
  *     - Pressing the general purpose button, sends a test uplink message. 
	*						
  * License: Public Domain
  **************************************************************************************************
  */


/*-----HEADER FILES--------------------------------------------------------------------------------*/
#include <Nanofox.h>   //Nanofox Library

/*------Global Variables---------------------------------------------------------------------------*/
uint16_t Counter_Sig = 0;        //Counter for testing 
unsigned long timeref_ms;        //Reference time for delay calculations

uint8_t Uplink_Buffer[12];		//Buffer for uplink Payload
uint8_t Downlink_Buffer[8];		//Buffer for Downlink Payload


/*------Objects -----------------------------------------------------------------------------------*/
Nanofox MyNanofox;		//Nanofox Object Instance

void setup() {

  Serial.begin(9600);   //Initi Debug serial port
  
  MyNanofox.Init_ArduinoNano_IO(); 	//Setup arduino Nano IO
  MyNanofox.Init_Modem_WISOL(RC2);  //Initialize WISOL Sigfox Modem
  Serial.println("Welcome to NANOFOX IoT Kit!");
  Serial.println("Access backend.sigfox.com/activate to activate the Kit.");
  Serial.println("Press on board button to send a Sigfox message.");
  Serial.println("");
  MyNanofox.Print_PAC_WISOL();	//Prints Modem PAC Number in serial Debug
  MyNanofox.Print_DEVICE_ID_WISOL(); 	//Prints Modem Device ID in serial Debug
  
  digitalWrite(LED, HIGH); //Blink general use LED (LD1) 
  delay(2000);
  digitalWrite(LED, LOW);
  
  timeref_ms = millis();	// Init time reference 
}

void loop() {
  
	delay(10);
	
	if((millis() - timeref_ms) > 20000) //Prints PAC and Device ID every 20s
	{ 
		timeref_ms = millis(); //restart time reference
    
		MyNanofox.Print_PAC_WISOL();
		MyNanofox.Print_DEVICE_ID_WISOL();
	}
  
	if(digitalRead(Buttom) == 0) //Send Sigfox msg when button pressed
	{
		Serial.println("Button pressed!");
		
		Uplink_Buffer[0] = Counter_Sig++;
		Uplink_Buffer[1] = Counter_Sig++;
		
		MyNanofox.Send_Payload_Sigfox(&Uplink_Buffer[0],2,&Downlink_Buffer[0],0);
	} 
}








 
