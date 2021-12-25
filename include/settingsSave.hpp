/* -------------------------------------------------------------------
 * AdminESP - ElectronicIOT 2021
 * Sitio WEB: https://electroniciot.com
 * Correo: admim@electroniciot.com
 * Plataforma ESP32
 * Proyecto Admin Panel Tool para el ESP32
 * -------------------------------------------------------------------
*/


// -------------------------------------------------------------------
// Guardar los estados de los Relays
// -------------------------------------------------------------------
boolean settingsSaveRelays(){
    // Guarda configuración de los Relays
    StaticJsonDocument<capacityrelays> jsonConfig;
    File file = SPIFFS.open(F("/settingrelays.json"), "w+");
    if (file){
        jsonConfig["Relay01_status"] = Relay01_status;
        jsonConfig["Relay02_status"] = Relay02_status;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Estado de los Relay guardado"));
        //serializeJsonPretty(jsonConfig, Serial);
        return true;
    }else{
        log(F("Error: Falló el guardado del estado de los Relay"));
        return false;
    }
}
// -------------------------------------------------------------------
// Guardar las configuraciones del WIFI y Generales
// -------------------------------------------------------------------
boolean settingsSaveWiFi(){
    // Graba configuración del WiFi y General
    StaticJsonDocument<capacitywifi> jsonConfig;
    File file = SPIFFS.open("/settingwifi.json", "w+");
    if (file){
        /* ------------------- GENERAL -------------------- */
        jsonConfig["id"] = id;
        jsonConfig["boot"] = bootCount;
        /* ------------------- CLIENTE -------------------- */
        jsonConfig["wifi_staticIP"] = wifi_staticIP;
        jsonConfig["wifi_ssid"] = wifi_ssid;
        jsonConfig["wifi_passw"] = wifi_passw;
        jsonConfig["wifi_ip_static"] = wifi_ip_static;
        jsonConfig["wifi_gateway"] = wifi_gateway;
        jsonConfig["wifi_subnet"] = wifi_subnet;
        jsonConfig["wifi_primaryDNS"] = wifi_primaryDNS;
        jsonConfig["wifi_secondaryDNS"] = wifi_secondaryDNS;
        /* ------------------- AP ------------------------- */
        jsonConfig["ap_accessPoint"] = ap_accessPoint;
        jsonConfig["ap_nameap"] = ap_nameap;
        jsonConfig["ap_passwordap"] = ap_passwordap;
        jsonConfig["ap_canalap"] = ap_canalap;
        jsonConfig["ap_hiddenap"] = ap_hiddenap;
        jsonConfig["ap_connetap"] = ap_connetap;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log("Info: Configuración WiFi Guardada");
        //serializeJsonPretty(jsonConfig, Serial);   
        return true;
    }else{
        log("Error: Falló el guardado de la configuración WiFI");
        return false;
    }    
}
// -------------------------------------------------------------------
// Guardar las configuraciones www_username/password
// -------------------------------------------------------------------
boolean settingsSaveAdmin(){
    // Guarda configuración Usuario y Contraseña
    StaticJsonDocument<capacityadmin> jsonConfig;
    File file = SPIFFS.open(F("/settingadmin.json"), "w+");
    if (file){
        jsonConfig["www_username"] = www_username;
        jsonConfig["www_password"] = www_password;
        serializeJsonPretty(jsonConfig, file);
        file.close();
        log(F("Info: Usuario y Contraseña guardados"));
        //serializeJsonPretty(jsonConfig, Serial);
        return true;
    }else{
        log(F("Error: Falló el guardado del Usuario y Contraseña"));
        return false;
    }
}
// -------------------------------------------------------------------
// Guardar las configuraciones del Broker MQTT
// -------------------------------------------------------------------
boolean settingsSaveMQTT(){
  // Guarda configuración MQTT
  StaticJsonDocument<capacitymqtt> jsonConfig;
  File file = SPIFFS.open(F("/settingmqtt.json"), "w+");
  if (file){
    jsonConfig["mqtt_user"] = mqtt_user;
    jsonConfig["mqtt_passw"] = mqtt_passw;
    jsonConfig["mqtt_server"] = mqtt_server;
    jsonConfig["mqtt_id"] = mqtt_id;
    jsonConfig["mqtt_time"] = mqtt_time;
    jsonConfig["mqtt_port"] = mqtt_port;
    jsonConfig["mqtt_enable"] = mqtt_enable;
    serializeJsonPretty(jsonConfig, file);
    file.close();
    log(F("Info: Configuración MQTT guardada"));
    //serializeJsonPretty(jsonConfig, Serial);
    return true;
  }else{
    log(F("Error: Falló el guardado de la configuración del MQTT"));
    return false;
  }    
}

