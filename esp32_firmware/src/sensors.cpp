#include <Arduino.h>
#include "sensors.h"

float lerPH(){
    int leituraPino = analogRead(PIN_PH);
    float pH = leituraPino*(14.0/4095.0);
    return pH;
}

float lerTurbidez(){
    int leituraPino = analogRead(PIN_TURBIDEZ);
    float turbidez = leituraPino * (1000.0/4095.0);
    return turbidez;
}

float lerVazao(){
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    long duracao = pulseIn(PIN_ECHO, HIGH);
    float distancia = duracao * 0.017;
    return distancia;
}