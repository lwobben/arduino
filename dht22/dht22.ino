// libraries
#include "DHT.h"
#include "ESP8266WiFi.h"
#include "aREST.h"
#include "secrets.h"

// vars
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
#define DHTPIN D2
#define DHTTYPE DHT22
#define LISTEN_PORT 80
float humidity;
float temperature;

// set instances
WiFiServer server(LISTEN_PORT);
aREST rest = aREST();
DHT dht(DHTPIN, DHTTYPE);

// run once
void setup() {
  Serial.begin(9600);
  Serial.println("Start setup");
  dht.begin();

  // REST API vars
  rest.variable("humidity",&humidity);
  rest.variable("temperature",&temperature);
  rest.set_id("1");
  rest.set_name("esp8266");
  
  // connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // start the server
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());
}

// repeat
void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  // handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}