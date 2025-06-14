#include <WiFi.h>
#include <NetworkClient.h>
#include <WiFiAP.h>
#include <dhtESP32-rmt.h>

const uint8_t ledPin  = 17;
float temperature = 0.0;
float humdity = 0.0;
const char *ssid = "ESP32AP";
const char *password = "complicatedPassword";
IPAddress myIP;

NetworkServer server(80);

void tempTask(void *pvParameters) {
  for (;;) {
    uint8_t error = read_dht(temperature, humdity, 18, DHT11);
    if (error) {
      Serial.println("Error: " + String(error));
    } else {
      Serial.println("TEMP:" + String(temperature));
      //Serial.println("humdity: " + String(humdity));
    }
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  Serial.println();
  Serial.println("Configurin access point...");
  xTaskCreatePinnedToCore(tempTask, "tempTask", 8192, NULL, 1, NULL, 0);

  if (!WiFi.softAP(ssid, password)) {
    log_e("Soft AP creation faild.");
    while(10);
  }

  myIP = WiFi.softAPIP();
  Serial.println("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  Serial.println("Server started.");
}

void loop() {
  NetworkClient client = server.accept();

  if (client) {
    String request = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        request += c;
        if (c == '\n' && request.endsWith("\r\n\r\n")) {
          break;
        }
      }
    }

    if (request.indexOf("GET /led/on") >= 0) {
      digitalWrite(ledPin, HIGH);
    } else if (request.indexOf("GET /led/off") >= 0) {
      digitalWrite(ledPin, LOW);
    }

    String response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
    response += "{";
    response += "\"temperature\":" + String(temperature, 1) + ",";
    response += "\"humidity\":" + String(humdity, 1);
    response += "}";

    client.print(response);
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void serialEvent() {
  while (Serial.available()) {
    char state = (char)Serial.read();
    if (state == '0') {
      digitalWrite(ledPin, 0);
    } else if (state == '1') {
      digitalWrite(ledPin, 1);
    }
  }
}
