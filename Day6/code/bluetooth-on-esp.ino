#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int ledPin = 5;              // LED connected to GPIO 5
const bool LED_ACTIVE_LOW = true;  // Flip logic if LED is active-low

String device_name = "ESP32";
String incomingMessage = "";

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LED_ACTIVE_LOW ? HIGH : LOW); // Start with LED OFF

  SerialBT.begin(device_name);
  Serial.printf("Bluetooth device \"%s\" started.\nPair and send 'on' or 'off'.\n", device_name.c_str());
}

void loop() {
  // Relay from serial monitor to Bluetooth
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  // Read from Bluetooth
  if (SerialBT.available()) {
    char c = SerialBT.read();

    // Collect characters into a message
    if (c == '\n' || c == '\r') {
      incomingMessage.trim();
      Serial.printf("Received via Bluetooth: %s\n", incomingMessage.c_str());

      if (incomingMessage.equalsIgnoreCase("on")) {
        digitalWrite(ledPin, LED_ACTIVE_LOW ? LOW : HIGH);
        Serial.println("LED turned ON");
      } else if (incomingMessage.equalsIgnoreCase("off")) {
        digitalWrite(ledPin, LED_ACTIVE_LOW ? HIGH : LOW);
        Serial.println("LED turned OFF");
      }

      incomingMessage = ""; // clear for next command
    } else {
      incomingMessage += c;
    }
  }

  delay(10);
}