#define GAS_SENSOR    // for testing decreasing required memory space
//BMP280
#include <Adafruit_BMP280.h>
//SD
#include <SD.h>
//MICS-6814
#include "GasBreakout.h"
//APC220
#include <SoftwareSerial.h>
//BMP280
#define BMP280_I2C_ADDRESS 0x76
Adafruit_BMP280 bmp;
//SD
const int chipSelect = 10;
//MICS-6814
#ifdef GAS_SENSOR
GasBreakout gas(Wire, 0x18);
#endif
//APC220
SoftwareSerial mySerial(2, 3);

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  mySerial.begin(9600);
  
  //BMP280
  if (!bmp.begin(0x76)) {  
    mySerial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  
  //MICS-6814
  Wire.begin();
  
#ifdef GAS_SENSOR
  if(!gas.initialise()){
      Serial.println("MICS6814 - Not Initialised");
      while(1);
  }
#endif
  Serial.println("MICS6814 - Initialised");
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");


  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tTemperature\tPressure\tAltitude");
  Serial.println("Time\tTemperature\tPressure\tAltitude");
  dataFile.close();
}

void loop() {
  //var time
  unsigned long Millitime;
  Millitime = millis();
  int Secondtime = Millitime / 1000;
  int Minute = Secondtime / 60;
  int Minutetime = Minute * 60;
  int Second = Secondtime - Minutetime;
  
  

//var BMP280
  float T = 0;
  float Altitude = 0;
  float Pa = 0;

  T = bmp.readTemperature();
  Altitude = bmp.readAltitude(1013.25);
  Pa = bmp.readPressure();
//var MICS-6814

  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
  //if file open print variables to file
  if (dataFile) {
    dataFile.print("00:");
    mySerial.print("00:");
    if (Minute < 10) {
      dataFile.print("0"); dataFile.print(Minute); dataFile.print(":");
      mySerial.print("0"); mySerial.print(Minute); mySerial.print(":");
    }
    else {
      dataFile.print(Minute); dataFile.print(":");
      mySerial.print(Minute); mySerial.print(":");
    }
    if (Second < 10) {
      dataFile.print("0"); dataFile.print(Second);
      mySerial.print("0"); mySerial.println(Second);
    }
    else {
      dataFile.print(Second);
      mySerial.println(Second);
    }
    dataFile.print("\t");
    dataFile.print (T); dataFile.print(" *C\t");
    dataFile.print(Pa); dataFile.print(" Pa\t");
    dataFile.print(Altitude); dataFile.print(" m\t\t");
    
    mySerial.print("\t");
    mySerial.print (T); mySerial.print(" *C\t");
    mySerial.print(Pa); mySerial.print(" Pa\t");
    mySerial.print(Altitude); mySerial.print(" m\t");

#ifdef GAS_SENSOR
    GasBreakout::Reading reading;
    reading = gas.readAll();

    Serial.print("NH3: ");
    Serial.println(reading.nh3);
    Serial.print("Ox: ");
    Serial.println(reading.oxidising);
    Serial.println("");
#endif    
    
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    mySerial.println("error opening datalog.txt");
  }
  


  delay(1000);

}