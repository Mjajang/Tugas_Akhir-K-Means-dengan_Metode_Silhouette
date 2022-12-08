#include <SoftwareSerial.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>

SoftwareSerial DataSerial(12, 13);

unsigned long previousMillis = 0;
const long interval = 3000;

String arrData[5];

#define USERNAME "mjajang"
#define DEVICE_ID "Tugas_Akhir"
#define DEVICE_CREDENTIAL "VwlKNW!-0y+18w1$"

#define LED_PIN 4

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

const char* ssid = "vivo1901";
const char* password = "singberkah";

int ldr, suara;
float temperature, humidity, gas;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DataSerial.begin(9600);


  WiFi.begin(ssid, password);

  thing.add_wifi(ssid, password);

  thing["Data"] >> [](pson & out){
    out["gas"] = gas;
    out["humidity"] = humidity;
    out["ldr"] = ldr;
    out["suara"] = suara;
    out["temperature"] = temperature;
  };
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    String data = "";
    while(DataSerial.available()>0)
    {
      data += char(DataSerial.read());
    }

    data.trim();
    
    if(data != "")
    {
      int index = 0;
      for(int i=0; i<= data.length(); i++)
      {
        char delimiter = '#';
        if(data[i] != delimiter)
          arrData[index] += data[i];
        else
          index++;
      }

      if(index == 4)
      {
        Serial.println("Gas         : " + arrData[0]);
        Serial.println("Humidity    : " + arrData[1]);
        Serial.println("LDR         : " + arrData[2]);
        Serial.println("Suara       : " + arrData[3]);
        Serial.println("Temperature : " + arrData[4]);
        Serial.println();
      }
      
 
      gas         = arrData[0].toFloat();
      humidity    = arrData[1].toFloat();
      ldr         = arrData[2].toInt();
      suara       = arrData[3].toInt();
      temperature = arrData[4].toFloat();
 

     
      
      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";
      arrData[3] = "";
      arrData[4] = "";

       thing.handle();
     
    }

    DataSerial.println("Ya");
  }
}
