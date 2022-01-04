/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/

// -------------------------------------------------------------------
// Librerias
// -------------------------------------------------------------------
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <TimeLib.h>

// -------------------------------------------------------------------
// Archivos *.hpp - Fragmentar el Código
// -------------------------------------------------------------------
#include "settings.hpp"
#include "functions.hpp"
#include "settingsReset.hpp"
#include "settingsRead.hpp"
#include "settingsSave.hpp"
#include "esp32_wifi.hpp"
#include "esp32_mqtt.hpp"
#include "esp32_websocket.hpp"
#include "esp32_server.hpp"

// -------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------
void setup() {
    // Baudrate
    Serial.begin(115200);
    // CPU Freq
    setCpuFrequencyMhz(240);
    // Inicio del Log por serial
    log("\nInfo: Iniciando Setup");
    // Configurar los Pines
    settingPines();
    // Inicio del SPIFFS                 
    if (!SPIFFS.begin()){
        log(F("Error: Falló la inicialización del SPIFFS"));
        while (true);
    }
    // Lee los estados de los Relays
    if(!settingsReadRelays()){
        //salvar estados de los relays
        settingsSaveRelays();
    }
    // Paso estados a los pines de los Relays
    setOnOffSingle(RELAY1,Relay01_status);
    setOnOffSingle(RELAY2,Relay02_status);
    // Lee la Configuración WiFi
    if(!settingsReadWiFi()){        
        // Salvar las configuraciones del WIFI
        settingsSaveWiFi();
    }
    // Incrementar el contador de reinicios
    bootCount++;
    // Salvar las configuraciones del WIFI + incremento del bootCount
    settingsSaveWiFi();
    // Configuracion WIFI
    WiFi.disconnect(true);
    delay(1000);
    // Setup del WiFI
    wifi_setup(); 
    // Lee la Configuración MQTT
    if(!settingsReadMQTT()){
        // Salvar la configuracion del MQTT
        settingsSaveMQTT();
    } 
    // leer www_username/password
    if(!settingsReadAdmin()){
        // Salvar el usuario y Contraseña
        settingsSaveAdmin();
    }   
    // Inicializar el WebSockets
    InitWebSockets();
    // Inicializar el Servidor
    InitServer();
    // Nos devuelve la lista de carpetas y archivos del SPIFFS ONLYDEBUG
    // listDir(SPIFFS, "/", 0); 
    log("Info: Setup completado");   

}


// -------------------------------------------------------------------
// Loop Pincipal Nucleo 0
// -------------------------------------------------------------------
void loop() {

    yield();
    // -------------------------------------------------------------------
    // WIFI
    // -------------------------------------------------------------------
    if (wifi_mode == WIFI_STA){
        wifiLoop();
    }else if (wifi_mode == WIFI_AP){
        wifiAPLoop();
    } 
    // -------------------------------------------------------------------
    // MQTT
    // -------------------------------------------------------------------
    if ((WiFi.status() == WL_CONNECTED) && (wifi_mode == WIFI_STA)){
        if(mqtt_server != 0){
            mqttLoop();
            if (mqttclient.connected()){
                if (millis() - lastMsg > mqtt_time){
                    lastMsg = millis();
                    mqtt_publish();
                }
            }      
        }
    }
    // -------------------------------------------------------------------
    // Enviar JSON por WS 
    // -------------------------------------------------------------------
    if (millis() - lastWsSend > 1000){
        lastWsSend = millis();
        WsMessage(GetJson(), "");
    }


}