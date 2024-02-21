// declaration
int led = LED_BUILTIN; // PIN NUMBER 42 DEFAULT
// setup
void setup() {
  pinMode(led,OUTPUT);

}

void loop() {
  digitalWrite(led,HIGH);
  Serial.println("led on");
  delay(10000);
  digitalWrite(led,LOW);
  Serial.println("led off");
  delay(10000);
}
