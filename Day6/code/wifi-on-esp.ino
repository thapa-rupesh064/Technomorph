#include <WiFi.h>

const char* ssid = "tesla";
const char* password = "aaaaaaaa";

WiFiServer server(80);

const int ledPin = 5;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // active-low: HIGH = OFF

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected. IP address:");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("New Client.");
  String currentLine = "";

  while (client.connected()) {
    if (client.available()) {
      char c = client.read();
      currentLine += c;
      Serial.write(c);

      if (c == '\n') {
        if (currentLine.indexOf("GET /H") >= 0) {
          digitalWrite(ledPin, LOW);  // ON (active-low)
        } else if (currentLine.indexOf("GET /L") >= 0) {
          digitalWrite(ledPin, HIGH); // OFF (active-low)
        }

        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        client.println(R"rawliteral(
          <!DOCTYPE html>
          <html>
          <head>
            <meta name="viewport" content="width=device-width, initial-scale=1">
            <title>ESP32 LED Control</title>
          </head>
          <body>
            <h1>ESP32 LED Control</h1>
            <l><a href="/H">Turn LED ON</a></l></br>
            <l><a href="/L">Turn LED OFF</a></l>
          </body>
          </html>
        )rawliteral");
        break;
      }
    }
  }

  delay(1);
  client.stop();
  Serial.println("Client Disconnected.");
}
