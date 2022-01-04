/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/

#include "LedBlink.hpp"

// -------------------------------------------------------------------
// Sensor Temp Interno CPU
// -------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
{
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif
uint8_t temprature_sens_read();

// -------------------------------------------------------------------
// Genera un log en el puerto Serial
// -------------------------------------------------------------------
void log(String s){
    Serial.println(s);
}
// -------------------------------------------------------------------
// Definir la Plataforma
// -------------------------------------------------------------------
String platform(){
// Optiene la plataforma de hardware
#ifdef ARDUINO_ESP32_DEV
    return "ESP32";
#endif
}
// -------------------------------------------------------------------
// Convierte un char a IP
// -------------------------------------------------------------------
IPAddress CharToIP(const char *str){
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}
// -------------------------------------------------------------------
// Retorna segundos como "d:hh:mm:ss"
// -------------------------------------------------------------------
String longTimeStr(const time_t &t){        
    String s = String(t / SECS_PER_DAY) + ':';
    if (hour(t) < 10)
    {
        s += '0';
    }
    s += String(hour(t)) + ':';
    if (minute(t) < 10)
    {
        s += '0';
    }
    s += String(minute(t)) + ':';
    if (second(t) < 10)
    {
        s += '0';
    }
    s += String(second(t));
    return s;
}
// -------------------------------------------------------------------
// Retorna IPAddress en formato "n.n.n.n" de IP a String
// -------------------------------------------------------------------
String ipStr(const IPAddress &ip){    
    String sFn = "";
    for (byte bFn = 0; bFn < 3; bFn++)
    {
        sFn += String((ip >> (8 * bFn)) & 0xFF) + ".";
    }
    sFn += String(((ip >> 8 * 3)) & 0xFF);
    return sFn;
}
// -------------------------------------------------------------------
// De HEX a String
// -------------------------------------------------------------------
String hexStr(const unsigned long &h, const byte &l = 8){
    String s;
    s = String(h, HEX);
    s.toUpperCase();
    s = ("00000000" + s).substring(s.length() + 8 - l);
    return s;
}
// -------------------------------------------------------------------
// Crear un ID unico desde la direccion MAC
// -------------------------------------------------------------------
String idUnique(){
    // Retorna los ultimos 4 Bytes del MAC rotados
    char idunique[15]; 
    uint64_t chipid = ESP.getEfuseMac();           
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(idunique, 15, "%04X", chip);
    return idunique;
}
// -------------------------------------------------------------------
// ID del Dispositivo para La Base de Datos
// -------------------------------------------------------------------
const String device_id = hexStr(ESP.getEfuseMac()) + "CE" + String(idUnique()); 
// -------------------------------------------------------------------
// ID del Dispositivo para La Base de Datos
// -------------------------------------------------------------------
String deviceID(){
    return String(platform()) + hexStr(ESP.getEfuseMac()) + String(idUnique());
}
// -------------------------------------------------------------------
// Configurar los Pines de Salida
// -------------------------------------------------------------------
void settingPines(){
    // Pines de Salida
    pinMode(WIFILED, OUTPUT);
    pinMode(MQTTLED, OUTPUT);
    pinMode(RELAY1, OUTPUT);  
    pinMode(RELAY2, OUTPUT);  
    // Los iniciamos en nivel bajo
    setOffSingle(RELAY1);
    setOffSingle(RELAY2); 
    setOffSingle(WIFILED); 
    setOffSingle(MQTTLED);             
}
// -------------------------------------------------------------------
// Parpadeo LED MQTT Transmisión
// -------------------------------------------------------------------
void mqttTX(){ 
    for (int i = 0; i < 2; i++){
        setOnSingle(MQTTLED);
        delay(50);
        setOffSingle(MQTTLED);
        delay(10);
    }  
}
// -------------------------------------------------------------------
// Parpadeo LED MQTT Recepción
// -------------------------------------------------------------------
void mqttRX(){
    for (int i = 0; i < 1; i++){
        blinkRandomSingle(5,50,MQTTLED);
        delay(5);
    }
}
// -------------------------------------------------------------------
// Parpadeo de los LEDs WIFI & MQTT
// -------------------------------------------------------------------
void leds(){
    for(int i=0; i < 4; i++) { 
        setOnSingle(MQTTLED);
        setOnSingle(WIFILED);
        delay(100);
        setOffSingle(MQTTLED);
        setOffSingle(WIFILED);
        delay(100);
    }
}
// -------------------------------------------------------------------
// Retorna la calidad de señal WIFI en %
// -------------------------------------------------------------------
int getRSSIasQuality(int RSSI){
    int quality = 0;
    if(RSSI <= -100){
        quality = 0;
    } else if(RSSI >= -50){
        quality = 100;
    } else{
       quality = 2 * (RSSI + 100); 
    }
    return quality;
}
// -------------------------------------------------------------------
// Retorna la temperatura del CPU
// -------------------------------------------------------------------
float TempCPUValue (){
    return temp_cpu = (temprature_sens_read() - 32) / 1.8;
}
// -------------------------------------------------------------------
// Retorna el listado de todos los archivos en el SPIFFS /
// -------------------------------------------------------------------
void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        log("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        log(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            log("  DIR : " + String(file.name()));
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            log("  FILE : " + String(file.name()));
            log("\tSIZE : " + String(file.size()));
        }
        file = root.openNextFile();
    }
    log("  Total space: " + String(SPIFFS.totalBytes()));
    log("  Total space used:  " + String(SPIFFS.usedBytes()));
}
// -------------------------------------------------------------------
// Convierte un char a IP
// -------------------------------------------------------------------
IPAddress StrIP(const char *str){
    sscanf(str, "%hhu.%hhu.%hhu.%hhu", &ip[0], &ip[1], &ip[2], &ip[3]);
    return IPAddress(ip[0], ip[1], ip[2], ip[3]);
}
// -------------------------------------------------------------------
// Nos Devuelve un String con el SweetAlert2
// -------------------------------------------------------------------
String SweetAlert(String TitleWeb, String SweetTitle, String SweetText, String SweetIcon, String type){
    String SweetAlert;
    if(type == "aviso"){
        SweetAlert = "<head>"
                        "<html><meta charset='UTF-8'>"
                        "<title>AdminTools | "+ TitleWeb +"</title>"
                        "<meta content='width=device-width, initial-scale=1.0' name='viewport' />"
                        "<link rel='icon' href='www/esp32.png' type='image/x-icon'>"
                        "<link rel='stylesheet' href='www/bootstrap.css' />"
                        "<link rel='stylesheet' href='www/main.css' />"
                        "<link rel='stylesheet' href='www/MoneAdmin.css' />"
                        "<link rel='stylesheet' href='www/font-awesome.css' />"
                        "<link rel='stylesheet' href='www/sweetalert2.min.css' />"
                        "<script src='www/jquery-2.0.3.min.js'></script>"
                        "<script src='www/bootstrap.min.js'></script>"
                        "<script src='www/modernizr-2.6.2.min.js'></script>"
                        "<script src='www/sweetalert2.min.js'></script>"
                    "</head>"
                    "<body>"
                    "<script>"
                        "Swal.fire({title: '"+SweetTitle+"!',"
                                        " text: '"+SweetText+"',"
                                        " icon: '"+SweetIcon+"',"
                                        " confirmButtonText: 'Cerrar'}).then((result) => {"
                                        "if (result.isConfirmed){"
                                            "history.back();"
                                        "};"
                                    "})"
                    "</script>"
                    "<body>"
                    "</html>";
    }else if(type == "accion"){
        SweetAlert = "<head>"
                        "<html><meta charset='UTF-8'>"
                        "<title>AdminTools | "+ TitleWeb +"</title>"
                        "<meta content='width=device-width, initial-scale=1.0' name='viewport' />"
                        "<link rel='icon' href='www/esp32.png' type='image/x-icon'>"
                        "<link rel='stylesheet' href='www/bootstrap.css' />"
                        "<link rel='stylesheet' href='www/main.css' />"
                        "<link rel='stylesheet' href='www/MoneAdmin.css' />"
                        "<link rel='stylesheet' href='www/font-awesome.css' />"
                        "<link rel='stylesheet' href='www/sweetalert2.min.css' />"
                        "<script src='www/jquery-2.0.3.min.js'></script>"
                        "<script src='www/bootstrap.min.js'></script>"
                        "<script src='www/modernizr-2.6.2.min.js'></script>"
                        "<script src='www/sweetalert2.min.js'></script>"
                    "</head>"
                    "<body>"
                    "<script>"
                        "Swal.fire({title: '"+SweetTitle+"!',"
                        " text: '"+SweetText+"',"
                        " icon: '"+SweetIcon+"',"
                        " showCancelButton: true,"
                        " confirmButtonColor: '#3085d6',"
                        " cancelButtonColor: '#d33',"
                        " confirmButtonText: 'Si, reiniciar',"
                        " cancelButtonText: 'Cancelar',"
                        " reverseButtons: true"
                        " }).then((result) => {"
                                    "if (result.isConfirmed){"
                                        "window.location = 'esp-restart';"
                                        "}else if ("
                                        "result.dismiss === Swal.DismissReason.cancel"
                                        "){"
                                        "history.back();"
                                        "}"
                                    "})"
                    "</script>"
                    "<body>"
                    "</html>";
    }else{
        SweetAlert = "<head>"
                        "<html><meta charset='UTF-8'>"
                        "<title>AdminTools | "+ TitleWeb +"</title>"
                        "<meta content='width=device-width, initial-scale=1.0' name='viewport' />"
                        "<link rel='icon' href='www/esp32.png' type='image/x-icon'>"
                        "<link rel='stylesheet' href='www/bootstrap.css' />"
                        "<link rel='stylesheet' href='www/main.css' />"
                        "<link rel='stylesheet' href='www/MoneAdmin.css' />"
                        "<link rel='stylesheet' href='www/font-awesome.css' />"
                        "<link rel='stylesheet' href='www/sweetalert2.min.css' />"
                        "<script src='www/jquery-2.0.3.min.js'></script>"
                        "<script src='www/bootstrap.min.js'></script>"
                        "<script src='www/modernizr-2.6.2.min.js'></script>"
                        "<script src='www/sweetalert2.min.js'></script>"
                    "</head>"
                    "<body>"
                    "<script>"
                        "Swal.fire({title: '"+SweetTitle+"!',"
                                        " text: '"+SweetText+"',"
                                        " icon: '"+SweetIcon+"',"
                                        " confirmButtonText: 'Cerrar'}).then((result) => {"
                                        "if (result.isConfirmed){"
                                            "window.location = '/';"
                                        "};"
                                    "})"
                    "</script>"
                    "<body>"
                    "</html>";
    }
    return SweetAlert;
}

// -------------------------------------------------------------------
// Control de los Relay desde MQTT & WS
// -------------------------------------------------------------------

boolean settingsSaveRelays();

void OnOffRelays(String command){

    DynamicJsonDocument JsonDoc(1024);

    deserializeJson(JsonDoc, command);
    
    if(JsonDoc["protocol"] == "WS"){
        log("Info: Commando por WS => " + command);
    }else{
        log("Info: Commando por MQTT => " + command);
    }
    	
    serializeJsonPretty(JsonDoc, Serial);

    if (JsonDoc["value"]){
        digitalWrite(JsonDoc["output"] == "RELAY1" ? RELAY1 : RELAY2, HIGH);
        JsonDoc["output"] == "RELAY1" ? Relay01_status = HIGH : Relay02_status = HIGH ;
    }else {
        digitalWrite(JsonDoc["output"] == "RELAY1" ? RELAY1 : RELAY2, LOW);
        JsonDoc["output"] == "RELAY1" ? Relay01_status = LOW : Relay02_status = LOW ;
    }

    settingsSaveRelays();    

}