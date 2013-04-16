/*
 Circadian Cylces - using Blink without Delay and SD library to turn a light off/on every 12 hours, for the purposes
 of growing C. Vulgaris Algae
 
 hardware: 
 microsd shield: https://www.sparkfun.com/tutorials/172#size
   --chip select pin = 8
 chronodot rtc: http://www.adafruit.com/products/255
  --chronodot wiring: 5v,gnd, analog pin 4 to SDA. analog pin 5 to SCL. for I2C comm
 arduino uno
 powerswitch tail // power to pin 7, ground to ground
 4.16.13
 
	 
 */

#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

File myFile;
RTC_DS1307 RTC;


const int ledPin =  7; //output to powerswitch tail
const int chipSelect = 8;

int ledState = LOW; //light off            
long previousMillis = 0;  
long interval = 3600000 * 12; //3,600,000 ms * 12 hrs
//3600 sec in 1 min
//3,600,000 ms = 1 hr

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.print("Initializing SD card...");

  pinMode(ledPin, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
  }
  RTC.adjust(DateTime(__DATE__, __TIME__));
  //readData();
}

void loop()
{
  
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW)
      ledState = HIGH;
    else
      ledState = LOW;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
  logData();
}

void logData()
{
  DateTime now = RTC.now();
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print(ledState);
    myFile.print(',');
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print('/');
    myFile.print(now.hour(), DEC);
    myFile.print('/');
    myFile.print(now.minute(), DEC);
    myFile.print('/');
    myFile.print(now.second(), DEC);
    myFile.print('/');
    myFile.println();
    myFile.close();
    Serial.println("done.");
  } 
  else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void readData()
{
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } 
  else {
    Serial.println("error opening test.txt");
  } 

}











