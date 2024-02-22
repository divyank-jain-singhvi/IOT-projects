// ultrasonic sensor

// distance??
// speed of sound 340m/s
// speed =0.034 microseconds
// time =10/0.034

// distance =speed*time/2
const int trigPin=7;
const int ecoPin=8;
long duration;
int distance;
void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(ecoPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(ecoPin,HIGH);
  distance = duration*0.034/2;
  Serial.print("Distance");
  Serial.println(distance);

}
