#define BLYNK_TEMPLATE_ID "TMPLNYCXRGiD"
#define BLYNK_DEVICE_NAME "intruder alarm"
#define BLYNK_AUTH_TOKEN "EbgN5HN6w0cqTGPPymkijZSOlhKiuhu_"



// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Redmi Note 7 Pro";
char pass[] = "79806375";

int wifiStatus; 
int connectSuccess = 0;

#define BOTtoken "5313442689:AAH7pUfZ2OQucXKQpR-YLTlRwVhl75OmPvA" 
#define CHAT_ID "652016456"
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);
X509List cert(TELEGRAM_CERTIFICATE_ROOT);

BlynkTimer timer;
// This function sends Arduino's uptime every second to Virtual Pin 2.


/*void myTimerEvent()
{
}*/


  int pinValue;
BLYNK_WRITE(V1)
{
   pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
}
  
void ir(){
      int ir_input=digitalRead(16);
   if(ir_input==0){
    digitalWrite(13,HIGH); 
     bot.sendMessage(CHAT_ID, "ALERT!!!SOMEBODY ENTERS THE PREMISES !!", "");
     }
   else{
    digitalWrite(13,LOW);
    }
  }
void setup()
{
  // Debug console
  Serial.begin(115200);
  WiFi.begin(ssid, pass); 
  pinMode(4,OUTPUT);  //BUZZER D2
  pinMode(5,INPUT); //MOTION_SENSOR D1
  pinMode(0,OUTPUT); //led motion
  pinMode(16,INPUT); //ir sensor
  pinMode(13,OUTPUT); // led ir
  Blynk.begin(auth, ssid, pass); 
  // Setup a function to be called every second
  //timer.setInterval(1000L, myTimerEvent);
   timer.setInterval(1000L, ir);
    secured_client.setTrustAnchors(&cert);
   settime();
}


void loop()
{
     wifiStatus = WiFi.status(); 
      if(connectSuccess == 0){ 
        Serial.println("System is not yet online...");
        }
      if(wifiStatus == WL_CONNECTED){ 
         Serial.println("Sytem is connected..");
        connectSuccess ++;
        } 
      else if(connectSuccess != 0){ 
        Serial.println("Connection interrupted..System is no longer connected to network..!!");
        digitalWrite(14,HIGH);
        } 
        
  
    int motion_input=digitalRead(5);
Serial.print(motion_input);

     if(motion_input==1)
      {
        if(pinValue==1){
         digitalWrite(4,HIGH);
         digitalWrite(0,HIGH);
         bot.sendMessage(CHAT_ID, "HIGH ALERT!!!INTRUDER DETECTED!!", "");
        }
        }
      
    if(pinValue==0){
          digitalWrite(4,LOW);
           digitalWrite(0,LOW);
          }



  Blynk.run();
  timer.run();


}

void settime(){
  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }  }
