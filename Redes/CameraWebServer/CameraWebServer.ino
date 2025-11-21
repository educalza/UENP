#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <esp32-hal-ledc.h>
#include <ESP32Servo.h>

// ===========================
// Seleção do modelo da câmera e pinos
// ===========================
#include "board_config.h"
#include "camera_pins.h"

// ===========================
// Suas credenciais e configurações de rede
// ===========================
const char *ssid = "Edilson Ligga 2.4g";
const char *password = "z1w1w2g1";

// oyjt9992

// ===========================
// Configurações do servidor WebSocket
// ===========================
const char* websocket_server = "192.168.18.6"; // SUBSTITUA PELO IP DO SEU SERVIDOR
const int websocket_port =  8080;
const char* websocket_path = "/";
String carId = "ze"; // Nome da sua equipe

WebSocketsClient webSocket;

// ===========================
// Pinos de controle para os motores e servo
// Usaremos a configuração do seu código:
// IN1, IN2, ENA para a Ponte H e SERVO_PIN para o servo.
// ===========================
#define IN1 13
#define IN3 12
#define IN2 15
#define IN4 14
// #define ENA 14
#define SERVO_PIN 2

// Canais de PWM para o servo
#define SERVO_PWM_CHANNEL 0

// Objeto do servo
Servo meuServo;


// ===========================
// Funções do Servidor e Câmera
// ===========================
void startCameraServer();

// Função de callback para eventos do WebSocket
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_CONNECTED) {
    Serial.println("✅ Conectado ao servidor WebSocket!");

    // Monta a URL de stream com IP local do ESP32
    String streamUrl = "http://" + WiFi.localIP().toString() + ":81/stream";

    // Envia JSON de registro
    DynamicJsonDocument doc(256);
    doc["type"] = "register_car";
    doc["carId"] = carId;
    doc["streamUrl"] = streamUrl;

    String json;
    serializeJson(doc, json);

    webSocket.sendTXT(json);
    Serial.println("📡 Registro enviado: " + json);
  }
  else if (type == WStype_DISCONNECTED) {
    Serial.println("⚠ Desconectado do WebSocket!");
    stopCar();
  }
  else if (type == WStype_TEXT) {
    String msg = (char*)payload;
    msg.trim();
    Serial.printf("📨 Mensagem recebida: %s\n", msg.c_str());

    DynamicJsonDocument doc(128);
    DeserializationError error = deserializeJson(doc, msg);

    if (!error) {
        // Se receber campos x e y, trata como comando analógico
        if (doc.containsKey("x") && doc.containsKey("y")) {
            // Converta para float antes de inverter
            float x = -(doc["x"].as<float>()); // Inverte esquerda/direita
            float y = -(doc["y"].as<float>()); // Inverte frente/trás
            int velocidade = abs(y * 255);

            controlarCarro(x, y, velocidade);
        }
        // Comando digital tradicional
        else if (doc.containsKey("command")) {
            String command = doc["command"].as<String>();
            command.trim();

            if (command == "forward") goForward();
            else if (command == "backward") goBackward();
            else if (command == "left") turnLeft();
            else if (command == "right") turnRight();
            else if (command == "stop") stopCar();
            else Serial.printf("Comando desconhecido: %s\n", command.c_str());
        } else {
            Serial.println("Mensagem não reconhecida ou sem campo 'command'/'x'/'y'");
        }
    } else {
        Serial.println("Erro ao decodificar JSON");
    }
  }
}

// Função para controlar o carro com x/y analógico
void controlarCarro(float x, float y, int velocidade) {
    Serial.printf("Controle analógico recebido: x=%.2f y=%.2f vel=%d\n", x, y, velocidade);

    // Servo: centro (90), esquerda (140), direita (40)
    int anguloServo = 90 + (int)(x * 50); // x=-1: esquerda, x=1: direita
    meuServo.write(anguloServo);

    // Motores: frente, trás ou parado
    if (y > 0.05) { // Para frente
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN4, HIGH);
        digitalWrite(IN3, LOW);
        // PWM (se usar ENA): ledcWrite(ENA, velocidade);
    } else if (y < -0.05) { // Para trás
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
        // PWM (se usar ENA): ledcWrite(ENA, velocidade);
    } else { // Parado
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        // PWM (se usar ENA): ledcWrite(ENA, 0);
    }
}

void setup() {
    Serial.begin(115200);
    // Serial.setDebugOutput(true);
    Serial.println();

    // Configuração dos pinos dos motores
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    // pinMode(ENA, OUTPUT);

    // Anexa o objeto servo ao pino
    meuServo.attach(SERVO_PIN);
    meuServo.write(90); // Centraliza o servo na posição inicial

    // Configuração inicial da câmera
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.frame_size = FRAMESIZE_UXGA;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.jpeg_quality = 20;
    config.fb_count = 1;

    if (config.pixel_format == PIXFORMAT_JPEG) {
        if (psramFound()) {
            config.jpeg_quality = 10;
            config.fb_count = 2;
            config.grab_mode = CAMERA_GRAB_LATEST;
        } else {
            config.frame_size = FRAMESIZE_SVGA;
            config.fb_location = CAMERA_FB_IN_DRAM;
        }
    } else {
        config.frame_size = FRAMESIZE_240X240;
    }

    // Inicialização da câmera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *s = esp_camera_sensor_get();
    if (s->id.PID == OV3660_PID) {
        s->set_vflip(s, 1);
        s->set_brightness(s, 1);
        s->set_saturation(s, -2);
    }

    if (config.pixel_format == PIXFORMAT_JPEG) {
        s->set_framesize(s, FRAMESIZE_QVGA);
    }

    // Conexão WiFi
    WiFi.begin(ssid, password);
    Serial.print("WiFi connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    // Inicia o servidor web da câmera
    startCameraServer();
    Serial.print("Camera Ready! Use 'http://");
    Serial.print(WiFi.localIP());
    Serial.println("' to connect");

    // Conexão WebSocket
    webSocket.begin(websocket_server, websocket_port, websocket_path);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
}

void loop() {
    webSocket.loop();
}

// ===========================
// Funções de controle do carrinho
// ===========================
void goForward() {
    Serial.println("Movendo para frente...");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN4, LOW);
    digitalWrite(IN3, HIGH);
    // digitalWrite(ENA, HIGH);
    meuServo.write(90); // Roda para trás
}

void goBackward() {
    Serial.println("Movendo para trás...");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    // digitalWrite(ENA, HIGH);
    meuServo.write(90); // Roda para frente
}

void turnLeft() {
    Serial.println("Virando para a esquerda...");
    meuServo.write(140);
    // digitalWrite(IN1, HIGH);
    // digitalWrite(IN2, LOW);
    // digitalWrite(IN4, HIGH);
    // digitalWrite(IN3, LOW);
    // digitalWrite(ENA, HIGH);
}

void turnRight() {
    Serial.println("Virando para a direita...");
    meuServo.write(40);
    // digitalWrite(IN1, HIGH);
    // digitalWrite(IN2, LOW);
    // digitalWrite(IN3, HIGH);
    // digitalWrite(IN4, LOW);
    // digitalWrite(ENA, HIGH);
}

void stopCar() {
    Serial.println("Parando...");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    // digitalWrite(ENA, LOW);
    meuServo.write(90); // Retorna o servo para o centro
}