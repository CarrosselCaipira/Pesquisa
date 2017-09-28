#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Carrega a biblioteca SoftwareSerial
#include <SoftwareSerial.h>

//Define os pinos para a serial
// 12=RX   13=TX
SoftwareSerial serialBT(12,13);

//Variável que receberá o byte enviado
char comando[9];
int command;



//Definindo nomes para os pinos
#define ENTRADA 1
#define MOTORE1 4
#define MOTORE2 7
#define MOTORD1 8
#define MOTORD2 2
#define RED 9
#define BLUE 10
#define GREEN 11
 







//===============================================================================================
//            SETUP
//===============================================================================================





void setup(){
  
  //Definindo pinos como entrada ou saída
  pinMode(ENTRADA, INPUT);  //Entrada serial através 
  pinMode(MOTORE1, OUTPUT); //Controle do motor Esquerdo
  pinMode(MOTORE2, OUTPUT); //Controle do motor Esquerdo
  pinMode(MOTORD1, OUTPUT); //Controle do motor Direito
  pinMode(MOTORD2, OUTPUT); //Controle do motor Direito
  pinMode(RED, OUTPUT); //Led Vermelho
  pinMode(GREEN, OUTPUT); //Led Verde
  pinMode(BLUE, OUTPUT); //Led Azul

  //Froçando motores a ficarem parados
  digitalWrite(MOTORE1, LOW);
  digitalWrite(MOTORE2, LOW);
  digitalWrite(MOTORD1, LOW);
  digitalWrite(MOTORD2, LOW);


  //Inicia a serial com baud 9600
  serialBT.begin(9600);
  Serial.begin(115200);
  
  
}



//================================================================================================
//            LOOP
//================================================================================================

void loop(){
  //Após inicializado o programa, chama constantemente a função receberComando()
  receberComando();

}

//=================================================================================================
//            FUNÇÕES
//=================================================================================================

//Chamado toda vez - determina qual função deve ser chamada segundo o byte recebido via bluetooth
void receberComando(){
  //RECEBENDO BYTE DA COMUNICAÇÃO BLUETOOTH
    
  //Enquanto houver algo na entrada serial ler e salvar em comando, ja convertido de inteiro para binário em char
  if (serialBT.available()){
      delay(10);
      command = serialBT.read();
      itoa(command,comando,2);
      Serial.println(command);
      
     
     
    //Consertando perda de bit por começar com 0
    if(strlen(comando) == 7){
         int i;
         for(i=7; i>=1; i--){
           comando[i] = comando[i-1];
         } 
         comando[0] = '0';
         comando[8] = '\n'; 
       
    }
    
    Serial.println(comando);
    
    //Determinando função recebida (os dois primeiros bits determinam a função)
    if(comando[0] == '0' && comando[1] == '0'){
      f00(comando);
    }else
      if(comando[0] == '0' && comando[1] == '1'){
      f01(comando);
    }else
      if(comando[0] == '1' && comando[1] == '0'){
      f10(comando);
    }else
      if(comando[0] == '1' && comando[1] == '1'){
      f11(comando);
    }
  }
    
}



//função 00: Movimentação e Cor de Led automático (Conforme velocidade)
void f00(char comando[8]){
  //Movimentação
  char esq[3], dir[3];
  
  esq[0] = comando[2];
  esq[1] = comando[3];
  esq[2] = comando[4];
  dir[0] = comando[5];
  dir[1] = comando[6];
  dir[2] = comando[7];
  
  int velE = 4*(esq[0]-'0') + 2*(esq[1]-'0') + 1*(esq[2]-'0'); 
  int velD = 4*(dir[0]-'0') + 2*(dir[1]-'0') + 1*(dir[2]-'0');

  //Valores menores que 3 rodam num sentido, enquanto maiores rodam em outro sentido


  //MOTOR Esquerdo
  
  if(velE<3){
    digitalWrite(MOTORE1,HIGH);
    digitalWrite(MOTORE2,LOW);
  }else if(velE==3){
    digitalWrite(MOTORE1,LOW);
    digitalWrite(MOTORE2,LOW);
  }else{
    digitalWrite(MOTORE1,LOW);
    digitalWrite(MOTORE2,HIGH);
  }

  //MOTOR Direito
  
  if(velD<3){
    digitalWrite(MOTORD1,HIGH);
    digitalWrite(MOTORD2,LOW);
  }else if(velD==3){
    digitalWrite(MOTORD1,LOW);
    digitalWrite(MOTORD2,LOW);
  }else{
    digitalWrite(MOTORD1,LOW);
    digitalWrite(MOTORD2,HIGH);
  }
 
  //Controle do LED               Variando com a velocidade de Azul até Vermelho

  
  analogWrite(GREEN, 0);                                    //Verde sempre 0
  analogWrite(RED, abs((velE*36+velD*36)/2-127)*2);       //Vermelho sobe com a velocidade média dos motores
  analogWrite(BLUE, abs(127-(velE*36+velD*36)/2)*2);      //Azul diminui com a velocidade média dos motores
}

