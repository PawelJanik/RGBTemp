#include <Thread.h>

#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"

void RGBColor(int color, int brightness);
//void light();
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

int mode;
int setColor;
int setBrightness;

void setup() 
{
  Serial.begin(19200);

  sensors.begin();
  sensors.getAddress(insideThermometer, 0);
  sensors.setResolution(insideThermometer, 9);

  dht.begin();

  
  //myThread.onRun(sendData);
  //myThread.setInterval(2000);

  mode = 0; //mode 0 - dht, mode 1 - ds18b20, mode 2 - color 
}

void loop() 
{
  char buffor[8];
  while(Serial.available() > 0)
  {
      Serial.readBytesUntil('\n', buffor, 8);
      if(buffor[0] == 'T' && buffor[1] == '1')
      {
        mode = 0;
        RGBColor(100,255);
        delay(500);
        RGBColor(100,getBrightness());
        delay(500);
        RGBColor(100,255);
        delay(500);
      }
      else if(buffor[0] == 'T' && buffor[1] == '2')
      {
        mode = 1;  
        RGBColor(100,255);
        delay(500);
        RGBColor(100,getBrightness());
        delay(500);
        RGBColor(100,255);
        delay(500);
      }
      else if(buffor[0] == 'C')
      {
        mode = 2;
        setColor = (buffor[1]-'0')*1000 + (buffor[2]-'0')*100 + (buffor[3]-'0')*10 + (buffor[4]-'0');
        Serial.println(setColor);
      }
      else if(buffor[0] == 'L')
      {
        if(buffor[1] == 'A')
          mode = 3;
        else
        {
          mode = 4;
          setBrightness = (buffor[1]-'0')*100 + (buffor[2]-'0')*10 + (buffor[3]-'0');
          Serial.println(setBrightness);
        }
      }
      else if(buffor[0] == 'D')
      {
        sendData();
      }
  }
  
  //if(myThread.shouldRun())
  //  myThread.run();
    
  if(mode == 0)
  {
    RGBColor(map(constrain(getTemp1(),15.0,27.0), 15, 27, 250, 1499),getBrightness());   
  }
  else if(mode == 1)
  {
    RGBColor(map(constrain(getTemp2(),15.0,27.0), 15, 27, 250, 1499),getBrightness());   
  }
  else if(mode == 2)
    RGBColor(setColor,getBrightness());
  else if(mode == 3)
    RGBColor(1600,getBrightness());
  else if(mode == 4)
    RGBColor(1600,setBrightness);
}

void sendData()
{
    int h = getHumidity();
    int t1 = getTemp1();
    int t2 = getTemp2();
    Serial.print("H");
    Serial.print(h);
    Serial.print("T1");
    Serial.print(t1);
    Serial.print("T2");
    if(t2>0)
      Serial.print("+");
    else
      Serial.print("-");
    if(abs(t2) < 10)
      Serial.print("0");
    Serial.print(abs(t2));
    Serial.print("\n");
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

  if(color >= 0 && color < 250)
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
  else if (color >= 1250 && color <= 1500)
  {
    blue = 0;
    green = 250 - (color % 250);
    red = 250;  
  }
  else if (color >1500)
  {
    blue = 250;
    green = 250;
    red = 250;  
  }
    
  analogWrite(11,map(red, 0, 250, 0, 255 - brightness));
  analogWrite(10,map(green, 0, 250, 0, 255 - brightness));
  analogWrite(9,map(blue, 0, 250, 0, 255 - brightness));
}
