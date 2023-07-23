#include <rf24g.h>
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

double packetIn[] = {0, 0, 0, 0, 0, 0, 0};
  int key = 0;
  int modeSwitch = 1;
  int launchMissiles = 2;
  int leftJoy_Y = 3;
  int rightJoy_Y = 4;
  int leftJoy_X = 5;
  int rightJoy_X = 6;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(DEFAULT);

  pololu.attach(pololuPin);

  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
}



bool isManual(){
  if(packetIn[key] == 0 && packetIn[modeSwitch] == 0){
    return true;
  }
  else{
    return false;
  }
}

bool isAutonomous(){
  if(packetIn[key] == 1 && packetIn[modeSwitch] == 1){
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
  setPercent(leftMotor, packetIn[leftJoy_Y]);
  setPercent(rightMotor, packetIn[rightJoy_Y]);
  
}



void loop() {
  // put your main code here, to run repeatedly:
  
  if(isManual()){

    activatePololu();

    tankSteering();

  }
  else if(isAutonomous()){

    activatePololu();

    //FIXME/////////////////////////////////////////////
    setPercent(leftMotor, 0.5);
    setPercent(rightMotor, -0.5);
    ////////////////////////////////////////////////////

  }
  else{

    deactivatePololu();

  }
}