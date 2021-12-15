/*
 * -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: info@electroniciot.com
 * -------------------------------------------------------------------
 */

#include <Arduino.h>

bool ioBlink = false;
unsigned long milOld;
int rndTemp = 0;

// -------------------------------------------------------------------
// Simple blinking function - Pestañeo para Alarmas tiempo variable
// -------------------------------------------------------------------
void blinkSingle(int speed, int _pin){
    if((milOld + speed) < millis()){
        milOld = millis();
        if(ioBlink == false){
            digitalWrite(_pin,HIGH);
            ioBlink = true;
        }
        else{
            digitalWrite(_pin,LOW);
            ioBlink = false;
        }
    }
}
// -------------------------------------------------------------------
// Parpadeo Asincrónico Tiempo On y Tiempo Off tiempo variable
// -------------------------------------------------------------------
void blinkSingleAsy(int timeHigh, int timeLow, int _pin){
    if(ioBlink == false){
        if( (milOld + timeHigh) < millis()){
            milOld = millis();
            digitalWrite(_pin,LOW);
            ioBlink = true;
        }
    }
    else{
        if((milOld + timeLow) < millis()){
            milOld = millis();
            digitalWrite(_pin,HIGH);
            ioBlink = false;            
        }
    }
}
// -------------------------------------------------------------------
// Blinking with randomised delay como para TX/RX de Datos
// -------------------------------------------------------------------
void blinkRandomSingle(int minTime, int maxTime, int _pin){
    if((milOld + rndTemp) < millis()){
        milOld = millis();
        rndTemp = random(minTime, maxTime);
        if(ioBlink == false){
            digitalWrite(_pin,HIGH);
            ioBlink = true;
        }
        else{
            digitalWrite(_pin,LOW);
            ioBlink = false;
        }
    }
}
// -------------------------------------------------------------------
// On un Led
// -------------------------------------------------------------------
void setOnSingle(int _pin){
    digitalWrite(_pin, HIGH);
}
// -------------------------------------------------------------------
// Off un Led
// -------------------------------------------------------------------
void setOffSingle(int _pin){
    digitalWrite(_pin, LOW);
}
