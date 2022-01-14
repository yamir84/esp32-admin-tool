/* capturar el url del navegador  http://server[/pathname] */
const pathname = window.location.pathname;
/* WebSocket */
const connection = new WebSocket('ws://' + location.hostname + '/ws', ['arduino']);
//const connection = new WebSocket('ws://192.168.0.3/ws', ['arduino']);
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
            // progressbar
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
    let x = document.getElementById("visu");
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

// Validacion de formulario
const formulario = document.getElementById('form');
const inputs = document.querySelectorAll('#form input');
// Objeto de las expresiones regulares
const expresiones = {
        TextNumber: /^[a-zA-Z0-9]{4,30}$/, // Letras a-z A-Z minúsculas - mayúsculas, números caracteres de 4 mínimo a 30 dígitos máximo 
        TextUnderscore: /^[a-zA-Z\_]{4,30}$/, // Letras minúsculas - mayusculas, guión bajo de 4 mínimo a 30 digitos máximo.
        TextNumberPassw: /^[a-zA-Z0-9\_\-\*]{4,30}$/, // Letras minúsculas - mayúsculas, números, guiones y asterisco caracteres de 4 mínimo a 30 dígitos máximo. 
        Dominio: /^([a-zA-Z]{2,6}|[a-zA-Z0-9-]{2,30}\.[a-zA-Z]{2,3})$/, // Ejemplo: ( tudominio.com ) sin los http:// o https://
        ServerPort: /^(6553[0-5]|655[0-2][0-9]|65[0-4][0-9]{2}|6[0-4][0-9]{3}|[0-5]?([0-9]){0,3}[0-9])$/, // Puertos desde 0 a 65535.
        TextNumberWIFI: /^[a-zA-Z0-9 \_\-\.\'\#]{4,30}$/, // Letras minúsculas - mayúsculas, números, guiones, comilla simple, numeral, espacio y punto caracteres de 4 mínimo a 30 dígitos máximo.
        //TextNumberWIFI: /^.{1,30}$/,
        IPv4: /^((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$/, // IPv4 255.255.255.255
        TextNumberAP: /^[a-zA-Z0-9 \_\-\.\'\#]{4,31}$/, // Letras minúsculas - mayúsculas, números, guiones, comilla simple, numeral y punto caracteres de 4 mínimo a 31 dígitos máximo.
        TextNumberPasswAP: /^[a-zA-Z0-9\_\-\*]{4,63}$/, // Letras minúsculas - mayúsculas, números, guiones y asterisco caracteres de 4 mínimo a 63 dígitos máximo. 
        www_username: /^[a-z]{4,15}$/, // Letras minúsculas caracteres de 4-15 digitos maximo.
        www_password: /^[a-zA-Z0-9]{4,15}$/, // Letras minúsculas - mayusculas, numeros, caracteres de 4-15 digitos maximo.
        id: /^[a-z0-9]{4,30}$/, // Letras minúsculas y numeros caracteres de 4-30 digitos maximo.
    }
    // Objeto de los campos a validar por formulario
const campos = {
        mqtt_id: true,
        mqtt_user: true,
        mqtt_passw: true,
        mqtt_server: true,
        mqtt_port: true,
        mqtt_time: true,
        // Wifi 
        wifi_ssid: true,
        wifi_passw: true,
        wifi_ip_static: true,
        wifi_subnet: true,
        wifi_gateway: true,
        wifi_primaryDNS: true,
        wifi_secondaryDNS: true,
        ap_nameap: true,
        ap_passwordap: true,
        ap_canalap: true,
        ap_connetap: true,
        // admin
        www_username: true,
        www_password: true,
        new_www_username: true,
        new_www_password: true,
        // device
        id: true
    }
    // Dejer de teclear y la perdida del foco del input
inputs.forEach((input) => {
    input.addEventListener('keyup', validarFormulario);
    input.addEventListener('blur', validarFormulario);
});
// Función para validar el Formulario.
function validarFormulario(e) {
    //console.log(e.target.dataset.expresion);
    switch (e.target.name) {
        case "mqtt_id":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_user":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_passw":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_server":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_port":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "mqtt_time":
            validarRango(e.target, e.target.name, 1, 60);
            break;
            // Sección WIFI
        case "wifi_ssid":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_passw":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_ip_static":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_subnet":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_gateway":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_primaryDNS":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "wifi_secondaryDNS":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
            //AP
        case "ap_nameap":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "ap_passwordap":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "ap_canalap":
            validarRango(e.target, e.target.name, 1, 13);
            break;
        case "ap_connetap":
            validarRango(e.target, e.target.name, 0, 8);
            break;
            /* ADMIN FORM */
        case "www_username":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "www_password":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "new_www_username":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
        case "new_www_password":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            validarPassword();
            break;
        case "c_new_www_password":
            validarPassword();
            break;
        case "id":
            validarCampo(expresiones[e.target.dataset.expresion], e.target, e.target.name);
            break;
    }
}
// Validar Campo
const validarCampo = (expresion, input, campo) => {
        //console.log(campo);
        if (expresion.test(input.value) && input.value != '') {
            document.querySelector(`#form_${campo}`).classList.remove('has-error');
            document.querySelector(`#form_${campo}`).classList.add('has-success');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.remove('formulario_input-error-activo');
            campos[campo] = true;
        } else {
            document.querySelector(`#form_${campo}`).classList.remove('has-success');
            document.querySelector(`#form_${campo}`).classList.add('has-error');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.add('formulario_input-error-activo');
            campos[campo] = false;
        }
    }
    // Validar un Rango de numeros min <-> max
const validarRango = (input, campo, min, max) => {
        if (input.value >= min && input.value <= max) {
            document.querySelector(`#form_${campo}`).classList.add('has-success');
            document.querySelector(`#form_${campo}`).classList.remove('has-error');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.remove('formulario_input-error-activo');
            campos[campo] = true;
        } else {
            document.querySelector(`#form_${campo}`).classList.add('has-error');
            document.querySelector(`#form_${campo}`).classList.remove('has-success');
            document.querySelector(`#form_${campo} .formulario_input-error`).classList.add('formulario_input-error-activo');
            campos[campo] = false;
        }
    }
    // Validar new password y confirm new password iguales 
const validarPassword = () => {
        const inputNewPassword = document.getElementById('new_www_password');
        const inputNewPasswordConfirm = document.getElementById('c_new_www_password');
        if (inputNewPassword.value == inputNewPasswordConfirm.value) {
            document.querySelector(`#form_c_new_www_password`).classList.add('has-success');
            document.querySelector(`#form_c_new_www_password`).classList.remove('has-error');
            document.querySelector(`#form_c_new_www_password .formulario_input-error`).classList.remove('formulario_input-error-activo');
            campos['new_www_password'] = true;
        } else {
            document.querySelector(`#form_c_new_www_password`).classList.add('has-error');
            document.querySelector(`#form_c_new_www_password`).classList.remove('has-success');
            document.querySelector(`#form_c_new_www_password .formulario_input-error`).classList.add('formulario_input-error-activo');
            campos['new_www_password'] = false;
        }
    }
    // Interceptar el Evento Submit del Boton
document.addEventListener("DOMContentLoaded", function(event) {
    document.getElementById('form').addEventListener('submit', manejadorValidacion)
});

function manejadorValidacion(e) {
    e.preventDefault();
    let page = document.getElementById('page').innerHTML;
    if (page == "Configuración del Broker MQTT") {
        //console.log(page);
        if (campos.mqtt_id && campos.mqtt_user && campos.mqtt_passw && campos.mqtt_server && campos.mqtt_port && campos.mqtt_time) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
            /*Swal.fire({
                title: '¡Guardar!',
                text: '( ' + page + ' )',
                icon: 'warning',
                showCancelButton: true,
                confirmButtonColor: '#3085d6',
                cancelButtonColor: '#d33',
                confirmButtonText: 'Si, guardar',
                cancelButtonText: 'Cancelar',
                reverseButtons: true
            }).then((result) => {
                if (result.isConfirmed) {
                    this.submit();
                } else if (
                    result.dismiss === Swal.DismissReason.cancel
                ) {
                    history.back();
                }
            })*/
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Configuración de la Red Inalámbrica") {
        if (campos.wifi_ssid && campos.wifi_passw && campos.wifi_ip_static && campos.wifi_subnet && campos.wifi_gateway && campos.wifi_primaryDNS && campos.wifi_secondaryDNS && campos.ap_nameap && campos.ap_passwordap && campos.ap_canalap && campos.ap_connetap) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Usuario y Contraseña") {
        if (campos.www_username && campos.www_password && campos.new_www_username && campos.new_www_password) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    } else if (page == "Información del dispositivo") {
        if (campos.id) {
            document.getElementById('formulario_mensaje').classList.remove('formulario_mensaje-activo');
            SweetAlert('¡Guardar!', page, 'question', this);
        } else {
            document.getElementById('formulario_mensaje').classList.add('formulario_mensaje-activo');
            mensajeFormError();
        }
    }
}
// Mansaje para confirmar el Guardado con el Evento Submit
function SweetAlert(title, text, icon, e) {
    Swal.fire({
        title: title,
        text: text,
        icon: icon,
        showCancelButton: true,
        confirmButtonColor: '#3085d6',
        cancelButtonColor: '#d33',
        confirmButtonText: 'Si, guardar',
        cancelButtonText: 'Cancelar',
        reverseButtons: true
    }).then((result) => {
        if (result.isConfirmed) {
            e.submit();
        } else if (
            result.dismiss === Swal.DismissReason.cancel
        ) {
            history.back();
        }
    })
}
// Mansaje de Error al Validar Input
let mensajeFormError = () => {
    Swal.fire({
        position: 'top-end',
        icon: 'error',
        title: '¡No se puede Guardar!',
        showConfirmButton: false,
        timer: 2000
    });
};