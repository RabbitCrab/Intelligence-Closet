#include <Stepper.h>
int numOfStep = 2048;//a round
int pinA2 = 9, pinB2 = 10, pinC2 = 11, pinD2 = 12;
int pinA = 4, pinB = 5, pinC = 6, pinD = 7;
Stepper stepper(200, pinA, pinB, pinC, pinD);
Stepper stepper2(200, pinA2, pinB2, pinC2, pinD2);
void setup() {
  // put your setup code here, to run once:
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  pinMode(pinD, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinB2, OUTPUT);
  pinMode(pinC2, OUTPUT);
  pinMode(pinD2, OUTPUT);
  stepper.setSpeed(150);  
  stepper2.setSpeed(150);
  stepper2.step(numOfStep*-0.25);//cloth turn
  stepper.step(numOfStep*(0.25));//pant turn
}

void loop() {
  // put your main code here, to run repeatedly:

}
