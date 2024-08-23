#include <ESP8266WiFi.h>
#include <Wire.h>
#include "SSD1306.h"

#define flipDisplay true;

const char* ssid = "Redmi Note 11 Pro 5G"; //Nombre de la red WiFi a la que nos vamos a conectar
const char* password = "Kido.87Cu"; //Contrasena de la red WiFi a la que nos vamos a conectar


SSD1306 display(0x3c, D6, D5); //SDA - GPIO 14 = D6, SCL - GPIO 12 = D5
#define flipDisplay true //Pantalla en horizontal

void setup() {
  Serial.begin(115200);
  delay(10);
  /* Inicializamos la pantalla */
  display.init();
  if (flipDisplay) display.flipScreenVertically();

  /* Mostramos la pantalla de bienvenida */
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(15, 25, "Kido.IoT");
  display.display();
  delay (3000);
  
  /*Mostrar red en pantala*/
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);  
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 0, "Connecting to:");
  display.drawString(0, 32, ssid);
  display.display();

  /*Conectarnos a la red WiFi*/
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  /*Mostrar nuestra IP*/
  Serial.println("");
  Serial.print("Red: ");
  Serial.println(ssid);
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  String ip = WiFi.localIP().toString();

  display.clear();
  display.drawString(0, 0, "Red: ");
  display.drawString(40, 0, ssid);
  display.drawString(0, 20, "IP address:");
  display.drawString(0, 40, ip);
  display.display();
}

void loop() {
}
