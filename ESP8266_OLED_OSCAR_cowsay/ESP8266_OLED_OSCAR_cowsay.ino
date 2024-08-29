#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "KidoIoT";          // Tu SSID de Wi-Fi
const char* password = "Kido.87Cu";  // Tu contraseña de Wi-Fi



;  // Tu contraseña de Wi-Fi

const char* serverUrl = "http://192.168.1.143"; // URL del servidor
const char* service= "cowsay";
const char* user = "oscar";            // Usuario para autenticación básica
const char* pass = "OWRkYmRj";            // Contraseña para autenticación básica

String msg = "Test with ESP8266";



String extractToken(String payload) {
  String browser[] = {"token\":\"", "\",\"file_stage_in\""};
  int pos[2];
  int k = -1;
  bool found = true;

  // Buscar las posiciones de los delimitadores en el payload
  for (int i = 0; i < 2; i++) {
    k = payload.indexOf(browser[i]);
    if (k != -1) {
      pos[i] = k;
    } else {
      found = false;
      break;
    }
  }

  // Si no se encuentran ambos delimitadores, se retorna un mensaje de error
  if (!found) {
    Serial.println("Error in connection");
    return "";
  } 
  else {
    // Extraer y devolver el token
    return payload.substring(pos[0] + 8, pos[1]);
  }
}


void executeService(WiFiClient& client, String serverUrl, String service,String token, String msg, String& response, int& httpResponseCode) {
    HTTPClient http;
    String runService = String(serverUrl) + "/run/" + String(service);
    http.begin(client, runService);
    http.setTimeout(10000);

    // Configurar headers
    http.addHeader("Authorization", "Bearer " + token);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Configurar datos
    String data = "{\"message\": \"" + msg + "\"}";

    // Enviar la solicitud POST
    Serial.println("Execute service");
    httpResponseCode = http.POST(data);

    if (httpResponseCode > 0) {
        if (httpResponseCode == 200) {
            Serial.println("Service OK");
            response = http.getString();
            //Serial.println(response);
        } else {
            Serial.print("Error running service: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
            response = "Error: " + http.errorToString(httpResponseCode);
        }
    } else {
        Serial.println("Error in HTTP request");
        response = "Error in HTTP request";
    }

    http.end();
}

void getServiceResponse(WiFiClient& client, String serverUrl, String service, String user, String pass, String& payload, int& httpCode) {
    HTTPClient http;
    String invokeService = String(serverUrl) + "/system/services/" + String(service);
    
    // Iniciar la conexión HTTP
    http.begin(client, invokeService);
    http.setTimeout(10000);

    // Configuración de autenticación básica
    http.setAuthorization(user.c_str(), pass.c_str());
    
    // Configurar el tipo de contenido (Content-Type)
    http.addHeader("Content-Type", "application/json");

    // Realizar la solicitud GET
    httpCode = http.GET();

    // Verificar si la solicitud fue exitosa
    if (httpCode > 0) {
        payload = http.getString();
       
    } else {
        payload = ""; // Asignar cadena vacía si la solicitud falla
        Serial.println("Error in HTTP request");
    }

    // Liberar recursos HTTP
    http.end();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  if (WiFi.status() == WL_CONNECTED) {
   
    String serviceData;
    int httpCode;
      WiFiClient client;
    // Llamar a la función para obtener la respuesta del servicio
       getServiceResponse(client, serverUrl, service, user, pass, serviceData, httpCode);
       Serial.println("HTTP Code: " + String(httpCode));
        Serial.println("Response: " + serviceData);

    if (httpCode==200){
        String token =extractToken(serviceData);
      if (token=="") {
        Serial.println("Error in connection");
      } 
      else {
        //String token = payload.substring(pos[0] + 8, pos[1]);
        Serial.println(token);

        String response;
        int httpResponseCode;

    // Llamar a la función para ejecutar el servicio
        executeService(client, serverUrl, service, token, msg, response, httpResponseCode);

    // Imprimir la respuesta y el código HTTP
        Serial.println("HTTP Response Code: " + String(httpResponseCode));
        Serial.println("Response: " + response);
      
    }
       }else {
        Serial.println("Error in HTTP request");
    }

    //http.end(); // Liberar recursos
  }

}


void loop() {
  // No necesitas nada en el loop en este ejemplo
}