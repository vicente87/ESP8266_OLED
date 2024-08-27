#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "KidoIoT";          // Tu SSID de Wi-Fi
const char* password = "Kido.87Cu";  // Tu contraseña de Wi-Fi



;  // Tu contraseña de Wi-Fi

const char* serverUrl = "http://192.168.1.143/system/services/cowsay"; // URL del servidor
const char* user = "oscar";            // Usuario para autenticación básica
const char* pass = "OWRkYmRj";            // Contraseña para autenticación básica

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
    HTTPClient http;
    WiFiClient client;
    // Inicializa la conexión con la URL del servidor
    http.begin(client,serverUrl);
    http.setTimeout(10000);
    // Configuración de autenticación básica
    http.setAuthorization(user, pass);
    // http.setAuthorization("base64encoded_String");
    // Configurar el tipo de contenido (Content-Type)
    http.addHeader("Content-Type", "application/json");

    // Realizar la solicitud GET
    int httpCode = http.GET();

    // Verificar si la solicitud fue exitosa
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("HTTP Code: " + String(httpCode));
      Serial.println("Response: " + payload);
      http.end();
    // Buscar el token del servicio

      String browser[] = {"token\":\"", "\",\"file_stage_in\""};
      int pos[2];
      int k = -1;
      bool found = true;

      for (int i = 0; i < 2; i++) {
        k = payload.indexOf(browser[i]);
        if (k != -1) {
          pos[i] = k;
        } 
        else {
          found = false;
          break;
        }
      }

      if (!found) {
        Serial.println("Error in connection");
      } 
      else {
        String token = payload.substring(pos[0] + 8, pos[1]);
        Serial.println(token);
        HTTPClient http;
        String serviceUrl="http://192.168.1.143/run/cowsay";
        http.begin(client,serviceUrl);
        http.setTimeout(10000);

        // Set headers
        http.addHeader("Authorization", "Bearer " + token);
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        // Set data
        String data = "{\"message\": \"Test with ESP8266\"}";

        // Send POST request
        Serial.println("Execute service");
        int httpResponseCode = http.POST(data);

        if (httpResponseCode > 0) {
          if (httpResponseCode == 200) {
            Serial.println("Services OK");
            String response = http.getString();
            Serial.println(response);

          } 
          else {
            Serial.print("Error running service: ");
             Serial.println(http.errorToString(httpResponseCode).c_str());
          }
        }
        http.end();
      } 
    }else {
        Serial.println("Error in HTTP request");
    }

    http.end(); // Liberar recursos
  }

}


void loop() {
  // No necesitas nada en el loop en este ejemplo
}