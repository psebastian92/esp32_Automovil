#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebSocketsServer.h>

WiFiMulti wifiMulti;
WebSocketsServer servidorWebSocket = WebSocketsServer(81);  // Puerto 81 para WebSocket

// Pines de los motores
#define I_AD1 27  // Motor delantero izquierdo
#define I_AD2 26
#define I_AT1 32  // Motor trasero izquierdo
#define I_AT2 18
#define D_AD1 5   // Motor delantero derecho
#define D_AD2 14
#define D_AT1 25  // Motor trasero derecho
#define D_AT2 33

#define LED_INTEGRADO 2  // LED del ESP32

// Control de tiempo para detener el auto si no recibe comandos
unsigned long ultimoComando = 0;
bool enMovimiento = false;
const unsigned long TIEMPO_LIMITE_MS = 500;  // 500 milisegundos

void setup() {
  Serial.begin(115200);

  // Configuración de pines
  pinMode(I_AD1, OUTPUT); pinMode(I_AD2, OUTPUT);
  pinMode(I_AT1, OUTPUT); pinMode(I_AT2, OUTPUT);
  pinMode(D_AD1, OUTPUT); pinMode(D_AD2, OUTPUT);
  pinMode(D_AT1, OUTPUT); pinMode(D_AT2, OUTPUT);
  pinMode(LED_INTEGRADO, OUTPUT);

  detenerMotores();

  // Conexión WiFi
  wifiMulti.addAP("TP-Link_3FE2", "26826615");
  Serial.print("Conectando a WiFi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConectado a WiFi");
  Serial.println(WiFi.localIP());

  servidorWebSocket.begin();
  servidorWebSocket.onEvent(manejarEventoWebSocket);
  Serial.println("Servidor WebSocket iniciado en el puerto 81");
}

void loop() {
  servidorWebSocket.loop();
}

void manejarEventoWebSocket(uint8_t cliente, WStype_t tipo, uint8_t *mensaje, size_t longitud) {
  if (tipo == WStype_TEXT) {
    String comando = String((char *)mensaje);
    Serial.println("Comando recibido: " + comando);

    if (comando != "stop") {
      ultimoComando = millis();
      enMovimiento = true;
    }

    if (comando == "w") avanzar();
    else if (comando == "s") retroceder();
    else if (comando == "a") avanzarIzquierda();
    else if (comando == "d") avanzarDerecha();
    else if (comando == "q") avanzarDiagonalIzquierda();
    else if (comando == "e") avanzarDiagonalDerecha();
    else if (comando == "z") retrocederDiagonalIzquierda();
    else if (comando == "c") retrocederDiagonalDerecha();
     else if (comando == "1") giroAntiHorario();
      else if (comando == "2") giroHorario();
    else if (comando == "stop") {
      detenerMotores();
      enMovimiento = false;
    }
  }
}

// Funciones de movimiento

void avanzar() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, HIGH);
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, HIGH);
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, HIGH);
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, HIGH);
}

void retroceder() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, HIGH); digitalWrite(I_AD2, LOW);
  digitalWrite(I_AT1, HIGH); digitalWrite(I_AT2, LOW);
  digitalWrite(D_AD1, HIGH); digitalWrite(D_AD2, LOW);
  digitalWrite(D_AT1, HIGH); digitalWrite(D_AT2, LOW);
}

void avanzarIzquierda () {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, HIGH); digitalWrite(I_AD2, LOW);
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, HIGH);
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, HIGH);
  digitalWrite(D_AT1, HIGH); digitalWrite(D_AT2, LOW);
}

void avanzarDerecha () {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, HIGH);
  digitalWrite(I_AT1, HIGH); digitalWrite(I_AT2, LOW);
  digitalWrite(D_AD1, HIGH); digitalWrite(D_AD2, LOW);
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, HIGH);
}

void giroAntiHorario() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, HIGH); digitalWrite(I_AD2, LOW);
  digitalWrite(I_AT1, HIGH); digitalWrite(I_AT2, LOW);
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, HIGH);
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, HIGH);
}

void giroHorario() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, HIGH);
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, HIGH);
  digitalWrite(D_AD1, HIGH); digitalWrite(D_AD2, LOW);
  digitalWrite(D_AT1, HIGH); digitalWrite(D_AT2, LOW);
}

void avanzarDiagonalIzquierda() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, LOW);    
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, HIGH);   
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, HIGH);   
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, LOW);    
}

void avanzarDiagonalDerecha() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, HIGH);   
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, LOW);    
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, LOW);    
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, HIGH);   
}

void retrocederDiagonalIzquierda() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, HIGH); digitalWrite(I_AD2, LOW);    
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, LOW);     
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, LOW);     
  digitalWrite(D_AT1, HIGH); digitalWrite(D_AT2, LOW);    
}

void retrocederDiagonalDerecha() {
  digitalWrite(LED_INTEGRADO, HIGH);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, LOW);
  digitalWrite(I_AT1, HIGH); digitalWrite(I_AT2, LOW);
  digitalWrite(D_AD1, HIGH); digitalWrite(D_AD2, LOW);    
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, LOW);     
}

void detenerMotores() {
  digitalWrite(LED_INTEGRADO, LOW);
  digitalWrite(I_AD1, LOW); digitalWrite(I_AD2, LOW);
  digitalWrite(I_AT1, LOW); digitalWrite(I_AT2, LOW);
  digitalWrite(D_AD1, LOW); digitalWrite(D_AD2, LOW);
  digitalWrite(D_AT1, LOW); digitalWrite(D_AT2, LOW);
}
