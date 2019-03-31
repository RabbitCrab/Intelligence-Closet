#include <DHT.h>
#include <Stepper.h>
#include "Ultrasonic.h"

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
  
  Serial.print("Temperature: ");
  tem=dht21_p2.readTemperature();
  Serial.println(tem);
  Serial.print("Humidity: ");
  hum=dht21_p2.readHumidity();
  Serial.println(hum);
      
  RangeInCentimeters = ultrasonic.MeasureInCentimeters(); // two measurements should keep an interval
  if((RangeInCentimeters<30)&&(beClose==false)){
    beClose=true;
    Serial.print("The state is Close: ");
    Serial.println((String)beClose);
    Serial.print("The distance to obstacles in front is: ");
    Serial.print(RangeInCentimeters);//0~400cm
    Serial.println(" cm");
    int light=0;
    light = analogRead(A0);
    Serial.print("Bright: ");
    Serial.println(light);
    Serial.print("Temperature: ");
    Serial.println(tem);
    Serial.print("Humidity: ");
    hum=dht21_p2.readHumidity();
    Serial.println(hum);
    
    double turnClothround = judgeCloth(tem,light);
    Serial.print(turnClothround);
    Serial.println(" Rounds");
    stepper2.step(numOfStep*turnClothround);     
    Serial.print("Now cloth index: ");
    Serial.println(nowClothIndex);
    
    double turnPantround = judgePant(tem,hum);    
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
    Serial.println("");
  }
  else if((RangeInCentimeters>30)&&(beClose==true)){
    beClose=false;
    Serial.print("The state is Close: ");
    Serial.println(beClose);
    Serial.print("The distance to obstacles in front is: ");
    Serial.print(RangeInCentimeters);//0~400cm
    Serial.println(" cm");
    Serial.println("");    
  }
  
  delay(200);
}
double judgePant(double tem, double hum){
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
    return (-0.25*subPant/3);
  }
  else{
    return (0.25*subPant);
  }
  
}
double judgeCloth(double tem, int light){
  int chooseClothIndex=0;
  Serial.println("Go in judgeCloth");
  if(tem>24.5){
    if(light>2000){
      chooseClothIndex=1;
    }
    else{
      chooseClothIndex=2;
    }
  }
  else{
    if(light>2000){
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
