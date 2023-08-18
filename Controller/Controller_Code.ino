#include "SPI.h"
#include "NRFLite.h"
#include <LCD_I2C.h>

//Radio Setup Begin
const static uint8_t RADIO_ID = 0;             // Controller radio's id.
const static uint8_t DESTINATION_RADIO_ID = 1; // Id of the boat radio.

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
//Radio Setup End

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
      int keyPin = 4;
      int autonSwitchPin = 5;

    //Out
      



    //Transmitter
      uint8_t CEpin = 9;
      uint8_t CSNpin = 10;
      int MOpin = 11;
      int MIpin = 12;
      int SCKpin = 12;

uint8_t safetyCounter = 0;

LCD_I2C lcd(0x27, 16, 2);

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

    lcd.begin();
    lcd.backlight();

}


void loop() {
  // put your main code here, to run repeatedly:
  safetyCounter++;
  lcd.clear();
  // By default, 'send' transmits data and waits for an acknowledgement.  If no acknowledgement is received,
  // it will try again up to 16 times.  This retry logic is built into the radio hardware itself, so it is very fast.
  // You can also perform a NO_ACK send that does not request an acknowledgement.  In this situation, the data packet
  // will only be transmitted a single time and there is no verification of delivery.  So NO_ACK sends are suited for
  // situations where performance is more important than reliability.
  //   _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::REQUIRE_ACK) // THE DEFAULT
  //   _radio.send(DESTINATION_RADIO_ID, &_radioData, sizeof(_radioData), NRFLite::NO_ACK)
  radioData.JoystickX = analogRead(joystickXpin);
  radioData.JoystickY = analogRead(joystickYpin);

  radioData.twist = analogRead(twist);
  radioData.thrust = analogRead(thrust);

  radioData.keyEnabled = digitalRead(keyPin);
  radioData.isAuton = digitalRead(autonSwitchPin);

  radioData.safetyCounter = safetyCounter;

  radioData.isJudsonSmart = true;

  if(digitalRead(keyPin) == 1){
    lcd.println("ENABLED");
  }
  else{
    lcd.println("DISABLED");
  }

  radio.send(DESTINATION_RADIO_ID, &radioData, sizeof(radioData)); // Note how '&' must be placed in front of the variable name.
   
}