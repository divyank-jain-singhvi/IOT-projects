#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPSPlus.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

#define DHTPIN 7
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define DHTTYPE    DHT11 
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
#if defined(ESP32)
#endif

#define WIFI_SSID "OPPO A52"
#define WIFI_PASSWORD "qqts2110"

#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

#define AUTHOR_EMAIL "divyanksinghvi@gmail.com"
#define AUTHOR_PASSWORD "kxfe mmus xjuh yhby"
#define RECIPIENT_EMAIL "ektaghosh2002@gmail.com"

SMTPSession smtp;

void smtpCallback(SMTP_Status status);
void sendingMail();


DHT_Unified dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
uint32_t delayMS;
int buzz=9;
const int sensorPin = A0;
int flag=0; 

const char *gpsStream =
  "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*7C\r\n"
  "$GPGGA,045104.000,3014.1985,N,09749.2873,W,1,09,1.2,211.6,M,-22.5,M,,0000*62\r\n"
  "$GPRMC,045200.000,A,3014.3820,N,09748.9514,W,36.88,65.02,030913,,,A*77\r\n"
  "$GPGGA,045201.000,3014.3864,N,09748.9411,W,1,10,1.2,200.8,M,-22.5,M,,0000*6C\r\n"
  "$GPRMC,045251.000,A,3014.4275,N,09749.0626,W,0.51,217.94,030913,,,A*7D\r\n"
  "$GPGGA,045252.000,3014.4273,N,09749.0628,W,1,09,1.3,206.9,M,-22.5,M,,0000*6F\r\n";
TinyGPSPlus gps;
sensors_event_t event;
// sensor_t sensor;
#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };


void setup() {
  pinMode(buzz, OUTPUT);
  Serial.begin(9600);
  Serial1.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
   Serial.print("Connecting to Wi-Fi");
   while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();


  Serial.println(F("starting our program"));
  Serial1.println(F("Start serial 2"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
  while (*gpsStream)
    if (gps.encode(*gpsStream++))
      displayInfo();
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }


    display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,28);
  display.println("WELCOME");
  display.display();
  delay(2000);
  display.clearDisplay();

  // Display Inverted Text
  // display.setTextColor(WHITE,BLACK); // 'inverted' text
  display.setCursor(0,28);
  display.println("Gas level");
  display.display();
  delay(2000);
  display.clearDisplay();

 
  // Scroll part of the screen
  // while (true){
  //   display.setCursor(0,0);
  //   display.setTextSize(2);
  //   display.println(analogRead(sensorPin));
  //   display.display();
  //   delay(1000);
  //   display.clearDisplay();
  //   Serial.println(event.relative_humidity)
  // }
}

void loop() {
  
  delay(delayMS);
  display.setCursor(0,0);
  display.setTextSize(2);
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    display.println(event.temperature);
  }
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    display.println(event.relative_humidity);
  }

  if (isnan(analogRead(sensorPin))) {
    Serial.println(F("Error reading humidity!"));
    
  }
  else{
    Serial.print("Analog output: ");
    Serial.println(analogRead(sensorPin)); 
    display.println(analogRead(sensorPin));
    // display.setCursor(0,0);
    // display.setTextSize(2);
    // display.println(analogRead(sensorPin));
    // display.println(event.relative_humidity);
    // display.println(event.temperature);
    // display.display();
    // delay(1000);
    // display.clearDisplay();
    // delay(1000);
    displayInfo();
  }
  if (event.temperature > 90){ // temperature of a average mines is 60 degree 
    tone(buzz, 2000);
      sendingMail();
    // delay(2000);
    // noTone(buzz);

  }  
  if (event.relative_humidity > 60){ // humidity of a average mines is 50% humidity 
    tone(buzz, 2000);
      sendingMail();
    // delay(2000);
    // noTone(buzz);
  }  
  if (analogRead(sensorPin) > 4000){ // humidity of a average mines is 50% humidity 
    tone(buzz, 2000);
      sendingMail();
    // delay(2000);
    // noTone(buzz);
  }  
  if (event.temperature < 20){ // temperature of a average mines is 60 degree 
    noTone(buzz);
    // delay(2000);
    // noTone(buzz);

  }  
  if (event.relative_humidity < 60){ // humidity of a average mines is 50% humidity 
    noTone(buzz);
    // delay(2000);
    // noTone(buzz);
  }  
  if (analogRead(sensorPin) < 4000){ // humidity of a average mines is 50% humidity 
    noTone(buzz);
    // delay(2000);
    // noTone(buzz);
  }  
  
  // display.setCursor(0,0);
  // display.setTextSize(2);
  // display.println(analogRead(sensorPin));
  // display.display();
  // delay(1000);
  // display.clearDisplay();
  display.display();
  delay(1000);
  display.clearDisplay();
}

void displayInfo(){
  Serial.print(F("Location: ")); 
  if (gps.location.isValid()){
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
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

void sendingMail(){
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
  message.sender.name = F("Coal mining team");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Rescue operation");
  message.addRecipient(F("Rescue Team"), RECIPIENT_EMAIL);
   
  //Send raw text message
  String textMsg = "hello";
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
  flag=1;
}
