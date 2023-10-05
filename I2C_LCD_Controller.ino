/* Programmer: Louis Bersine 
 LCD Display Custom 
 2/8/2019 */

/*
 * The LCD I have uses 
 * 4-bits for E RS R/W
 * 4-bits for D7-D4
 * 
 */

 /*
  * This sketch was tested with Arduino Uno, there is no garantee this will work on other boards
  * -------Pinout-------
  * LCD             Arduino
  * VCC             5V
  * GND             GND
  * SCL             A5
  * SDA             A4
  */

#include <Wire.h>

byte startCmd = B00110000;
byte address = 63; //LCD address

char charBuffer[32]; 
String startupMsg = "Welcome";
byte result = 0;
int reading = 0;
byte incomingByte = 0;
bool cmdMode = false;

//setup command found in HJ1602A manual 
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
  lcdWrite(startupMsg);

}
  
void loop() {
  
        // send data only when you receive data:
        if (Serial.available() > 0) {
          //Awaiting a command to control LCD
          if(cmdMode){
              String cmd = Serial.readString();
              cmd.trim();
              Serial.println("Command received:" + cmd);

                //Clear the display
                if(cmd == "clr"){
                  clearScreen();
                }
                //New line
                else if (cmd == "nl"){
                  newLine();
                }
                //Cursor left
                else if (cmd == "cl"){
                  cursorLeft();
                }
                //Cursor Right
                else if (cmd == "cr"){
                  cursorRight();
                }
                //Display shift left
                else if (cmd == "dl"){
                  displayLeft();
                }

                //Shift display right
                else if (cmd == "dr"){
                  displayRight();
                }

                //Display home
                else if (cmd == "hm"){
                  displayHome();
                }

                //Cursor off
                else if (cmd == "coff"){
                  cursorOff();
                }

                //Cursor on/Turn off blinking
                else if (cmd == "con"){
                  cursorOn();
                }

                //Cursor on/Turn on blinking
                else if (cmd == "cblink"){
                  cursorBlink();
                }

              
              //Turn of command mode and clear incoming byte
              cmdMode = false;
              Serial.flush();
            }

            // read the incoming byte:
            incomingByte = Serial.read();
            // say what you got:
            Serial.print("I received: ");
            Serial.println(incomingByte, DEC);
            switch (incomingByte) {
              case 10:
                Serial.println("Serial message complete");
                cmdMode = false;
                Serial.flush();
                break;

              //Activate command mode with ! char
              case 33:
                cmdMode = true;
                Serial.println("Command mode activated");
                break;

              case 255:
                break;
              
              //Write character to LCD screen
              default:         
                dWrite(incomingByte,3);
                break;
            }

        }
  
}

//Function to call to clear the screen
//Use command !clr
void clearScreen(){
    //Clear
  dWrite(B00000001,1); 
  delay(2);

  //Entry Mode
  dWrite(B00000110,1);
  delayMicroseconds(40);
}

//Set to new line on display
void newLine(){
  dWrite(B10101000,1);
  delay(5);
}

//Previous line
void prevLine(){
  dWrite(B10101000,1);
  delay(5);
}

//Move cursor left
void cursorLeft(){
  dWrite(B00010000,1);
  delay(5);
}

//Move cursor right
void cursorRight(){
  dWrite(B00010100,1);
  delay(5);
}

//Shift all display text left
void displayLeft(){
  dWrite(B00011000,1);
  delay(5);
}

//Shift all display text right
void displayRight(){
  dWrite(B00011100,1);
  delay(5);
}

//Move display and cursor back to home position
void displayHome(){
  dWrite(B00000010,1);
  delay(5);
}

//Turn off cursor
void cursorOff(){
  dWrite(B00001100,1);
  delay(5);
}

//Turn on cursor
void cursorOn(){
  dWrite(B00001110,1);
  delay(5);
}

//Turn on cursor and have it blink
void cursorBlink(){
  dWrite(B00001111,1);
  delay(5);
}

void lcdWrite(String message){
  Serial.print("Writing to lcd");
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
  //Write characters
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
