#include <DHT.h>
#include <Stepper.h>
#include "Ultrasonic.h"
#include <LRemote.h>

LRemoteSlider slider, slider2;
LRemoteLabel label;
LRemoteSwitch switchButton, button2;
LRemoteCircleButton bigButton;
double judgePant(int tem, int hum);
double judgeCloth(int tem, int light);
int light=0;
int guilight=0;
int guihum=0;
int guitem=0;
double tem=0,hum=0;
int nowClothIndex=0;
int nowPantIndex=0;
int numOfStep = 2048;//a round
int pinA2 = 9, pinB2 = 10, pinC2 = 11, pinD2 = 12;
int pinA = 4, pinB = 5, pinC = 6, pinD = 7;
int pinOUT = 13;
Stepper stepper(200, pinA, pinB, pinC, pinD);
Stepper stepper2(200, pinA2, pinB2, pinC2, pinD2);
DHT dht21_p2(8, DHT21);
Ultrasonic ultrasonic(2, 3);
bool beClose = true;
long RangeInCentimeters = 200;
int count=0;

void setup()
{ 
  Serial.begin(9600);
  
  Serial.println("Start configuring remote");

  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  
  LRemote.setName("LinkIt 7697");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 5);

  
  button2.setText("Get Close");
  button2.setPos(1, 1);
  button2.setSize(2, 1);
  button2.setColor(RC_PINK);
  LRemote.addControl(button2);

  
  slider2.setText("Humidity: Value Slider(50 ~ 100)");
  slider2.setPos(0, 3);
  slider2.setSize(3, 1);
  slider2.setColor(RC_ORANGE);
  slider2.setValueRange(50, 100, 0);
  LRemote.addControl(slider2);

  
  slider.setText("Temperature: Value Slider(23 ~ 26)");
  slider.setPos(0, 2);
  slider.setSize(3, 1);
  slider.setColor(RC_ORANGE);
  slider.setValueRange(23, 26, 0);
  LRemote.addControl(slider);

  
  label.setText("Intelligience Closet");
  label.setPos(0, 0);
  label.setSize(3, 1);
  label.setColor(RC_GREY);
  LRemote.addControl(label);

  
  switchButton.setText("Light");
  switchButton.setPos(0, 1);
  switchButton.setSize(1, 1);
  switchButton.setColor(RC_BLUE);
  LRemote.addControl(switchButton);

  LRemote.begin();
  Serial.println("begin() returned");

  dht21_p2.begin();
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinB2, OUTPUT);
  pinMode(pinC2, OUTPUT);
  pinMode(pinD2, OUTPUT);
  pinMode(pinOUT, OUTPUT);
  digitalWrite(pinOUT, LOW); 
  stepper.setSpeed(150);  
  stepper2.setSpeed(150);
}

void loop()
{  
  if(!LRemote.connected()) {
    Serial.println("waiting for connection");
    delay(1000);
  } else {
    delay(15);
  }
  
  LRemote.process();
  tem=dht21_p2.readTemperature();
  hum=dht21_p2.readHumidity();

      

  
  if((button2.isValueChanged()||slider.isValueChanged()||slider2.isValueChanged()||switchButton.isValueChanged())&&(button2.getValue()==1)){
    guitem=slider.getValue();
    Serial.print("Temperature: ");
    Serial.println(guitem);
    guilight=switchButton.getValue();
    Serial.print("Light: ");
    Serial.println(guilight);
    guihum=slider2.getValue();
    Serial.print("Humidity: ");
    Serial.println(guihum);  
    double turnClothround = judgeCloth(guitem,guilight);
    Serial.print(turnClothround);
    Serial.println(" Rounds");
    stepper2.step(numOfStep*turnClothround);     
    Serial.print("Now cloth index: ");
    Serial.println(nowClothIndex);
    
    double turnPantround = judgePant(guitem,guihum);    
    Serial.print(turnPantround);
    Serial.println(" Rounds");
    stepper.step(numOfStep*turnPantround);     
    Serial.print("Now pant index: ");
    Serial.println(nowPantIndex);
    count++;
    Serial.print("Count: ");
    Serial.println(count);
    if(count==3){
      count=0;
      digitalWrite(pinOUT, HIGH); //output signal to speaker
      delay(10);
      digitalWrite(pinOUT, LOW); 
    
  }
    
  delay(200);
}}
double judgePant(int tem, int hum)
{
  int choosePantIndex=0;
  Serial.println("Go in judgePant");
  if(tem>24.5){
    if(hum>75){
      choosePantIndex=0;
    }
    else{
      choosePantIndex=1;
    }
  }
  else{
    if(hum>75){
      choosePantIndex=2;
    }
    else{
      choosePantIndex=3;
    }
  }
  Serial.print("Now pant index: ");
  Serial.println(nowPantIndex);
  Serial.print("Choose pant index: ");
  Serial.println(choosePantIndex);
  int subPant = choosePantIndex-nowPantIndex;
  nowPantIndex = choosePantIndex;
  if((subPant==3)||(subPant==-3)){
    return (0.25*subPant/3);
  }
  else{
    return (-0.25*subPant);
  }
  
}
double judgeCloth(int tem, int light){
  int chooseClothIndex=0;
  Serial.println("Go in judgeCloth");
  if(tem>24.5){
    if(light==1){
      chooseClothIndex=1;
    }
    else{
      chooseClothIndex=2;
    }
  }
  else{
    if(light==1){
      chooseClothIndex=0;
    }
    else{
      chooseClothIndex=3;
    }
  }
  Serial.print("Now cloth index: ");
  Serial.println(nowClothIndex);
  Serial.print("Choose cloth index: ");
  Serial.println(chooseClothIndex);
  int subCloth = chooseClothIndex-nowClothIndex;
  nowClothIndex = chooseClothIndex;
  if((subCloth==3)||(subCloth==-3)){
    return (0.25*subCloth/3);
  }
  else{
    return (-0.25*subCloth);
  }
  
}
