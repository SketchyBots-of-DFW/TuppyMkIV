#include <RF24G.h>
#include <Servo.h>

//Pins
  //Analog
    //In
      
    //Out

  //Digital
    //In

    //Out
      int LEDbuzzerPin = 2;
      int LEDgreenPin = 3;
      int LEDyellowPin = 4;
      int LEDredPin = 5;
      
      int leftMotorPin = 22;
                          
      int rightMotorPin = 24;
                          
      int pololuPin = 26;
                          

      int buttonPin = 39;



    //Transceiver
      int CEpin = 7;
      int CSNpin = 8;
      int MOpin = 11;
      int MIpin = 12;
      int SCKpin = 13;

Servo pololu;

Servo leftMotor;
Servo rightMotor;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(DEFAULT);

  //pololu.attach(pololuPin);

  //leftMotor.attach(leftMotorPin);
  //rightMotor.attach(rightMotorPin);

  //receiverR.setMinMax(minsMaxsR);
  receiverL.setMinMax(minsMaxsL);
}



bool isManual(){
  if(receiverL.getRaw(lCH4key) > middleValue && receiverL.getRaw(lCH1toggleswitch) < middleValue){
    return true;
  }
  else{
    return false;
  }
}

bool isAutonomous(){
  if(receiverL.getRaw(lCH4key) > middleValue && receiverL.getRaw(lCH1toggleswitch) > middleValue){
    return true;
  }
  else{
    return false;
  }
}

void activatePololu(){
  pololu.writeMicroseconds(2000);
}

void deactivatePololu(){
  pololu.writeMicroseconds(0);
}

void setPercent(Servo motor, double percentage){
  if(percentage > 10 || percentage < -10){
    motor.writeMicroseconds((percentage/100 * 400) + 1500);
    //Serial.println((percentage/100 * 400) + 1500);
  }
  else{
    motor.writeMicroseconds(1500);
    //Serial.println(1500);
  }
}

void tankSteering(){
  int leftPwr = receiverL.getMap(1) - 28;
  
}



void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(receiverR.getRaw(2));
  //Serial.println(receiverR.getMap(2));
  
  //if(/*isManual()*/true){

    //activatePololu();

    //setPercent(leftMotor, receiver.getMap(10));
    //setPercent(rightMotor, receiver.getMap(11));

    tankSteering();

  /*}
  else if(isAutonomous()){

    activatePololu();

    /////////////////////////////////////////////
    setPercent(leftMotor, 0.5);
    setPercent(rightMotor, -0.5);
    /////////////////////////////////////////////

  }
  else{

    deactivatePololu();

  }*/
}