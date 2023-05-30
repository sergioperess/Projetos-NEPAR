//Arduino Micro Controlador - Projeto de medição de corrente NEPAR

#include "EmonLib.h" //biblioteca que possui os recursos para medir as grandezas basicas em corrente alternada
//#include <Wire.h> *biblioteca que contem as funções que gerenciam a comunicação entre os dispositivos pelo protocolo I2C
//#include <LiquidCrystal_I2C.h> *comunica com display LCD - não utilizado!

EnergyMonitor SCT013; //nome do objeto

int pinSCT = A3; //pino analógico conectado ao SCT013 000

int tensao = 127; //tensao do estado de Minas Gerais
int potencia; //armazena o resultado de (tensao x corrente)

//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE) *inicializa o display no endereco 0x27

void setup() 
{
  SCT013.current(pinSCT, 60.607); //funcao da biblioteca para armazenar o valor de calibracao 
  //valor de calibração = N2(número de espiras)/R(carga) = 2000/33 = 60,606
  
  //lcd.begin(16,2); *inicia LCD  
}
 
void loop() 
{
  double Irms = SCT013.calcIrms(1480); //calcIrms(1480) e uma funcao que calcula o valor da corrente *Irms e uma variavel que armazenara este valor *a funcao vai ler o pino 1480 vezes
  potencia = Irms * tensao; //calcula o valor da potencia instantanea

  //PARA SAIR NO DISPLAY:
  //lcd.clear();
  //lcd.setCursor(0,0);
  //lcd.print("Corr. = ");
  //lcd.print(Irms);
  //lcd.print(" A");

  //lcd.setCursor(0,1);
  //lcd.print("Pot. = ");
  //lcd.print(potencia);
  //lcd.print(" W");

  delay(500); //o programa deve esperar 500 milissegundos para passar para a próxima linha de código
}
