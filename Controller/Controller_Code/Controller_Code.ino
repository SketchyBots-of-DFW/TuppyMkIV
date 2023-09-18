#include "SPI.h"
#include "NRFLite.h"
#include <LCD_I2C.h>
#include <BetterJoystick.h>

//Pins
  //Analog
    //In
      int joystickXpin = A0;
      int joystickYpin = A1;
      int twist = A2;
      int thrust = A3;

    //Out
      
  //Digital
    //In
      int keyPin = 2;
      int autonSwitchPin = 3;

    //Out
      int lcdSDA = A4;
      int lcdSCL = A5;

//Transmitter
  uint8_t CEpin = 9;
  uint8_t CSNpin = 10;
  int MOpin = 11;
  int MIpin = 12;
  int SCKpin = 13;

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
//Radio setup end

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

uint8_t p2 = 0;
uint8_t p6 = 2;

uint8_t np2 = 4;
uint8_t np6 = 6;

uint8_t full = 8;
uint8_t emptyBox = 9;
//LCD setup end

Joystick leftJoystick(joystickXpin, joystickYpin, 4);
Joystick rightJoystick(twist, thrust, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(keyPin, INPUT_PULLUP);
  pinMode(autonSwitchPin, INPUT_PULLUP);

  if (!radio.init(RADIO_ID, CEpin, CSNpin, NRFLite::BITRATE2MBPS, 100)) {
    Serial.println("Cannot communicate with radio");
    while (1)
      ;  // Wait here forever.
  }

  lcd.begin();
  lcd.backlight();

  lcd.createChar(p2, posPoint2);
  lcd.createChar(p6, posPoint6);

  lcd.createChar(np2, negPoint2);
  lcd.createChar(np6, negPoint6);

  lcd.createChar(full, fullSpot);
  lcd.createChar(emptyBox, emptySpot);

}

//Updates the LCD and displays the joystick values
void updateLCD(){
  if(millis() % 3 == 0){
    lcd.clear();
    writeScreen(radioData.joystickY, 0);
    writeScreen(radioData.thrust, 1); 
  }
}

void writeScreen(int joystickVal, int row){
  int boxes = ((joystickVal) / 128.0) * 35;
  int pos;

  lcd.setCursor(8,row);
  lcd.write(emptyBox);
  lcd.setCursor(0,row);

  if(row == 0){
    lcd.print("R");
  }
  else{
    lcd.print("L");
  }

  if(boxes > 0){  
    lcd.setCursor(9,row);
    for(int i = boxes / 5; i > 0; i--){
      lcd.write(full);
    }
    switch(boxes % 5){
      case 1:
        lcd.write(p2);
        break;
      case 2:
        lcd.write(p2);
        break;
      case 3:
        lcd.write(p6);
        break;
      case 4:
        lcd.write(p6);
        break;              
    }
  }
  else if(boxes < 0){
    pos = 7;
    for(int i = boxes / 5; i < 0; i++){
      lcd.setCursor(pos,row);
      lcd.write(full);
      pos--;
    }
    lcd.setCursor(pos,row);
    switch(boxes % 5){
      case -1:
        lcd.write(np2);
        break;
      case -2:
        lcd.write(np2);
        break;
      case -3:
        lcd.write(np6);
        break;
       case -4:
        lcd.write(np6);
        break;
    } 
  }
}

void loop() {
  // put your main code here, to run repeatedly:

  radioData.joystickX = rightJoystick.x(0,255) - 128;//RIGHT Reads and applies the joystick values to the data pack
  radioData.joystickY = rightJoystick.y(0,255) - 128;//RIGHT ^
  radioData.twist = leftJoystick.x(0,255) - 128;     //LEFT  ^
  radioData.thrust = leftJoystick.y(0,255) - 128;    //LEFT  ^

  radioData.keyEnabled = !digitalRead(keyPin);    //Reads and applies the key value to the data pack
  radioData.isAuton = digitalRead(autonSwitchPin);//^ for the switch

  updateLCD();
  
  radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData), NRFLite::NO_ACK);  // Note how '&' must be placed in front of the variable name. Sends data packs
  
}