#include "SPI.h"
#include "NRFLite.h"
#include <Servo.h>

//begin Radio Setup
const static uint8_t RADIO_ID = 1;       // Our radio's id.  The transmitter will send to this id.


struct RadioPacket // Any packet up to 32 bytes can be sent.
{
    uint8_t FromRadioId;
    uint32_t OnTimeMillis;
    uint32_t FailedTxCount;
};

NRFLite _radio;
RadioPacket _radioData;
//End radio Setup

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
      int CEpin = 49;
      int CSNpin = 53;
      int MOpin = 50;
      int MIpin = 51;
      int SCKpin = 52;

Servo pololu;

Servo leftMotor;
Servo rightMotor;

//FIXME ////////////////////////////////////////////////////////////
int packetIn[] = {0,0,0,0,0,0,0,0,0,0,};
int key = 0;
int modeSwitch = 1;
int leftJoy_Y = 2;
int leftJoy_X = 3;
int rightJoy_Y = 4;
int rightJoy_X = 5;
//FIXME /////////////////////////////////////////////////////////////



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
    
    if (!_radio.init(RADIO_ID, CEpin, CSNpin))
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
  while (_radio.hasData())
    {
        _radio.readData(&_radioData); // Note how '&' must be placed in front of the variable name.

        String msg = "Radio ";
        msg += _radioData.FromRadioId;
        msg += ", ";
        msg += _radioData.OnTimeMillis;
        msg += " ms, ";
        msg += _radioData.FailedTxCount;
        msg += " Failed TX";

        Serial.println(msg);

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
  
  
  
}