#include <Arduino.h>
#include <WiFi.h>
#include "sensors.h"

// Credenciais da rede Wi-Fi virtual do Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

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
}

void loop() {
  float valor_pH = lerPH();
  float valor_turbidez = lerTurbidez();
  float distancia = lerVazao();
  
  Serial.print("pH: ");
  Serial.print(valor_pH);
  Serial.print(" | Turbidez: ");
  Serial.print(valor_turbidez);
  Serial.print(" | Distancia do sensor ate a lamina de agua: ");
  Serial.println(distancia);

  delay(2000);
}