/*
 * -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: info@electroniciot.com
 * -------------------------------------------------------------------
 */

#include "ESPAsyncWebServer.h"

// -------------------------------------------------------------------
// Instancia de AsyncWebServer puerto 80
// -------------------------------------------------------------------
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
AsyncEventSource events("/events");
// -------------------------------------------------------------------
// Declaracion de funciones
// -------------------------------------------------------------------
void ProcessRequest(AsyncWebSocketClient * client, String request);
void WsMessage(String icon, String texto );
// -------------------------------------------------------------------
// Variables generales WS
// -------------------------------------------------------------------
long lastWsSend = 0;           // Variable para tiempo de envio por WS

// -------------------------------------------------------------------
// Eventos del Websocket
// -------------------------------------------------------------------
void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){ 
	if(type == WS_EVT_CONNECT){
		Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
		client->printf("{\"ClientID\": %u }", client->id());
		client->ping();
	} else if(type == WS_EVT_DISCONNECT){
		//Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
	} else if(type == WS_EVT_ERROR){
		//Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
	} else if(type == WS_EVT_PONG){
		//Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
	} else if(type == WS_EVT_DATA){
		AwsFrameInfo * info = (AwsFrameInfo*)arg;
		String msg = "";
		if(info->final && info->index == 0 && info->len == len){
			if(info->opcode == WS_TEXT){
				for(size_t i=0; i < info->len; i++) {
					msg += (char) data[i];
				}
			} else {
				char buff[3];
				for(size_t i=0; i < info->len; i++) {
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}

			if(info->opcode == WS_TEXT)
			   ProcessRequest(client, msg);
			
		} else {
			//message is comprised of multiple frames or the frame is split into multiple packets
			if(info->opcode == WS_TEXT){
				for(size_t i=0; i < len; i++) {
					msg += (char) data[i];
				}
			} else {
				char buff[3];
				for(size_t i=0; i < len; i++) {
					sprintf(buff, "%02x ", (uint8_t) data[i]);
					msg += buff ;
				}
			}
			Serial.printf("%s\n",msg.c_str());

			if((info->index + len) == info->len){
				if(info->final){
					if(info->message_opcode == WS_TEXT)
					   ProcessRequest(client, msg);
				}
			}
		}
	}
}

// -------------------------------------------------------------------
// Inicializar el Websocket
// -------------------------------------------------------------------
void InitWebSockets(){
	ws.onEvent(onWsEvent);
	server.addHandler(&ws);
    log(F("Info: WebSocket server iniciado"));
}


// -------------------------------------------------------------------
// Manejador de ordenes enviada por Websocket
// -------------------------------------------------------------------
void ProcessRequest(AsyncWebSocketClient * client, String request){
    
	String command = request;
    
	if(command == "restore"){
		log("Info: Commando por WS => " + command);
		settingsResetWiFi();
		settingsResetMQTT();
		settingsResetAdmin();
		settingsResetRelays();
		if(settingsSaveWiFi() && settingsSaveMQTT() && settingsSaveAdmin() && settingsSaveRelays()){
			WsMessage("success", " ¡Equipo restaurado correctamente!" );
		}else{
			WsMessage("error", " ¡El equipo no se restauro!" );
		}
		delay(5000);
		ESP.restart();
	}else if (command == "restart"){
		log("Info: Commando por WS => " + command);
		WsMessage("success", " ¡Equipo reiniciado correctamente!" );
		delay(5000);		
		ESP.restart(); 
	}
	

	if(command != "restore"){
		OnOffRelays(command);
	}	
    
}
// -------------------------------------------------------------------
// Función enviar JSON por Websocket 
// -------------------------------------------------------------------
void WsMessage(String icon, String texto ){
  if(texto != ""){
    String response;
    StaticJsonDocument<300> doc;
	doc["type"] = "info";
    doc["icon"] = icon;
    doc["texto"] = texto;
    serializeJson(doc, response);
	// Enviar por WS
    ws.textAll(response);
  }else{
    ws.textAll(icon);
  }
}
// -------------------------------------------------------------------
// Empaquetar el JSON para enviar por WS
// -------------------------------------------------------------------
String GetJson(){
	String response;
	DynamicJsonDocument jsonDoc(3000);
	jsonDoc["type"] = "data";
	jsonDoc["wifi_status"] =  WiFi.status() == WL_CONNECTED ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-danger'>DESCONECTADO</span>");
	jsonDoc["wifi_dbm"] = WiFi.status() == WL_CONNECTED ? WiFi.RSSI() : 0;
	jsonDoc["wifi_percent"] = WiFi.status() == WL_CONNECTED ? getRSSIasQuality(WiFi.RSSI()) : 0; 
	jsonDoc["temp_cpu"] = String(round(TempCPUValue()),2);  
	jsonDoc["ram_available"] = ESP.getFreeHeap() * 100 / ESP.getHeapSize(); 
	jsonDoc["spiffs_used"] = SPIFFS.usedBytes() * 100 / SPIFFS.totalBytes();
	jsonDoc["mqtt_status"] = mqttclient.connected() ? F("<span class='label label-success'>CONECTADO</span>") : F("<span class='label label-danger'>DESCONECTADO</span>");
	jsonDoc["mqtt_on"] = mqttclient.connected() ? F("<span class='label btn-metis-2'>Online</span>") : F("<span class='label label-danger'>Offline</span>");
	jsonDoc["mqtt_server"] = mqttclient.connected() ? F(mqtt_server) : F("server not connected");
	jsonDoc["active"] = longTimeStr(millis() / 1000);
	jsonDoc["relay1_status"] = Relay01_status ? true : false;
	jsonDoc["relay2_status"] = Relay02_status ? true : false;
	serializeJson(jsonDoc, response);
	return response;
}

