#include <DHT.h>
#define VIN 5
#define R 10000
#define RL10 50
#define Gamma 0.7
#define Suara A1
#define DHT_PIN 8
#define DHTTYPE DHT22

DHT dht(DHT_PIN, DHTTYPE);

int ldrRawData, suara;
float humidity, temperature;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  pinMode (Suara, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  String minta = "";
  while(Serial.available()>0)
  {
    minta += char(Serial.read());
  }

  minta.trim();

  if(minta == "Ya")
  {
    kirimdata();
  }

  minta = "";
  delay(1000);
}

void kirimdata(){
//Code Sensor MQ7
  float RS_gas = 0;
  float ratio = 0;
  float sensorValue = 0;
  float sensor_volt = 0;
  float R0 = 18686.00;
  sensorValue = analogRead(A2);
  sensor_volt = sensorValue/1024*5.0;
  RS_gas = (5.0-sensor_volt)/sensor_volt;
  ratio = RS_gas/R0; //Replace R0 with the value found using the sketch above
  float x = 1538.46 * ratio;
  float ppm = pow(x,-1.709);

//Code Sensor LDR
  float resistorVoltage, ldrVoltage;
  float ldrResistance;
  int ldrLux;
  ldrRawData = analogRead(A0);
  resistorVoltage = ldrRawData/1023.*VIN;
  ldrResistance = R * (1/((VIN/resistorVoltage)-1));
  ldrLux = pow(RL10*1e3*pow(10,Gamma)/ldrResistance,(1/Gamma));
  
//Code Sensor DHT22
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
//Code Sensor KY-307
  const int sampleWindow = 50;                              // Sample window width in mS (50 mS = 20Hz)
  unsigned int sample;
  unsigned long startMillis= millis();
  float peakToPeak = 0;
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(Suara);                    
      if (sample < 1024)                                 
      {
         if (sample > signalMax)
         {
            signalMax = sample;                           
         }
         else if (sample < signalMin)
         {
            signalMin = sample;                          
         }
      }
   }
   peakToPeak = signalMax - signalMin;                   
   int db = map(peakToPeak,20,900,40.0,90); 
   

  String datakirim = String(ppm) + "#" + String(humidity) + "#" + String(ldrLux) + "#" + String(db) + "#" + String(temperature);

  Serial.println(datakirim);
  
}


