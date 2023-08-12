#include <rf24g.h>

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

RF24_G send;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  analogReference(DEFAULT);
  send = RF24_G(4, 7, 8);
}


void loop() {
  // put your main code here, to run repeatedly:
   uint8_t randNumber = random(300);
  
  int actual;
  
  packet senderPacketVar;

  packet receiverPacketVar;
  // set the destination of the packet to address 1
  sender.setAddress(1);
  // write the payload to the packet
  sender.addPayload(&randNumber, sizeof(int));
  // print out the original payload
  Serial.print("original number:");
  Serial.println(randNumber);
  // send the packet, if it is successful try to read back the packet
  if (send.write(&sender) == true) {
    // wait until a packet is received
    while (send.available() != true);
    
    send.read(&receiver);
    
    Serial.println(actual);
  
}