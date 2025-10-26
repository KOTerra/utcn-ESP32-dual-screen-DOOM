void setup() {
  Serial.begin(115200); // Initialize serial at 115200 baud
  while (!Serial);      // Wait for Serial to be ready
  delay(1000);

  Serial.println("Hello from ESP32-S3!");
}

void loop() {
  Serial.println("Printing every second...");
  delay(1000); // Wait 1 second
}
