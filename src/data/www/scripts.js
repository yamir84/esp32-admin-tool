/* capturar el url del navegador  http://server[/pathname] */
let pathname = window.location.pathname;
/* WebSocket */
//var connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);
const connection = new WebSocket('ws://192.168.0.3/ws', ['arduino']);
/* Conexion Abierta */
connection.onopen = function() {
    console.log('Conexión al WebSocket abierta');
};
/* Conexión Error */
connection.onerror = function(error) {
    console.log('Error en el WebSocket', error);
};
/* Manejo de mensajes entrantes por WS */
connection.onmessage = function(e) {
    /* Precesar mensajes*/
    processData(e.data);
};
/* Conexión Cerrada */
connection.onclose = function() {
    console.log('Conexión al WebSocket cerrada');
};
/* */
function processData(data) {
    let json = JSON.parse(data);
    //console.log(json);
    //Mensajes de retorno en un SweetAlert si retorna un status válido
    if (json.type == "info") {
        console.log(json);
        Swal.fire({
            position: 'top-end',
            icon: json.icon,
            title: json.texto,
            showConfirmButton: false,
            timer: 2000
        });
    }
    if (pathname == "/") {
        console.log(json);
        if (json.type == "data") {
            document.getElementById('wifi_status').innerHTML = json.wifi_status; // Estado del WIFI
            document.getElementById('mqtt_status').innerHTML = json.mqtt_status; // Estado del MQTT
            document.getElementById('mqtt_on').innerHTML = json.mqtt_on; // Estado del MQTT
            document.getElementById('mqtt_server').innerHTML = json.mqtt_server; // Broker MQTT
            document.getElementById('wifi_dbm').innerHTML = json.wifi_dbm + " dBm"; // dbm Señal WIFI
            document.getElementById('dbm_label').innerHTML = json.wifi_dbm + " dBm"; // dbm Señal WIFI
            document.getElementById('wifi_percent').innerHTML = json.wifi_percent + " %"; // % de la señal WIFI
            // preogressbar
            document.getElementById('wifi_Signal').style.width = json.wifi_percent + "%";
            document.getElementById('wifi_label').innerHTML = json.wifi_percent + "%";
            document.getElementById('spiffs_Signal').style.width = json.spiffs_used + "%";
            document.getElementById('spiffs_label').innerHTML = json.spiffs_used + "%";
            document.getElementById('ram_Signal').style.width = json.ram_available + "%";
            document.getElementById('ram_label').innerHTML = json.ram_available + "%";

            document.getElementById('temp_cpu').innerHTML = json.temp_cpu + " °C"; // Muestra Temperatura del CPU
            document.getElementById('active').innerHTML = json.active; // Tiempo Activo del Dispositivo
            document.getElementById('time_active').innerHTML = json.active; // Tiempo Activo del Dispositivo

            // Capturar los estados de los relays en tiempo real
            let RELAY1_Status = document.getElementById('RELAY1_Status');
            let RELAY2_Status = document.getElementById('RELAY2_Status');
            let divRELAY1 = document.getElementById('RELAY1').parentNode;
            let divRELAY2 = document.getElementById('RELAY2').parentNode;

            if (json.relay1_status) {
                divRELAY1.classList.remove('switch-off');
                divRELAY1.classList.add('switch-on');
                RELAY1_Status.innerText = 'Activo';
                RELAY1_Status.classList.remove('label-info');
                RELAY1_Status.classList.add('label-primary');
            } else {
                divRELAY1.classList.remove('switch-on');
                divRELAY1.classList.add('switch-off');
                RELAY1_Status.innerText = 'Apagado';
                RELAY1_Status.classList.remove('label-primary');
                RELAY1_Status.classList.add('label-info');
            }
            if (json.relay2_status) {
                divRELAY2.classList.remove('switch-off');
                divRELAY2.classList.add('switch-on');
                RELAY2_Status.innerText = 'Activo';
                RELAY2_Status.classList.remove('label-info');
                RELAY2_Status.classList.add('label-primary');
            } else {
                divRELAY2.classList.remove('switch-on');
                divRELAY2.classList.add('switch-off');
                RELAY2_Status.innerText = 'Apagado';
                RELAY2_Status.classList.remove('label-primary');
                RELAY2_Status.classList.add('label-info');
            }

        }
    }

}
/* habilitar o deshabilitar los input a la hora de la carga del html */
function mqtt() {
    let x = document.getElementById("mqtten");
    if (x.value == "1") {
        formDisableFalse("mqtt");
    } else {
        formDisableTrue("mqtt");
    }
}
/* Habilitar input WIFI/AP segun estado */
function wifi() {
    let ip = document.getElementById("wifi_staticIPen");
    let ap = document.getElementById("ap_AP_en");
    if (ip.value == "1") {
        formDisableFalse("ip");
    } else {
        formDisableTrue("ip");
    }
    if (ap.value == "1") {
        formDisableFalse("ap");
    } else {
        formDisableTrue("ap");
    }
}
/* ------------------- Habilitar MQTT -------------------- */
function mqttenable() {
    if (document.getElementById("mqtt_enable").checked) {
        document.getElementById("mqtten").value = 1;
        formDisableFalse("mqtt");
    } else {
        document.getElementById("mqtten").value = 0;
        formDisableTrue("mqtt");
    }
}
/* ----------- Habilitar IP Estática ------------------ */
function staticIPen() {
    if (document.getElementById("wifi_staticIP").checked) {
        document.getElementById('wifi_staticIPen').value = 1;
        formDisableFalse("ip");
    } else {
        document.getElementById('wifi_staticIPen').value = 0;
        formDisableTrue("ip");
    }
}
/* ------------------- Habilitar AP -------------------- */
function apenable() {
    if (document.getElementById("ap_accessPoint").checked) {
        document.getElementById('ap_AP_en').value = 1;
        formDisableFalse("ap");
    } else {
        document.getElementById('ap_AP_en').value = 0;
        formDisableTrue("ap");
    }
}
/* -------- Habilitar AP Visible (0) No (1) ------------ */
function visibilityen() {
    if (document.getElementById("ap_hiddenap").checked) {
        document.getElementById('ap_visibility').value = 0;
    } else {
        document.getElementById('ap_visibility').value = 1;
    }
}
/* Quitar el disabled de los input */
function formDisableFalse(clase) {
    let formElement = document.getElementsByClassName(clase);
    for (let i = 0; i < formElement.length; i++)
        formElement[i].disabled = false;
}
/* Habilitar el disabled de los input */
function formDisableTrue(clase) {
    let formElement = document.getElementsByClassName(clase);
    for (let i = 0; i < formElement.length; i++)
        formElement[i].disabled = true;
}
/* Limpiar Formulario */
function clearAdmin() {
    document.getElementById("formulario").reset();
}
/* funcion para enviar ON/OFF por WS */
let relay = (e) => {
        //console.log(e.id);
        let miCheckbox = document.getElementById(e.id);
        let msg = document.getElementById(e.id + "_Status");
        if (miCheckbox.checked) {
            msg.innerText = "Activo";
            msg.classList.remove("label-info");
            msg.classList.add("label-primary");
            OnOffRelay(e.id, true);
        } else {
            msg.innerText = "Apagado";
            msg.classList.remove("label-primary");
            msg.classList.add("label-info");
            OnOffRelay(e.id, false);
        }
    }
    /* funcion para enviar JSON ON/OFF por WS */
