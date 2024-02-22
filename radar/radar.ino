#include<ESP32Servo.h>

Servo myservo;

int pos=0;
void setup(){
  Serial.begin(115200);
  myservo.attach(9);
}
void loop(){
  for(pos=0;pos<=90;pos+=1){
  myservo.write(pos);
  delay(15);
}
  for(pos=90;pos>=0;pos-=1){
  myservo.write(pos);
  delay(15);
}
}