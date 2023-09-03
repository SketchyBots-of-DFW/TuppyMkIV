#include "SPI.h"
#include "NRFLite.h"
#include <LCD_I2C.h>

//Radio Setup Begin
const static uint8_t RADIO_ID = 0;              // Controller radio's id.
const static uint8_t DESTINATION_RADIO_ID = 1;  // Id of the boat radio.


struct RadioPacket  // Any packet up to 32 bytes can be sent.
{
  int8_t joystickX;
  int8_t joystickY;

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

//LCD setup begin
LCD_I2C lcd(0x27, 16, 2);

uint8_t negPoint2[8] =
{
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001,
    0b00001
};
uint8_t negPoint4[8] =
{
    0b00011,
    0b00011,
    0b00011,
    0b00011,
    0b00011,
    0b00011,
    0b00011,
    0b00011
};
uint8_t negPoint6[8] =
{
    0b00111,
    0b00111,
    0b00111,
    0b00111,
    0b00111,
    0b00111,
    0b00111,
    0b00111
};
uint8_t negPoint8[8] =
{
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111,
    0b01111
};

uint8_t posPoint2[8] =
{
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000,
    0b10000
};
uint8_t posPoint4[8] =
{
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000,
    0b11000
};
uint8_t posPoint6[8] =
{
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100,
    0b11100
};
uint8_t posPoint8[8] =
{
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110,
    0b11110
};

uint8_t fullSpot[8] =
{
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};
uint8_t emptySpot[8] =
{
    0b11111,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b10001,
    0b11111
};

int p2 = 0;
int p4 = 1;
int p6 = 2;
int p8 = 3;

int np2 = 4;
int np4 = 5;
int np6 = 6;
int np8 = 7;

int full = 8;
int emptyBox = 9;
//LCD setup end

String rightString = " ";
String leftString = " ";

double currentRightValue = 0;
double currentLeftValue = 0;

int rightBoxes = 0;
int leftBoxes = 0;

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

  lcd.createChar(p2, posPoint2);
  lcd.createChar(p4, posPoint4);
  lcd.createChar(p6, posPoint6);
  lcd.createChar(p8, posPoint8);

  lcd.createChar(np2, negPoint2);
  lcd.createChar(np4, negPoint4);
  lcd.createChar(np6, negPoint6);
  lcd.createChar(np8, negPoint8);

  lcd.createChar(full, fullSpot);
}

//Updates the LCD and displaces the state of the robot
void updateLCD(){
  if(needToUpdateLCD != 0){
    lcd.clear();
    writeRightScreen();
    writeLeftScreen();
    //lcd.print(rightString);
    //lcd.setCursor(0,2);
    //lcd.print(leftString);
    //lcd.setCursor(0,1);
    needToUpdateLCD = false;
  }
}

void writeRightScreen(){
  rightBoxes = radioData.joystickX * 35;
  if(rightBoxes > 0){
    lcd.setCursor(1,1);
    lcd.print("       ");
    lcd.write(emptyBox);
    for(int i = rightBoxes / 5; i > 0; i--){
      lcd.write(full);
    }
    switch(rightBoxes % 5)
    {
      case 1:
        lcd.write(p2);
        break;
      case 2:
        lcd.write(p4);
        break;
      case 3:
        lcd.write(p6);
        break;
      case 4:
        lcd.write(p8);
        break;              
    }
  }
  else{
    for(int i = 7 - ((rightBoxes / 5) + 1); i > 0; i--){
      lcd.print(" ");
    }
    switch(rightBoxes % 5)
    {
      case 1:
        lcd.write(np2);
        break;
      case 2:
        lcd.write(np4);
        break;
      case 3:
        lcd.write(np6);
        break;
       case 4:
        lcd.write(np8);
        break;
    }
    for(int i = rightBoxes / 5; i > 0; i--){
      lcd.write(full);
    }
    lcd.write(emptyBox);
      
  }
}

void writeLeftScreen(){
  leftBoxes = radioData.thrust * 35;
  if(leftBoxes > 0){
    lcd.setCursor(2,1);
    lcd.print("       ");
    lcd.write(emptyBox);
    for(int i = leftBoxes / 5; i > 0; i--){
      lcd.write(full);
    }
    switch(leftBoxes % 5)
    {
      case 1:
        lcd.write(p2);
        break;
      case 2:
        lcd.write(p4);
        break;
      case 3:
        lcd.write(p6);
        break;
      case 4:
        lcd.write(p8);
        break;              
    }
  }
  else{
    for(int i = 7 - ((leftBoxes / 5) + 1); i > 0; i--){
      lcd.print(" ");
    }
    switch(leftBoxes % 5)
    {
      case 1:
        lcd.write(np2);
        break;
      case 2:
        lcd.write(np4);
        break;
      case 3:
        lcd.write(np6);
        break;
       case 4:
        lcd.write(np8);
        break;
    }
    for(int i = leftBoxes / 5; i > 0; i--){
      lcd.write(full);
    }
    lcd.write(emptyBox);
      
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  radioData.joystickX = analogRead(joystickXpin);//Reads and applies the joystick values to the data pack
  radioData.joystickY = analogRead(joystickYpin);//^
  radioData.twist = analogRead(twist);           //^
  radioData.thrust = analogRead(thrust);         //^

  radioData.keyEnabled = !digitalRead(keyPin);//Reads and applies the key and switch values to the data pack
  radioData.isAuton = digitalRead(autonSwitchPin);

  //checks if theres a change in the enabled state of the robot
  if (radioData.joystickX != currentRightValue && !rightString.equals("Enabled")){
    needToUpdateLCD = true;
    rightString = "Enabled";
  }
  else if (!radioData.keyEnabled && !rightString.equals("Disabled")){
    needToUpdateLCD = true;
    rightString = "Disabled";
  }
    
  //checks if theres a change in the manual/auton state of the robot
  if (radioData.isAuton && !leftString.equals("Auton")){
    needToUpdateLCD = true;
    leftString = "Auton";
  }
  else if (!radioData.isAuton && !leftString.equals("Manual")){
    needToUpdateLCD = true;
    leftString = "Manual";
  }

  updateLCD();

  Serial.print(rightString + " ");
  Serial.println(leftString);

  radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData), NRFLite::NO_ACK);  // Note how '&' must be placed in front of the variable name. Sends data packs
  
}