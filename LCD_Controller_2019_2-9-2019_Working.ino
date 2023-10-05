/* Programmer: Louis Bersine 
 LCD Display Custom 
 2/8/2019 */

/*
 * The LCD I have uses 
 * 4-bits for E RS R/W
 * 4-bits for D7-D4
 * 
 */

#include <Wire.h>

byte startCmd = B00110000;
byte address = 63; //LCD address

char charBuffer[32]; 
String startupMsg = "Wristband Erase v0.1";
byte result = 0;
int reading = 0;
byte incomingByte = 0;

//setup command fund in HJ1602A manual 
void setup() {
  delay(15);
  Wire.begin();
  Serial.begin(9600);     // Start serial communication at 9600bps
  delay(15);
  
  //Initialize
  //Function Set
  dWrite(B00100000,2);
  delayMicroseconds(4001);
 
  //Function Set
  dWrite(B00100000,2);
  delayMicroseconds(105);

  //Function Set
  dWrite(B00100000,2);
  delayMicroseconds(40);

  //Function Set
  dWrite(B00100000,2);
  delayMicroseconds(40);

  //Function Set
  dWrite(B00101100,1);
  delayMicroseconds(40);

  //Display Off
  dWrite(B00001000,1);
  delayMicroseconds(40);

  //Clear
  dWrite(B00000001,1); 
  delay(2);

  //Entry Mode
  dWrite(B00000110,1);
  delayMicroseconds(40);

  //Display On
  dWrite(B00001111,1);
  delayMicroseconds(40);
  
  //Write Startup Message
  lcdWrite(startupMsg);ede

}
  
void loop() {
  
        // send data only when you receive data:
        if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
               if (incomingByte != 10){
                dWrite(incomingByte,3);
               }
        }
  
}

void lcdWrite(String message){

  //Write the message
   message.toCharArray(charBuffer,32);
   for(int i =0; i<=(message.length()-1); i++){  
             if(i == 16){
                       //Set to second line
                     dWrite(B10101000,1);
                     delay(5);
                  }
     Serial.print("Value Being Sent To Writer: ");
     Serial.print(byte(charBuffer[i]),BIN);
     Serial.print(" Which Is Letter ");
     Serial.print(charBuffer[i]);
     Serial.println();
     dWrite(byte(charBuffer[i]),3);
     delay(60);
   }
  
}

byte dWrite(byte data, int enable) {
  byte dResult = 0;
  byte dEn = B00000100;
  byte dRw = B00000010;
  byte dRs = B00000001;
  byte lcd = B00001000;

  byte cmdHigh = data & B11110000;
  byte cmdLow  = data << 4;


//toggle enabled
  if(enable == 1){
   Wire.beginTransmission(0x3F);
   Wire.write(cmdHigh|lcd|dEn);
   Wire.endTransmission();
   Serial.print("Command Byte 1: ");
   Serial.print(cmdHigh|lcd|dEn,BIN);
   Serial.println();
   delayMicroseconds(1); //1 microsecond between enable
   Wire.beginTransmission(0x3F);
   Wire.write((cmdHigh|lcd)&~dEn);
   Wire.endTransmission();
   Serial.print("Command Byte 2: ");
   Serial.print((cmdHigh|lcd)&~dEn,BIN);
   Serial.println();
   Wire.beginTransmission(0x3F);
   Wire.write(cmdLow|lcd|dEn);
   Wire.endTransmission();
   Serial.print("Command Byte 3: ");
   Serial.print(cmdLow|lcd|dEn,BIN);
   Serial.println();
   delayMicroseconds(1); // 1 microsecond between enable
   Wire.beginTransmission(0x3F);
   Wire.write((cmdLow|lcd)&~dEn);
   Wire.endTransmission();
   Serial.print("Command Byte 4: ");
   Serial.print((cmdLow|lcd)&~dEn,BIN);
   Serial.println();
   


  }


//toggle setup
  else if(enable == 2){
   Wire.beginTransmission(0x3F);
   Wire.write(cmdHigh|lcd|dEn);
   Wire.endTransmission();  
   Serial.print("Command Byte 1: ");
   Serial.print(cmdHigh|lcd|dEn,BIN);
   Serial.println();
   delayMicroseconds(1);
   Wire.beginTransmission(0x3F);
   Wire.write((cmdHigh|lcd)&~dEn);
   Wire.endTransmission();  
   Serial.print("Command Byte 2: ");
   Serial.print((cmdHigh|lcd)&~dEn,BIN);
   Serial.println();
    
  }

  else if(enable == 3){
    Wire.beginTransmission(0x3F);
    Wire.write(cmdHigh|lcd|dRs|dEn);
    Wire.endTransmission(0x3F);
    Serial.print("Command Byte 1: ");
    Serial.print(cmdHigh|lcd|dRs|dEn,BIN);
    Serial.println();
    delayMicroseconds(1);
    Wire.beginTransmission(0x3F);
    Wire.write((cmdHigh|lcd|dRs)&~dEn);
    Wire.endTransmission(0x3F);
    Serial.print("Command Byte 2: ");
    Serial.print((cmdHigh|lcd|dRs)&~dEn,BIN);
    Serial.println();
    Wire.beginTransmission(0x3F);
    Wire.write(cmdLow|lcd|dRs|dEn);
    Wire.endTransmission(0x3F);
    Serial.print("Command Byte 3: ");
    Serial.print(cmdLow|lcd|dRs|dEn,BIN);
    Serial.println();
    delayMicroseconds(1);
    Wire.beginTransmission(0x3F);
    Wire.write((cmdLow|lcd|dRs)&~dEn);
    Wire.endTransmission(0x3F);
    Serial.print("Command Byte 4: ");
    Serial.print((cmdLow|lcd|dRs)&~dEn,BIN);
    Serial.println();
    Serial.print("Printed Character: ");
    Serial.print(char(data));
    Serial.println();
   
  }

//toggle disabled
   else{
   Wire.beginTransmission(0x3F);
   Wire.write(cmdLow|lcd);
   Wire.endTransmission();
   Wire.beginTransmission(0x3F);
   Wire.write(cmdHigh|lcd);
   dResult =  Wire.endTransmission();  
   }

   return dResult;
   
   }
