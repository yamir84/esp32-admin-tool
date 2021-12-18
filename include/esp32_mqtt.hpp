/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/

#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient mqttclient(espClient);

char topico[150];

String mqtt_data = "";
long lastMqttReconnectAttempt = 0;
long lastMsg = 0;

void callback(char *topic, byte *payload, unsigned int length);
String Json();

// -------------------------------------------------------------------
// MQTT Connect
// -------------------------------------------------------------------
boolean mqtt_connect(){  
  
  mqttclient.setServer(mqtt_server, mqtt_port);
  mqttclient.setCallback(callback);
  log("Info: Intentando conexión al Broker MQTT...");

  // Conexion al Servidor MQTT , ClienteID, Usuario, Password.
  // Ver documentación => https://pubsubclient.knolleary.net/api.html  

  if (mqttclient.connect(mqtt_id, mqtt_user, mqtt_passw)) {  // Attempt to connect
    log("Info: Conectado al Broker MQTT");
    // Nos suscribimos a comandos Topico: usuario/dispositivo/comando
    String topico_subscribe = String(mqtt_user)+"/"+mqtt_id+"/command";
    topico_subscribe.toCharArray(topico, 150);
    mqttclient.subscribe(topico); 

    topico_subscribe.toCharArray(topico, 25);
    mqttclient.publish(topico, "ok"); //Publicar respuesta ok por MQTT

  } else {
    log("Error: failed, rc= " + mqttclient.state());
    return(0);
  }

  return (1);

}
// -------------------------------------------------------------------
// Manejo de los Mensajes Entrantes
// -------------------------------------------------------------------
void callback(char *topic, byte *payload, unsigned int length){

  String mensaje = "";
  String str_topic(topic);

  for(uint16_t i = 0; i < length; i++){
    mensaje += (char)payload[i];
    mqttRX();
  }

  mensaje.trim();

  DynamicJsonDocument jsonDoc(300);

  deserializeJson(jsonDoc, mensaje);

  if(jsonDoc["RELAY1"] == "on"){
    setOnSingle(RELAY1);
    Relay01_status = HIGH;
    settingsSaveRelays();
  }else if (jsonDoc["RELAY1"] == "off"){
    setOffSingle(RELAY1);
    Relay01_status = LOW;
    settingsSaveRelays();
  }else if(jsonDoc["RELAY2"] == "on"){
    setOnSingle(RELAY2);
    Relay02_status = HIGH;
    settingsSaveRelays();
  }else if (jsonDoc["RELAY2"] == "off"){
    setOffSingle(RELAY2);
    Relay02_status = LOW;
    settingsSaveRelays();
  }

  log("Info: Topico -->" + str_topic);
  log("Info: Mensaje -->" + mensaje); 

  serializeJsonPretty(jsonDoc, Serial);

}
// -------------------------------------------------------------------
// Manejo de los Mensajes Salientes
// ------------------------------------------------------------------- 
void mqtt_publish() {
  
  String topic = String(mqtt_user)+"/"+mqtt_id+"/values";
  mqtt_data = Json();
  mqttclient.publish(topic.c_str(), mqtt_data.c_str());
  mqtt_data="";

  mqttTX();

}
// -------------------------------------------------------------------
// JSON con informacion de envio por MQTT
// ------------------------------------------------------------------- 
String Json(){

   String response;
   DynamicJsonDocument jsonDoc(3000);

   jsonDoc["wifi_dbm"] = WiFi.status() == WL_CONNECTED ? String(WiFi.RSSI()) : F("0");
   jsonDoc["wifi_percent"] = WiFi.status() == WL_CONNECTED ? String(getRSSIasQuality(WiFi.RSSI())) : F("0"); 
   jsonDoc["temp_cpu"] = String(TempCPUValue()); 
   jsonDoc["ram_available"] = String(ESP.getFreeHeap() * 100 / ESP.getHeapSize()); 
   jsonDoc["flash_used"] = String(round(SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes()), 0);
   jsonDoc["relay1_status"] = String(Relay01_status ? "true" : "false");
   jsonDoc["relay2_status"] = String(Relay02_status ? "true" : "false");

   serializeJson(jsonDoc, response);   
   return response;

}
// -------------------------------------------------------------------
// MQTT Loop Principal
// -------------------------------------------------------------------
void mqttLoop(){
  if(mqtt_enable){
      
      if (!mqttclient.connected()) {
        long now = millis();
        // try and reconnect continuously for first 60s then try again once every 120s
        if ( (now < 60000) || ((now - lastMqttReconnectAttempt)  > 120000) ) {
          lastMqttReconnectAttempt = now;
          if (mqtt_connect()) { // Attempt to reconnect
            lastMqttReconnectAttempt = 0;
          }                   
          // Poner en ON el Led del MQTT
          setOnSingle(MQTTLED);          
        }
      } else {
        // if MQTT connected
        mqttclient.loop();
        // Poner en OFF el Led del MQTT
        setOffSingle(MQTTLED);
      }
         
    }
}


