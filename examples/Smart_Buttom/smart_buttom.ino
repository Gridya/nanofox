/**
**************************************************************************************************
* @file    smart_buttom.ino
* @author  Edson Jeske e Kelvin Welter / Gridya Tecnologia
* @brief   Smart Buttom usando Nanofox IoT.
* 
*     - Envia um sinal através da rede Sigfox quando pressionado um botão por 2 segundos
*            
* License: Public Domain
**************************************************************************************************
*/


/*-----HEADER FILES--------------------------------------------------------------------------------*/
#include <Nanofox.h>   //Nanofox Library

/*------Global Variables---------------------------------------------------------------------------*/
uint16_t Counter_Sig = 0;        //Contador para testes
unsigned long timeref_ms;        //Referência de tempo para cálculos de atraso

uint8_t Uplink_Buffer[12];    //Buffer para o uplink do Payload
uint8_t Downlink_Buffer[8];   //Buffer para o Downlink do Payload


/*------Objects -----------------------------------------------------------------------------------*/
Nanofox MyNanofox;    //Nanofox Object Instance

void setup() {

Serial.begin(9600);   //Inicialização do Serial

MyNanofox.Init_ArduinoNano_IO();  //Configuração do Arduino Nano IO
MyNanofox.Init_Modem_WISOL(RC2);  //Inicialização do WISOL Sigfox Modem
Serial.println("Welcome to NANOFOX IoT Kit!");
Serial.println("Smartbuttom com Nanofox IoT");
Serial.println("Pressione o botão por 2 segundos para ativar");
pinMode(2, INPUT_PULLUP);

Uplink_Buffer[0] = 01;

timeref_ms = millis();  // Inicialização da referência de tempo
}

void loop() {

  delay(10);
  
  while(digitalRead(2) == LOW){ //Leitura do pino D2 e estrutura de repetição while para caso o botão seja pressionado
    
  delay(2000); //Aguardo de 2 segundos para conferir novamente se o botão está pressionado

    if(digitalRead(2) == LOW){ //Se o botão estiver pressionado após os dois segundos, a placa Nanofox IoT transmite sinal 
      Serial.println("Botão pressionado! Envio de mensagem para rede Sigfox iniciado.");  
      MyNanofox.Send_Payload_Sigfox(&Uplink_Buffer[0],1,&Downlink_Buffer[0],0);
    }
  }
}
