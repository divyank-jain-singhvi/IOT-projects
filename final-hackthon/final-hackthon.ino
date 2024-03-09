#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#endif
#include <ESP_Mail_Client.h>

#define WIFI_SSID "colourofsky"
#define WIFI_PASSWORD "doesntexist"
void sendingMail(String msg,String msg1);
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465


#define AUTHOR_EMAIL "divyanksinghvi@gmail.com"
#define AUTHOR_PASSWORD "kxfe mmus xjuh yhby"


#define RECIPIENT_EMAIL "ektaghosh2002@gmail.com"


SMTPSession smtp;

void smtpCallback(SMTP_Status status);
int buzz=6;
const int buttonPin=9;  
const int ledPin = LED_BUILTIN;
int buttonState = 0;         
int sensorPin = 8;
int sensorValue;
int lastTiltState = HIGH;
long lastDebounceTime = 0;  
long debounceDelay = 50;
String latitude;
String longitude;
#include <TinyGPSPlus.h>
const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
TinyGPSPlus gps;
void setup() {
   pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(buzz,OUTPUT);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting to Wi-Fi");
   while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      displayInfo();
 
}

void loop() {
  sensorValue = digitalRead(sensorPin);
  buttonState = digitalRead(buttonPin);
  
  
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzz,HIGH);
    displayInfo();
    if (gps.location.isValid()){
    latitude = (gps.location.lat());
    longitude = (gps.location.lng());
    sendingMail(latitude,longitude);
    }
    else{
    Serial.print(F("INVALID"));
  }
    // sendingMail();
    digitalWrite(ledPin, lastTiltState);
    Serial.println(sensorValue);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzz,LOW);
  }

  if (sensorValue == lastTiltState) {

    lastDebounceTime = millis();
  } 
    if ((millis() - lastDebounceTime) > debounceDelay) {
      lastTiltState = sensorValue;
  }
    delay(500);
}


void displayInfo(){
  Serial.print(F("Location: ")); 
  
  if (gps.location.isValid()){
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    // latitude = (gps.location.lat());
    // longitude = (gps.location.lng());
    // int x=1;
    // if (x==1){
    //   sendingMail(latitude,longitude);
    //   x=0
    // }
  }
  else{
    Serial.print(F("INVALID"));
  }
  Serial.println();
}

void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      SMTP_Result result = smtp.sendingResult.getItem(i);

      
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    smtp.sendingResult.clear();
  }
}

void sendingMail(String msg,String msg1){
  // WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //  Serial.print("Connecting to Wi-Fi");
  //  while (WiFi.status() != WL_CONNECTED){
  //   Serial.print(".");
  //   delay(300);
  // }
  // Serial.print("Connected with IP: ");
  // Serial.println(WiFi.localIP());
  // Serial.println();


  MailClient.networkReconnect(true);
  smtp.debug(1);
  smtp.callback(smtpCallback);
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("Helper");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Rescue operation");
  message.addRecipient(F("Rescue Team"), RECIPIENT_EMAIL);
   
  //Send raw text message
  String mess=("HELP!! I am feeling unsafe, please arrive at this location to help me."+msg+","+msg1+"check on port http://192.168.250.179");
  String textMsg = mess;
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)){
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()){
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
}




