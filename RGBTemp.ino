#include <Thread.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

void RGBColor(int color, int brightness);
float getTemp1();
float getTemp2();
float getHumidity();
int getBrightness();

OneWire oneWire(2);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;
DHT dht(3, DHT11);

Thread myThread = Thread();
void sendData();

int ledMode;
int brightness;
bool BrightnessMode;
int tempInMin;
int tempInMax;
int tempoutMin;
int tempOutMax;

void setup() 
{
  Serial.begin(19200);

  sensors.begin();
  sensors.getAddress(insideThermometer, 0);
  sensors.setResolution(insideThermometer, 9);

  dht.begin();

  
  myThread.onRun(sendData);
  myThread.setInterval(2000);
}

void loop() 
{
  //for(int i = 0; i<1500; i++)
  //{
  //  RGBColor(i,220);
  //  delay(5);
  //}

  if(myThread.shouldRun())
    myThread.run();
  
  RGBColor(map(getTemp2(), 17, 27, 500, 1500),getBrightness());
}

void sendData()
{
    Serial.print("Humidity: ");
    Serial.print(getHumidity());
    Serial.print(" %\tTemperature: ");
    Serial.print(getTemp1());
    Serial.print("*C \tTemperature:");
    Serial.print(getTemp2());
    Serial.print("*C \n");
}

float getTemp1()
{
  return dht.readTemperature();
}

float getTemp2()
{
    sensors.requestTemperatures();
  return sensors.getTempC(insideThermometer);
}

float getHumidity()
{
  return dht.readHumidity();
}

int getBrightness()
{
  return 255 - map(analogRead(A5), 0, 1023, 3, 255);
}

void RGBColor(int color, int brightness)
{
  int red, green, blue;

  if(color < 250)
  {
    blue = color;
    green = 0;
    red = 250;  
  }
  else if(color >= 250 && color < 500)
  {
    blue = 250;
    green = 0;
    red = 250 - (color % 250);;  
  }
  else if(color >= 500 && color < 750)
  {
    blue = 250;
    green = color % 250;
    red = 0;  
  }
  else if (color >= 750 && color < 1000)
  {
    blue = 250 - (color % 250);
    green = 250;
    red = 0;  
  }
  else if (color >= 1000 && color < 1250)
  {
    blue = 0;
    green = 250;
    red = color % 250;  
  }
  else if (color >= 1250 && color < 1500)
  {
    blue = 0;
    green = 250 - (color % 250);
    red = 250;  
  }
  else
    red = 250;
    
  analogWrite(11,map(red, 0, 250, 0, 255 - brightness));
  analogWrite(10,map(green, 0, 250, 0, 255 - brightness));
  analogWrite(9,map(blue, 0, 250, 0, 255 - brightness));
}
