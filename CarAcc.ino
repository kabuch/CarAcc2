#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;

File myFile;
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);

MPU9250_asukiaaa mySensor;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;

void setup()
{
  ss.begin(GPSBaud);
  Serial.begin(115200);
    if(!SD.begin(4)){
        Serial.println("Card Mount Failed");
        return; 
    }
    if(!SD.exists("/gps.txt")){
    
    File Gps = SD.open("/gps.txt",FILE_WRITE);
    Gps.println("Latitude;Longitude;Satellites;Altitude(M);Time;Speed;Date;\n");
    //Serial.println("New file created");
    Serial.print("Latitude;Longitude;Satellites;Altitude(M);Time;Speed;Date;");
    }
    if(!SD.exists("/acc.txt")){
    
    File Acc = SD.open("/acc.txt",FILE_WRITE);
    Acc.println("AcX;AcY;AcZ;AccSq;\n");
    //Serial.println("New file created");
    Serial.println("AcX;AcY;AcZ;AccSq;\n");
    }
    #ifdef _ESP32_HAL_I2C_H_
  // for esp32
  Wire.begin(SDA_PIN, SCL_PIN); //sda, scl
#else
  Wire.begin();
#endif
mySensor.setWire(&Wire);
 
mySensor.beginAccel();
}
void loop()
{
  File Gps = SD.open("/gps.txt", FILE_WRITE);
  File Acc = SD.open("/acc.txt", FILE_WRITE); 
  mySensor.accelUpdate();
  Serial.print(gps.location.lat(), 5);
  Serial.print(";");
  Serial.print(gps.location.lng(), 4);
  Serial.print(gps.satellites.value());
  Serial.print(";");  
  Serial.print(gps.altitude.feet() / 3.2808);
  Serial.print(";");
  Serial.print(gps.time.hour());
  Serial.print(":");
  Serial.print(gps.time.minute());
  Serial.print(":");
  Serial.print(gps.time.second());
  Serial.print(";");
  Serial.print(gps.speed.kmph());
  Serial.print(";");
  Serial.print(String(mySensor.accelX()));
  Serial.print(";");
  Serial.print(String(mySensor.accelY()));
  Serial.print(";");
  Serial.print(String(mySensor.accelZ()));
  Serial.print(";");
  Serial.print(mySensor.accelSqrt());
  Serial.println(";");
  
  //zapis do pliku
  Acc.print(String(mySensor.accelX()));
  Acc.print(";");
  Acc.print(String(mySensor.accelY()));
  Acc.print(";");
  Acc.print(String(mySensor.accelZ()));
  Acc.print(";");
  Acc.print(String(mySensor.accelSqrt()));
  Acc.println(";");
  
  Gps.print(gps.location.lat(), 5);
  Gps.print(";");
  Gps.print(gps.location.lng(), 4);
  Gps.print(";"); 
  Gps.print(gps.satellites.value());
  Gps.print(";");  
  Gps.print(gps.altitude.feet() / 3.2808);
  Gps.print(";");
  Gps.print(gps.time.hour());
  Gps.print(":");
  Gps.print(gps.time.minute());
  Gps.print(":");
  Gps.print(gps.time.second());
  Gps.print(";");
  Gps.print(gps.speed.kmph());
  Gps.print(";");
  Gps.print(gps.date.day());
  Gps.print("-");
  Gps.print(gps.date.month());
  Gps.print("-");
  Gps.print(gps.date.year());
  Gps.println(";");

  smartDelay(1000);                                      

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

static void smartDelay(unsigned long ms)                
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}