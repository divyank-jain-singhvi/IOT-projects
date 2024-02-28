int swit= 8;
int led=42;
int mode=0;
void setup() {
  pinMode(swit,INPUT);
  pinMode(led,OUTPUT);

}

void loop() {
  int x= digitalRead(swit);
  if (x == HIGH){
    if (mode==0){
      mode=1;
      digitalWrite(led,HIGH);
    }
    else if (mode==1){
      mode=0;
      digitalWrite(led,LOW);
    }
    delay(3000);
  }
}
