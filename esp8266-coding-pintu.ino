#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
 #include <BlynkSimpleEsp8266.h>

#define SS_PIN 4 //PIN SDA ke D2
#define RST_PIN 5 //PIN RST ke D1
MFRC522 rfid(SS_PIN, RST_PIN); //mendefinisikan rfid / menngabungkan pin menjadi 1
Servo myServo;
int LED_G = D0; //led hijau
int LED_R = D8; //led merah 
char auth[] = "SXbAzkCOIX8ygVucAOfz0atR5_WxQ4_c"; //menghubungkan esp ke aplikasi blynk yang didapatakan ketika kita membuat project pada blynk

char ssid[] = "ARGA'S Hotspot";
char pass[] = "@silandak37";
SimpleTimer timer;
WidgetTerminal terminal(V2);
WidgetLED LED(V5); //widget led hijau pada blynk
WidgetLED LED1(V6); //widget led merah pada blynk  


void setup(){
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_R, OUTPUT);
    myServo.attach(2);
    myServo.write(0);
    SPI.begin();
    rfid.PCD_Init();
      timer.setInterval(1000L, iot_rfid);
  }

void loop(){
  timer.run();
  Blynk.run();
  
 }

 void iot_rfid(){
  if( ! rfid.PICC_IsNewCardPresent()){
    return;
    }
    if( ! rfid.PICC_ReadCardSerial()){
      return;
      }
      
      Blynk.virtualWrite(V2, "Scan Disini !!");
  Serial.print("UID tag : ");
  String content = "";
  byte letter;
  for (byte i = 0; i < rfid.uid.size;i++){
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], DEC);
    Blynk.virtualWrite(V2,rfid.uid.uidByte[i]);
    content.concat(String(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ")); 
    content.concat(String(rfid.uid.uidByte[i], DEC));
  }
    Serial.println();
    content.toUpperCase();

    if(content.substring(1)  == "")
    {
      Serial.print("Akses Diterima ");
      Serial.println();
      LED.on();
      digitalWrite(LED_G, HIGH);
      Blynk.virtualWrite(V2, " Akses Diterima ");
      delay(500);
      Blynk.virtualWrite(V2, " Selamat datang Kelompok-2");
      delay(1250);
      myServo.write(190);
      delay(5000);
      myServo.write(20);
      LED.off();
      digitalWrite(LED_G, LOW);
      Serial.println();
      }
      else
      {
        Serial.print("Akses Ditolak ");
        Serial.println();
        Serial.println();
        Blynk.virtualWrite(V2, " Akses Ditolak ");
        LED1.on();
        digitalWrite(LED_R, HIGH);
        delay(1500);
        LED1.off();
        digitalWrite(LED_R, LOW);
        Serial.println();
        }
        
      Blynk.virtualWrite(V2, "Scan Disini !!");    
    }