let OnOffRelay = (relay, command) => {
        // Ejemplo: {"protocol": "WS", "output": "RELAY1", "value": true }
        connection.send(`{"protocol": "WS", "output": "${relay}", "value": ${command} }`);
    }
    /* ------------ Función para restaurar el dispositivo ------ */
function restaurar() {
    Swal.fire({
        title: 'Restablecer!',
        text: ' Toda configuración que haya guardado se perdera si restablece a los valores de fábrica. ¿Está seguro de continuar?',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Si, restablecer',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            RestoreRestart("restore");
        } else if (
            result.dismiss === Swal.DismissReason.cancel
        ) {
            history.back();
        }
    })
}
/* ------------ Función para Reiniciar el Dispositivo ------ */
function reiniciar() {
    Swal.fire({
        title: 'Reiniciar!',
        text: ' ¿Está seguro de reiniciar el Dispositivo?',
        icon: 'warning',
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Si, reiniciar',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            RestoreRestart("restart");
        } else if (
            result.dismiss === Swal.DismissReason.cancel
        ) {
            history.back();
        }
    })
}
/* conteo de reinicio 10s */
let t = 10;
/* Ocultar Visualizador de Progress Bar */
function visu() {
    var x = document.getElementById("visu");
    x.style.display = "none";
}
/* reinicio & restaurar */
function RestoreRestart(type) {
    console.log(type);
    t--;
    document.getElementById('visu').style.display = 'block';
    document.getElementById("time").innerHTML = " " + t;
    document.getElementById('progress').style.width = t * 10 + '%';
    document.getElementById("pregressvalue").innerHTML = " " + t * 10 + '%';
    if (type == "restart") {
        if (t == 0) {
            document.getElementById('visu').style.display = 'none';
            document.getElementById('btn').disabled = false;
            window.location = "/";
            t = 10;
        } else if (t == 9) {
            connection.send("restart");
            document.getElementById('btn').disabled = true;
            window.setTimeout("RestoreRestart('restart')", 1000);
        } else {
            document.getElementById('btn').disabled = true;
            window.setTimeout("RestoreRestart('restart')", 1000);
        }
    } else {
        if (t == 0) {
            document.getElementById('visu').style.display = 'none';
            document.getElementById('btn').disabled = false;
            window.location = "/";
            t = 10;
        } else if (t == 9) {
            connection.send("restore");
            document.getElementById('btn').disabled = true;
            window.setTimeout("RestoreRestart('restore')", 1000);
        } else {
            document.getElementById('btn').disabled = true;
            window.setTimeout("RestoreRestart('restore')", 1000);
        }
    }
}