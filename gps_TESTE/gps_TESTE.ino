#include <NMEAGPS.h>
#include <SoftwareSerial.h>

#define RXPin 3
#define TXPin 2
#define gpsB 9600

SoftwareSerial gpsSerial(RXPin, TXPin);
NMEAGPS gps;
gps_fix fix; //valor do gps fixo a um satelite (conectado)

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(gpsB);
  Serial.println("espera de conectar...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    if (gps.decode(gpsSerial.read())) {  //recebe uma mensagem inteira do gps ()
      fix = gps.fix();
      
      if (fix.valid.location) {
        Serial.print("Latitude: "); Serial.println(fix.latitude(), 6);
        Serial.print("Longitude: "); Serial.println(fix.longitude(), 6);
      } else {
        Serial.println("vals invs coord");
      }

      if (fix.valid.altitude) {
        Serial.print("Altitude: "); Serial.print(fix.altitude()); Serial.println(" m");
      } else {
        Serial.println("val invs alt");
      }

      //Serial.print("Satellites: "); Serial.println(fix.satellites);
    }
    delay(1000);
  }
}
