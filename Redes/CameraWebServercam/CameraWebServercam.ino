// Arquivo: esp32_cam_ip_ANNOUNCER.ino

#include "esp_camera.h"
#include <WiFi.h>
#include <WiFiUdp.h> // Biblioteca para comunicação UDP

// ===========================
// Configurações de Rede
// ===========================
const char *ssid = "Eduardo's S23";
const char *password = "1234567e";
const int udpPort = 4210; // Porta para a comunicação. Deve ser a mesma no receptor.

WiFiUDP udp;
unsigned long lastAnnounceTime = 0;

// ===========================
// Câmera
// ===========================
#include "board_config.h"
#include "camera_pins.h"

void startCameraServer();

void setup() {
  Serial.begin(115200);
  
  // -- COLOQUE AQUI TODA A SUA CONFIGURAÇÃO DE CÂMERA --
  // camera_config_t config;
  // ...
  // esp_err_t err = esp_camera_init(&config);
  // ...
  
  // Conexão WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Meu IP (Câmera): ");
  Serial.println(WiFi.localIP());

  // Inicia o servidor de vídeo da câmera
  startCameraServer();
  Serial.println("Servidor da Câmera iniciado.");
}

void loop() {
  // A cada 5 segundos, anuncia o IP na rede
  if (millis() - lastAnnounceTime > 5000) {
    String ipMessage = "CAM_IP:" + WiFi.localIP().toString();
    
    // Inicia um pacote UDP para o endereço de broadcast da rede
    udp.beginPacket(WiFi.broadcastIP(), udpPort);
    udp.print(ipMessage);
    udp.endPacket();
    
    Serial.print("Anunciando na rede: ");
    Serial.println(ipMessage);
    
    lastAnnounceTime = millis();
  }
}