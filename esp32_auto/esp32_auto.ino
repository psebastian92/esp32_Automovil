#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebSocketsServer.h>

WiFiMulti wifiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);  // WebSocket en puerto 81

// Pines de motores (8 pines para 4 motores)
#define IN1 14  // Motor 1
#define IN2 12
#define IN3 27  // Motor 2
#define IN4 26
#define IN5 25  // Motor 3 (agregado)
#define IN6 33
#define IN7 32  // Motor 4 (agregado)
#define IN8 13

#define LED_INTEGRADO 2

void setup() {
  Serial.begin(115200);

  // Pines de salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  pinMode(LED_INTEGRADO, OUTPUT);
  
  stopMotors();

  // Conexión WiFi
  wifiMulti.addAP("TP-Link_3FE2", "26826615");
  Serial.print("Conectando a WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConectado a WiFi");
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);

  Serial.println("Servidor WebSocket iniciado en el puerto 81");
}

void loop() {
  webSocket.loop();
}

void onWebSocketEvent(uint8_t client, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char *)payload);
    Serial.println("Comando: " + msg);

    if (msg == "w") adelante();
    else if (msg == "s") atras();
    else if (msg == "a") izquierda();
    else if (msg == "d") derecha();
    else if (msg == "stop") stopMotors();
  }
}

// Control de motores

// Todos adelante
void adelante() {
  digitalWrite(LED_INTEGRADO, HIGH);
  // Motor 1
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor 2
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Motor 3
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  // Motor 4
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

// Todos atrás
void atras() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

// Girar a la izquierda: motores derechos adelante, motores izquierdos atrás
void izquierda() {
  digitalWrite(LED_INTEGRADO, HIGH);
  // Motor 1 (izquierdo)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor 2 (izquierdo)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Motor 3 (derecho)
  digitalWrite(IN5, HIGH);
  digitalWrite(IN6, LOW);
  // Motor 4 (derecho)
  digitalWrite(IN7, HIGH);
  digitalWrite(IN8, LOW);
}

// Girar a la derecha: motores izquierdos adelante, motores derechos atrás
void derecha() {
  digitalWrite(LED_INTEGRADO, HIGH);
  // Motor 1 (izquierdo)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor 2 (izquierdo)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  // Motor 3 (derecho)
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, HIGH);
  // Motor 4 (derecho)
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, HIGH);
}

void stopMotors() {
  digitalWrite(LED_INTEGRADO, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
  digitalWrite(IN6, LOW);
  digitalWrite(IN7, LOW);
  digitalWrite(IN8, LOW);
}
