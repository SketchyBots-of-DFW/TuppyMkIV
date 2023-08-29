#include "SPI.h"
#include "NRFLite.h"
#include <LCD_I2C.h>

//Radio Setup Begin
const static uint8_t RADIO_ID = 0;              // Controller radio's id.
const static uint8_t DESTINATION_RADIO_ID = 1;  // Id of the boat radio.


struct RadioPacket  // Any packet up to 32 bytes can be sent.
{
  int8_t JoystickX;
  int8_t JoystickY;

  int8_t thrust;
  int8_t twist;

  bool keyEnabled;
  bool isAuton;
};

NRFLite radio;
RadioPacket radioData;
//Radio Setup End

//Pins
  //Analog
    //In
      int joystickXpin = A0;
      int joystickYpin = A1;
      int twist = A2;
      int thrust = A3;

    //Out
      int lcdSDA = A4;
      int lcdSCL = A5;

  //Digital
    //In
      int keyPin = 2;
      int autonSwitchPin = 3;

    //Out

//Transmitter
  uint8_t CEpin = 9;
  uint8_t CSNpin = 10;
  int MOpin = 11;
  int MIpin = 12;
  int SCKpin = 12;

LCD_I2C lcd(0x27, 16, 2);

String enabled = " ";
String manual = " ";

bool needToUpdateLCD = true;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(keyPin, INPUT_PULLUP);
  pinMode(autonSwitchPin, INPUT_PULLUP);

  // By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
  // Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
  // You can run the 'ChannelScanner' example to help select the best channel for your environment.
  // You can assign a different bitrate and channel as shown below.
  //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
  //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE1MBPS, 75)
  //   _radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN, NRFLite::BITRATE250KBPS, 0)

  if (!radio.init(RADIO_ID, CEpin, CSNpin)) {
    Serial.println("Cannot communicate with radio");
    while (1)
      ;  // Wait here forever.
  }

  lcd.begin();
  lcd.backlight();
}

//Updates the LCD and displaces the state of the robot
void updateLCD(){
  if(needToUpdateLCD != 0){
    lcd.clear();
    lcd.print(enabled);
    lcd.setCursor(0,2);
    lcd.print(manual);
    lcd.setCursor(0,1);
    needToUpdateLCD = false;
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  radioData.JoystickX = analogRead(joystickXpin);//Reads and applies the joystick values to the data pack
  radioData.JoystickY = analogRead(joystickYpin);//^
  radioData.twist = analogRead(twist);           //^
  radioData.thrust = analogRead(thrust);         //^

  radioData.keyEnabled = !digitalRead(keyPin);//Reads and applies the key and switch values to the data pack
  radioData.isAuton = digitalRead(autonSwitchPin);

  //checks if theres a change in the enabled state of the robot
  if (radioData.keyEnabled && !enabled.equals("Enabled")){
    needToUpdateLCD = true;
    enabled = "Enabled";
  }
  else if (!radioData.keyEnabled && !enabled.equals("Disabled")){
    needToUpdateLCD = true;
    enabled = "Disabled";
  }
    
  //checks if theres a change in the manual/auton state of the robot
  if (radioData.isAuton && !manual.equals("Auton")){
    needToUpdateLCD = true;
    manual = "Auton";
  }
  else if (!radioData.isAuton && !manual.equals("Manual")){
    needToUpdateLCD = true;
    manual = "Manual";
  }

  updateLCD();

  radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData), NRFLite::NO_ACK);  // Note how '&' must be placed in front of the variable name. Sends data packs
  
}