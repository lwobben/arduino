#include "DHT.h"
#include "ESP8266WiFi.h"
#include "aREST.h"
#include "secrets.h"

#define DHTPIN D2
#define DHTTYPE DHT22
#define LISTEN_PORT 80

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
float humidity;
float temperature;

WiFiServer server(LISTEN_PORT);
aREST rest = aREST();
DHT dht(DHTPIN, DHTTYPE);

void startServer() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  }
  Serial.println("WiFi connected");
  
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(9600);
  Serial.println("Start setup");
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();

  rest.variable("humidity",&humidity);
  rest.variable("temperature",&temperature);
  rest.set_id("1");
  rest.set_name("esp8266_liana");
  
  startServer();
}

// repeat
void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  while (WiFi.status() != WL_CONNECTED) {
    startServer();
  }
  
  WiFiClient client = server.available();
  while (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}
