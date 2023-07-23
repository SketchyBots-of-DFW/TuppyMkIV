#include <RF24G.h>

//Pins
  //Analog
    //In
      
    //Out

  //Digital
    //In

    //Out
      



    //Transceiver
      int CEpin = 7;
      int CSNpin = 8;
      int MOpin = 11;
      int MIpin = 12;
      int SCKpin = 13;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(DEFAULT);
}


void loop() {
  // put your main code here, to run repeatedly:
  
}