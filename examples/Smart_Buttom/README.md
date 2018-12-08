# Smart Buttom Sigfox usando Nanofox IoT Kit

Essa página apresenta um tutorial de um botão inteligente (smart buttom) usando o kit Nanofox IoT e a rede Sigfox.

O termo Smart Buttom é geralmente associado a um botão com conexão a internet, que quando pressionado pode disparar algum serviço associado. Alguns exemplos são botões de emergência/pânico, chamada para manutenção de algum equipamento, Chamada para atendimento VIP, etc.    

No nosso tutorial o botão quando acionado, envia uma mensagem para a plataforma Tago.io que dispara um email de notificação. 

## Recomendações:
Para realizar esse tutorial você vai precisar:

- [Arduino IDE instalada no computador.](https://www.arduino.cc/en/Guide/HomePage)
- [Conta cadastrada na plataforma Tago.](https://tago.io/)
- [Kit Nanofox IoT ativado.](https://github.com/Gridya/NANOFOX-Activate/blob/master/README.md)

## Primeiro passo: Circuito eletrônico (hardware)
  O circuito é muito simples, consistindo apenas em um botão ligado a algum pino de I/O do Arduino (pino D2 no nosso exemplo). 
  
  Em nosso projeto, optamos por realizar a alimentação via porta USB (cabo fornecido junto com o Kit), no entanto uma bateria externa ligada na porta de alimentação da placa pode ser utilizada. 
  
  Informações completas sobre a placa, como datasheet, esquemático e manual de uso estão disponíveis para [download.](https://github.com/Gridya/NANOFOX_Documentation)  
  
  [![IMG-20181204-213812.jpg](https://i.postimg.cc/wjSjxBYH/IMG-20181204-213812.jpg)](https://postimg.cc/4YQgBs3F "Placa Nanofox IoT montada com antena e botão externo")
  
## Segundo passo: Programação do Arduino (firmware)
  Para realizar a leitura do botão e envio da mensagem via rede Sigfox, um programa muito simples foi desenvolvido. O código fonte pode ser clonado deste repositório, compilado e carregado para a placa Arduino Nano do Kit Nanofox. 
  
  Segue abaixo o código fonte e uma descrição resumida:
  
  ```c++
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
  ```
 
  #### Bibliotecas necessárias:
  O código utiliza a biblioteca [nanofox](https://github.com/Gridya/nanofox), responsável pela comunicação com o modem Sigfox Wisol presente no Kit, que realiza a transmissão de dados para a rede Sigfox.
  #### Função Setup:
  Como necessitamos da leitura de um botão, devemos tratar o pino correspondente como *input*. Nesse caso, iremos escolher o pino D2. 
Além da nossa configuração necessária para o projeto, devemos adicionar mais algumas coisas como a configuração do Arduino Nano IO e a inicialização do Modem Sigfox WISOL. Além disso, no código fornecido configuramos algumas mensagens para serem mostradas no monitor serial, essa parte é totalmente opcional e o código funciona sem ela.
  #### Função Loop: 
  No loop principal o estado do botão é monitorado. Se este for pressionado por mais de 2 segundos, então é enviada uma mensagem via rede Sigfox. 
  
  As mensagens deverão chegar no backend sigfox de forma similar ao apresentado na imagem abaixo.
  
  [![backend-message.png](https://i.postimg.cc/q7kZ3sTG/backend-message.png)](https://postimg.cc/Xrz8MyXZ "Backend Sigfox com sinal recebido")
  
## Terceiro passo: Integração da plataforma Tago com o Backend Sigfox
  Essa etapa é muito bem relatada e explicada a partir de um [tutorial escrito pela própria Tago](https://tago.elevio.help/en/articles/33). Nesse tutorial estão descritas as configuração necessárias tanto na plataforma Tago quanto na criação de Callbacks no Backend Sigfox para direcionar as mensagens Sigfox para a aplicação rodando na Tago.
  
## Quarto passo: Configuração para envio de e-mail via plataforma Tago
  Após concluída a integração da plataforma Tago com o backend Sigfox, vamos criar um trigger na plataforma Tago para para enviar um e-mail sempre que for recebida uma mensagen de botão pressionado. Para começarmos, acessaremos a opção **Action** no menu lateral da página de desenvolvedor na Tago. Após entrarmos em **Action**, clicamos no botão **Add Action** no canto superior direito. Assim, entramos em uma nova ação a ser criada. Na aba **General Information** damos um nome a nossa ação, escolhemos a ação a ser tomada (**Send Email**) e preenchemos com o destinatário, título e conteúdo do email. A configuração nessa aba deve ficar assim:
  
  [![general-information.png](https://i.postimg.cc/kXxqs3F1/general-information.png)](https://postimg.cc/3dJsrqqm "Configuração da aba General Information")
  
  A seguir, precisamos ir para a aba **Trigger**, nessa aba iremos configurar quando a plataforma Tago deve enviar o e-mail. Queremos que ela envie sempre que um sinal Sigfox chegue na plataforma, portanto, devemos **configurar a condição de acionamento da ação como Any** e também devemos escolher uma variável que será monitorado para a condição de acionamento, no caso, escolheremos a variável **data**. Além disso, devemos **desativar a opção Lock trigger after action is taken**. Feito isso, a configuração deve ficar da seguinte maneira:
  
  [![trigger.png](https://i.postimg.cc/YCT6rzwG/trigger.png)](https://postimg.cc/XrcGxFq4 "Configuração da aba Trigger")
  
  Após isso, nossa ação já está pronta para ser salva e usada! Antes de testarmos a nossa ação é importante lembrar que existem muitos mais recursos na plataforma Tago, explore um pouco a aba **Dashboard**, nessa aba podemos utilizar as mais diversas formas para visualização de nossos dados, um exemplo é a criação de uma tabela com a ID do nosso dispositivo, data e o horário que os emails foram enviados:
  
  [![tabela-emails.png](https://i.postimg.cc/6QJ4jc3d/tabela-emails.png)](https://postimg.cc/4YP3nzpd "Tabela com ID, data e horário")
  
  Vamos agora testar a nossa action que acabamos de criar, caso você executar a ação necessária para envio de dados para a rede Sigfox, você deverá então receber, em poucos segundos, um email conforme você configurou na plataforma Tago. A imagem exemplifica isso:
  
  [![email-recebido.png](https://i.postimg.cc/tR3Nt0mX/email-recebido.png)](https://postimg.cc/4n3cs0gM "Email recebido!")
  
  Pode-se mostrar também um exemplo usando uma tabela na aba Dashboard da plataforma Tago. Montando uma tabela dinâmica que contenha todas as datas e horários em que foi enviado um email. O exemplo abaixo demostra isso:
  
  ![Gif_Smart_Buttom](https://media.giphy.com/media/kFezlCCsnJ4OiMOgvR/giphy.gif)
  
  Agora você está pronto para ir muito além no mundo IoT, sua primeira aplicação usando a placa Nanofox IoT já foi feita! Liberte sua imaginação e mão na massa! Os seus projetos poderão ser cada vez mais interessantes e complexos, projete sem medo! A Nanofox IoT dá conta
