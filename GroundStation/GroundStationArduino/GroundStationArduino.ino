#include <LCD_I2C.h>

//Pins
  //Analog
    //In

    //Out
      int lcdSDA = A4;
      int lcdSCL = A5;

  //Digital
    //In

    //Out
      



    //Transmitter
      int outD0 = 4;
      int outD1 = 5;
      int outD2 = 6;
      int outD3 = 7;

    //Receiver
      int inD0 = 4;
      int inD1 = 5;
      int inD2 = 6;
      int inD3 = 7;

LCD_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  lcd.begin();
  lcd.backlight();
}

void updateValues(){
  if(digitalRead(inD0) == 0){
    //receiving from boat
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();


}
