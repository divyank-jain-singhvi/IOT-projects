
// int ir_sensor=7;
int ldr=7;

void setup() {
  // pinMode(ir_sensor,INPUT);
  pinMode(ldr,INPUT);

}

void loop() {
  // int data = digitalRead(ir_sensor);
  int data = digitalRead(ldr);
  Serial.println(data);

}
