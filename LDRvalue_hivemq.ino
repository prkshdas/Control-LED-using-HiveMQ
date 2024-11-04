#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "realme 9 5G"; // Replace with your Wi-Fi SSID
const char* password = "@1234567"; // Replace with your Wi-Fi password
const char* mqtt_server = "broker.hivemq.com"; // HiveMQ broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  setupWiFi();
  
  // Set MQTT server
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Read LDR value
  int ldrValue = analogRead(A0);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Publish LDR value
  client.publish("home/ldr", String(ldrValue).c_str());

  delay(5000); // Publish every 5 seconds
}

void setupWiFi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("NodeMCUClient")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
