#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define uS_TO_S_FACTOR 1000000ULL //
//fator que converte microsegundos para segundos
#define TIME_TO_SLEEP 3600 
//TIME_TO_SLEEP são 3600s que é o equivalente a 1h

const char* ssid = "Wokwi-GUEST";
const char* password = "";
// Credenciais da rede Wi-Fi virtual do Wokwi

const String url_banco = "https://waterquality-iot-49505-default-rtdb.firebaseio.com/leituras.json";
//usando um site para verificar se o código envia os dados via json

void setup() {
  Serial.begin(115200);
  
  Serial.print("Conectando ao WiFi");
  WiFi.begin(ssid, password);
  
  // Trava o código aqui até conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado!");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("Sistema Iniciado. Lendo sensores...");

  float valor_pH = lerPH();
  float valor_turbidez = lerTurbidez();
  float distancia = lerVazao();
  //chama as funções para ler os valores e coloca em suas respectivas variáveis
  
  Serial.print("pH: ");
  Serial.print(valor_pH);
  Serial.print(" | Turbidez: ");
  Serial.print(valor_turbidez);
  Serial.print(" | Distancia do sensor ate a lamina de agua: ");
  Serial.println(distancia);
  //imprime no temrinal só de teste por enquanto 
  bool tem_vazao = (distancia < 380.0);
  bool alerta_esgoto = false;
  if(valor_pH < 6.5 || valor_pH > 8.5 || valor_turbidez > 5.0){
    alerta_esgoto = true;
    Serial.println("Alerta: Qualidade da água comprometida!");
  }
  else{
    Serial.println("Qualidade da água dentro dos parâmetros normais.");
  }

  if (WiFi.status()==WL_CONNECTED){
  //testa se o wifi conectou
    
    WiFiClientSecure clienteSeguro;
    clienteSeguro.setInsecure(); 
    // Necessário para o Firebase aceitar a conexão HTTPS
    
    HTTPClient http;
    //cria um objeto do tipo HTTPClient para usar o protocolo http para enviar os dados
    
    http.begin(clienteSeguro, url_banco);
    //para enviar dados para o firebase aceitar os dados precisamos desse passo.
    
    http.addHeader("Content-Type","application/json");
    
    String json = "{";
    json += "\"pH\":" + String(valor_pH) + ",";
    json += "\"turbidez\":" + String(valor_turbidez) + ",";
    json += "\"distancia\":" + String(distancia) + ",";
    json += "\"tem_vazao\":" + String(tem_vazao ? "true" : "false") + ",";
    json += "\"alerta_esgoto\":" + String(alerta_esgoto ? "true" : "false");
    json += "}";
    //cria um json com ph, turbidez e distancia
    int codigo = http.PUT(json);
    /*de fato envio o dado usando o post do json que eu acabei de criar. e esse post gera um 
    código que se for 200 significa que o dado foi enviado com sucesso, se não da algo -1 */
    
    if(codigo>0){
      Serial.println("Dados enviados. Código resposta: ");
      Serial.println(codigo);
    }
    else{
      Serial.println("Erro no envio.");
      Serial.println(codigo);
    }
    http.end();
  }

  Serial.println("Entrando no modo deepSleep");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP*uS_TO_S_FACTOR);
  Serial.flush();
  esp_deep_sleep_start();
  /*Configura o esp para ao final e toda a leitura e envio para o banco de dados entrar no modo deep
  sleep por TIME_TO_SLEEP*uS_TO_S_FACTOR segundos, ou seja, após passar uma hora o timer acorda o esp*/
  /*Por esse motivo toda a lógica do dispositivo está na parte de setup e não no loop, pq tod vez que 
   o esp entra no modo deepsleep ele sofre um reset.*/
}

void loop() {

}