//função 01: Determina cor do LED
void f01(char comando[8]){
  int r, g, b;
  char vermelho[2], verde[2], azul[2];
  
  vermelho[0] = comando[2]; 
  vermelho[1] = comando[3];
  verde[0] = comando[4];
  verde[1] = comando[5];
  azul[0] = comando[6];
  azul[1] = comando[7];
  
  r = (2*(vermelho[0]-'0') + 1*(vermelho[1] - '0')) * 85;
  g = (2*(verde[0]-'0') + 1*(verde[1] - '0')) * 85;
  b = (2*(azul[0]-'0') + 1*(azul[1] - '0')) * 85;
  
  Serial.println(r);
  Serial.println(g);
  Serial.println(b);
  
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
  
  
}

//função 10: Movimentaçao sem mudar cor do Led
void f10(char comando[8]){
  //Movimentação
  char esq[3], dir[3];
  
  esq[0] = comando[2];
  esq[1] = comando[3];
  esq[2] = comando[4];
  dir[0] = comando[5];
  dir[1] = comando[6];
  dir[2] = comando[7];
  
  int velE = 4*(esq[0]-'0') + 2*(esq[1]-'0') + 1*(esq[2]-'0'); 
  int velD = 4*(dir[0]-'0') + 2*(dir[1]-'0') + 1*(dir[2]-'0');

  //Valores menores que 3 rodam num sentido, enquanto maiores rodam em outro sentido


  //MOTOR Esquerdo

  //PWM pra um polo só
  
  Serial.println(velE);
  Serial.println(velD);
  
  if(velE<3){
    digitalWrite(MOTORE1,HIGH);
    digitalWrite(MOTORE2,LOW);
  }else if(velE==3){
    digitalWrite(MOTORE1,LOW);
    digitalWrite(MOTORE2,LOW);
  }else{
    digitalWrite(MOTORE1,LOW);
    digitalWrite(MOTORE2,HIGH);
  }

  //MOTOR Direito
  
  if(velD<3){
    digitalWrite(MOTORD1,HIGH);
    digitalWrite(MOTORD2,LOW);
  }else if(velD==3){
    digitalWrite(MOTORD1,LOW);
    digitalWrite(MOTORD2,LOW);
  }else{
    digitalWrite(MOTORD1,LOW);
    digitalWrite(MOTORD2,HIGH);
  }
  
}

//função 11: Modo de exibição (Roda loucamente e pisca)
void f11(char comando[8]){
  digitalWrite(MOTORD1, HIGH);
  digitalWrite(MOTORD2, LOW);
  digitalWrite(MOTORE1,LOW);
  digitalWrite(MOTORE2, HIGH);
}





//=========================================================
//            HELP
//=========================================================

/*
Conexões:

BLUETOOTH : 12 e 13 (TX e RX Respctivamente)
MOTOR DIREITO: PINO 4 e 7
MOTOR ESQUERDO: PINO 8 1
LED VERMELHO: PINO 9
LED AZUL: PINO 10
LED VERDE: PINO 11




=======================================================================================================================================================================
      Carrossel Caipira - Time de Róbitica - Unesp Bauru - Grupo de Pesquisa - 2017
      Feito como parte integrante do projeto de pesquisa Ratobô
      Integrantes:
        Izabele Pizzo
        Jorge Vicente Oliva Gonçalves
        Luis Fernando Uzai
        Marcelo Hideaki Iwata Kito
        Paulo Eduardo Manzone Maia
=========================================================================================================================================================================
*/
