#include "SPI.h"
#include "NRFLite.h"
#include <Servo.h>

//begin Radio Setup
const static uint8_t RADIO_ID = 1;       // Our radio's id.  The transmitter will send to this id.


struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    int8_t JoystickX;
    int8_t JoystickY;

    int8_t thrust;
    int8_t twist;

    bool keyEnabled;
    bool isAuton;

    uint8_t safetyCounter;

    bool isJudsonSmart;

};

NRFLite radio;
RadioPacket radioData;
//End radio Setup

//Pins
  //Analog
    //In
      int leftAutonMotorPin = A0;
      int rightAutonMotorPin = A1;
      
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



    //Transceiver
      int CEpin = 49;
      int CSNpin = 53;
      int MOpin = 50;
      int MIpin = 51;
      int SCKpin = 52;

Servo pololu;

Servo leftMotor;
Servo rightMotor;

bool isFirstSafetyNumber = true;
uint8_t safetyChecker;
uint8_t safetyCounter = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

    // By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    // Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    // You can run the 'ChannelScanner' example to help select the best channel for your environment.
    // You can assign a different bitrate and channel as shown below.
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
    //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)
    
    if (!radio.init(RADIO_ID, CEpin, CSNpin))
    {
        Serial.println("Cannot communicate with radio");
        while (1); // Wait here forever.
    }

  analogReference(DEFAULT);

  pololu.attach(pololuPin);

  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);
}



bool isManual(){
  if(radioData.keyEnabled && !radioData.isAuton){
    return true;
  }
  else{
    return false;
  }
}

bool isAutonomous(){
  if(radioData.keyEnabled && radioData.isAuton){
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
  }
  else{
    motor.writeMicroseconds(1500);
  }
}

void tankSteering(){
  setPercent(leftMotor, radioData.thrust/255);
  setPercent(rightMotor, radioData.JoystickX);
  
}

void autonSteering(){
  setPercent(leftMotor, analogRead(leftAutonMotorPin));
  setPercent(rightMotor, analogRead(rightAutonMotorPin));
  
}

bool isSafe(){
  if(isFirstSafetyNumber){
    safetyChecker = radioData.safetyCounter;
    isFirstSafetyNumber = false;
  }

  if(safetyChecker = radioData.safetyCounter){
    safetyCounter++;
  }
  else{
    safetyCounter = 0;
  }

  if(safetyCounter > 10){
    return false;
    deactivatePololu();
  }
  else{
    return true;
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  

  while (radio.hasData()){

    radio.readData(&radioData); // Note how '&' must be placed in front of the variable name.

    Serial.println(radioData.isJudsonSmart);

    if(isManual() & isSafe()){

      activatePololu();

      tankSteering();

    }
    else if(isAutonomous() & isSafe()){

      activatePololu();

      autonSteering();

    }
    else{

      deactivatePololu();

    }
  }

  deactivatePololu();
  
}