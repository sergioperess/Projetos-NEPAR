
#include <EmonLib.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

EnergyMonitor SCT013;
//definição de pinos e variaveis
int pinSCT = A0;
int tensao = 127; //tensao do estado de Minas Gerais
int potencia; //armazena o resultado de (tensao x corrente)


//definição das configurações de Wifi e broker mqtt
const char* ssid = "SA";
const char* password = "pateta10";
const char* mqtt_server = "152.67.37.25";
const int mqttPort = 1883;
const char* mqttUser = "nepar";
const char* mqttPassword = "88221886";

// Criação de usuario Wifi
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// inicialização 
void setup() {

  Serial.begin(115200);     //inicialização do monitor serial
  setup_wifi();             //inicialização Wifi
  client.setServer(mqtt_server, 1883);    //inicialização mqtt
  client.setCallback(callback);     //inicialização função callback que retorna um valor quando for requerida
  pinMode (pinSCT,INPUT);   //inicializando sensor analógico como INPUT (entrada)
  SCT013.current(pinSCT, 60.607); //funcao da biblioteca para armazenar o valor de calibracao 
  //valor de calibração = N2(número de espiras)/R(carga) = 2000/33 = 60,606
 
}

// função para conectar Wifi
void setup_wifi() {

  delay(10);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// função callback que espera retorna um valor quando solicitado pelo broker sem parar o loop
void callback(char* topic, byte* payload, unsigned int length) {  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  }

}

//função que reconecta o wifi 
void reconnect() {
  
   while (!client.connected()) {
        Serial.println("Connecting to MQTT…");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str(), mqttUser, mqttPassword )) {
            Serial.println("connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }
 
}
float leituraSensor = 0;
void loop() {

  if (!client.connected()) {  //se o wifi nao estiver conectado, é reconectado
      reconnect();
  }
  client.loop(); //espera uma resposta mqtt sem parar o loop

      double Irms = SCT013.calcIrms(1480); //calcIrms(1480) e uma funcao que calcula o valor da corrente *Irms e uma variavel que armazenara este valor *a funcao vai ler o pino 1480 vezes
      potencia = Irms * tensao; //calcula o valor da potencia instantanea
      delay(500); 
  
      Serial.print(potencia);
      client.publish("corrente", String(potencia).c_str());   //publica no node o valor do dado analógico

          
        
  
  

 
}
