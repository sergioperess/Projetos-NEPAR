#include "EmonLib.h" //biblioteca que possui os recursos para medir as grandezas basicas em corrente alternada

EnergyMonitor SCT013; //nome do objeto

int pinSCT = A3; //pino analógico conectado ao SCT013 000

int tensao = 127; //tensao do estado de Minas Gerais
int potencia; //armazena o resultado de (tensao x corrente)

void setup() 
{
  SCT013.current(pinSCT, 60.607); //funcao da biblioteca para armazenar o valor de calibracao 
  //valor de calibração = N2(número de espiras)/R(carga) = 2000/33 = 60,606
}
 
void loop() 
{
  double Irms = SCT013.calcIrms(1480); //calcIrms(1480) e uma funcao que calcula o valor da corrente *Irms e uma variavel que armazenara este valor *a funcao vai ler o pino 1480 vezes
  potencia = Irms * tensao; //calcula o valor da potencia instantanea
  delay(500); //o programa deve esperar 500 milissegundos para passar para a próxima linha de código
}
