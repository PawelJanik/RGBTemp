import android.content.Intent;
import android.os.Bundle;
import ketai.net.bluetooth.*;
import ketai.ui.*;
import ketai.net.*;
import android.bluetooth.BluetoothAdapter;
import android.view.KeyEvent;



BluetoothAdapter bluetooth = BluetoothAdapter.getDefaultAdapter();

KetaiBluetooth bt;
int info,pinfo,sendangle;
String cninfo = "";
String dateFromRGBTemp;
String tempInside = "";
String tempOutside = "";
String humidity = "";

int lastMillis = 0;



//To start BT on start********* 

void onCreate(Bundle savedInstanceState) 
{
 super.onCreate(savedInstanceState);
 bt = new KetaiBluetooth(this);
}

void onActivityResult(int requestCode, int resultCode, Intent data) 
{
  bt.onActivityResult(requestCode, resultCode, data);
}
//**********


//To select bluetooth device**********
void onKetaiListSelection(KetaiList klist)
{
 String selection = klist.getSelection();
 bt.connectToDeviceByName(selection);
 klist = null;
}
//**********

//***** To get data from blue tooth
void onBluetoothDataEvent(String who, byte[] data)
{
  if(data[0] == 'H')
  {
    dateFromRGBTemp = new String(data);
  }
  else
  {
    dateFromRGBTemp = dateFromRGBTemp + new String(data);
    //println(dateFromRGBTemp);
    
    humidity = new StringBuilder().append(dateFromRGBTemp.charAt(1)).append(dateFromRGBTemp.charAt(2)).append("%").toString();
    tempInside = new StringBuilder().append(dateFromRGBTemp.charAt(5)).append(dateFromRGBTemp.charAt(6)).append("*C").toString();
    if(dateFromRGBTemp.charAt(9) == '-')
      tempOutside = new StringBuilder().append(dateFromRGBTemp.charAt(9)).append(dateFromRGBTemp.charAt(10)).append(dateFromRGBTemp.charAt(11)).append("*C").toString();
    else
      tempOutside = new StringBuilder().append(dateFromRGBTemp.charAt(10)).append(dateFromRGBTemp.charAt(11)).append("*C").toString();
  }
  //byte[] send = {'D','\n'};
  //bt.broadcast(send);
}
//***************

//To get connection status
String getBluetoothInformation()
{
  String btInfo = "Connected to: ";

  ArrayList<String> devices = bt.getConnectedDeviceNames();
  for (String device: devices)
  {
    btInfo+= device+"\n";
  }
  return btInfo;
}
//**********

void setup()
{
   //size(600,600);
   size(displayWidth,displayHeight);
   bt.start(); //start listening for BT connections
   bt.getPairedDeviceNames();
   bt.connectToDeviceByName("RGBTemp");  
   
   delay(500);
   //byte[] send = {'L','1','2','3','\n'};
   //byte[] send = {'T','1','\n'};
   //println(send);
   //bt.broadcast(send);
   //byte[] send1 = {'D','\n'};
   //println(send1);
   //bt.broadcast(send1);
}

void draw()
{ 
  background(255);
  textSize(60);
  cninfo = getBluetoothInformation();
  fill(100,100,100);
  text(cninfo,5,60);
  
  
  text("Temperature outside:",width/2-300,230);
  textSize(150);
  text(tempOutside,width/2-200,400);
  textSize(60);
  text("Temperature inside:",width/2-300,630);
  textSize(150);
  text(tempInside,width/2-200,800);
  textSize(60);
  text("Humidity:",width/2-300,1050);
  textSize(150);
  text(humidity,width/2-200,1200);
  
  definebuttons();
  
  if(lastMillis + 2000 < millis())
  {
    byte[] send = {'D','\n'};
    bt.broadcast(send);
    lastMillis = millis();
    println(lastMillis);
    definebuttons();
  }
}


void definebuttons()
{
    if(mouseY >= 170 && mouseY <= 400 && mousePressed)
    {
       println("tempOut");
       byte[] send = {'T','2'};
       bt.broadcast(send);
    }
    if(mouseY >= 570 && mouseY <= 800 && mousePressed)
    {
        println("tempIn");
        byte[] send = {'T','1'};
        bt.broadcast(send);
    }
}