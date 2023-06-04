const int RELAY_PIN = D7;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);
  delay(5000);
  digitalWrite(RELAY_PIN, LOW);
  ESP.deepSleep(15e6); // connect D0 to RST AFTER uploading the code!
}
