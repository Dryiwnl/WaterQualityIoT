#include <Arduino.h>
#include "sensors.h"

float lerPH(){ 
    //função para ler o ph
    int leituraPino = analogRead(PIN_PH);
    float pH = leituraPino*(14.0/4095.0);
    /*o valor do ph é algo entre 0 a 14, e a resolução do ADC do esp32 é 4095
    então para obter o valor do ph fazemos, 14/4095*/
    return pH;
}

float lerTurbidez(){
    //função para ler a turbidez da água
    int leituraPino = analogRead(PIN_TURBIDEZ);
    float turbidez = leituraPino * (1000.0/4095.0);
    return turbidez;
}

float lerVazao(){
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);
    //faço a configuração dos pinos onde: pin_trig é saída e pin_echo é entrada

    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);
   /*Essa é a sequencia exigida pelo fabricante para o sensor lançar o ultrassom
   ou seja: desliga e espera 2ms para evitar ler valores fantasmas. Ligo por 10ms para
   avisar o sensor que ele deve liberar o ultrassom e desligo*/

    long duracao = pulseIn(PIN_ECHO, HIGH);
    float distancia = duracao * 0.017;
    return distancia; //em cm
    /*no momento que o sensor liberou o ultrassom, o sensor internamente salva o valor
    que demorou para o som bater em alguma superfície e voltar. Para acessar esse valor chamamos
    a função pulseIn(PIN_ECHO, HIGH), ou seja, o valor retornado é a duração em que o pino
    PIN_ECHO ficou como HIGH.
    fazemos a convesão para distancia usando a velocidade do som 0.017*/
}