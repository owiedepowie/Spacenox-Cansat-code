//BMP280
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
//SD
#include <SPI.h>
#include <SD.h>
//MICS-6814
#include <MiCS6814-I2C.h>
//APC220
#include <SoftwareSerial.h>
//BMP280
#define BMP280_I2C_ADDRESS 0x76
Adafruit_BMP280 bmp;
//SD
const int chipSelect = 10;
//MICS-6814
MiCS6814 sensor;
bool sensorConnected;
//mq-135
#define ANALOG_PIN 0
uint16_t gasVal;
boolean isgas = false;
String gas;
uint8_t preheatTime = 5;
//time
unsigned long Millitime;
int Minute = 0;
//APC220
SoftwareSerial mySerial(2, 3);


//var BMP280
int T = 0;
int Altitude = 0;
int Pa = 0;
//var MICS-6814
int CarbonMonoxide = 0;
int NitrogenDioxide = 0;
int Ammonia = 0;
int Propane = 0;
int Butane = 0;
int Methane = 0;
int Hydrogen = 0;
int Ethanol = 0;



void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  mySerial.println("The sensor is warming up...");

  while (preheatTime > 0) {
        mySerial.println(preheatTime);
        
        // Wait a minute
        delay(1000);

        preheatTime = preheatTime - 1;
    }
  //BMP280
  if (!bmp.begin(0x76)) {  
    mySerial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  mySerial.begin(9600);

  mySerial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    mySerial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  mySerial.println("card initialized.");

  sensorConnected = sensor.begin(0x18);
 
  if (sensorConnected == true) {
    // Print status message
    mySerial.println("Connected to MiCS-6814 sensor");

    // Turn heater element on
    sensor.powerOn();
    
    // Print header for live values
    
  } 
  else {
    // Print error message on failed connection
    mySerial.println("Couldn't connect to MiCS-6814 sensor");
  }
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  dataFile.println("Time\tTemperature\tPressure\tAltitude\tCO\tNO2\tNH3\tC3H8\tC4H10\tCH4\tH2\tC2H5OH\tGas Percentage");
  mySerial.println("Time\tTemperature\tPressure\tAltitude\tCO\tNO2\tNH3\tC3H8\tC4H10\tCH4\tH2\tC2H5OH\tGas Percentage");
  dataFile.close();
}

void loop() {
  //var time
  Millitime = millis();
  int Secondtime = Millitime / 1000;
  int Minute = Secondtime / 60;
  int Minutetime = Minute * 60;
  int Second = Secondtime - Minutetime;
  
  

//var BMP280
  T = bmp.readTemperature();
  Altitude = bmp.readAltitude(1013.25);
  Pa = bmp.readPressure();
//var MICS-6814
  CarbonMonoxide = sensor.measureCO();
  NitrogenDioxide = sensor.measureNO2();
  Ammonia = sensor.measureNH3();
  Propane = sensor.measureC3H8();
  Butane = sensor.measureC4H10();
  Methane = sensor.measureCH4();
  Hydrogen = sensor.measureH2();
  Ethanol = sensor.measureC2H5OH();
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
    
    if (sensorConnected) {
    // Print live values
    dataFile.print(CarbonMonoxide);
    dataFile.print("\t");
    dataFile.print(NitrogenDioxide);
    dataFile.print("\t");
    dataFile.print(Ammonia);
    dataFile.print("\t");
    dataFile.print(Propane);
    dataFile.print("\t");
    dataFile.print(Butane);
    dataFile.print("\t");
    dataFile.print(Methane);
    dataFile.print("\t");
    dataFile.print(Hydrogen);
    dataFile.print("\t");
    dataFile.print(Ethanol);
    dataFile.print("\t");

    mySerial.print(CarbonMonoxide);
    mySerial.print("\t");
    mySerial.print(NitrogenDioxide);
    mySerial.print("\t");
    mySerial.print(Ammonia);
    mySerial.print("\t");
    mySerial.print(Propane);
    mySerial.print("\t");
    mySerial.print(Butane);
    mySerial.print("\t");
    mySerial.print(Methane);
    mySerial.print("\t");
    mySerial.print(Hydrogen);
    mySerial.print("\t");
    mySerial.print(Ethanol);
    mySerial.print("\t");

    gasVal = analogRead(ANALOG_PIN);
    gasVal = map(gasVal, 0, 1023, 0, 100);
    dataFile.print(gasVal);
    dataFile.println("%");
    
    mySerial.print(gasVal);
    mySerial.println("%");
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    mySerial.println("error opening datalog.txt");
  }
  


  delay(1000);

}