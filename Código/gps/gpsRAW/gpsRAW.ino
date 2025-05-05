#include <SoftwareSerial.h>

#define TX 2  // GPS TX pin
#define RX 3  // GPS RX pin

SoftwareSerial gpsSerial(RX, TX);

String getField(String data, int index) {
  int commaCount = 0;
  int startIndex = 0;
  int endIndex = 0;

  for (int i = 0; i < data.length(); i++) {
    if (data[i] == ',') {
      commaCount++;
      if (commaCount == index) {
        startIndex = i + 1;
      } else if (commaCount == index + 1) {
        endIndex = i;
        break;
      }
    }
  }
  return data.substring(startIndex, endIndex);
}

float convertToDecimal(String coordinate, String direction) {
  if (coordinate.length() < 4) return 0;

  int degreeLength = (coordinate.length() > 7) ? 3 : 2; 
  float degrees = coordinate.substring(0, degreeLength).toFloat();
  float minutes = coordinate.substring(degreeLength).toFloat() / 60.0;
  float decimal = degrees + minutes;

  if (direction == "S" || direction == "W") decimal *= -1;

  return decimal;
}

void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  Serial.println("espera para conectar");
}

void loop() {
  String gpsData = "";
  // dando valores a variavel c ate que a mensagem $GPGGA seja toda lida
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    gpsData += c;
    if (c == '\n') break; 
  }

  if (gpsData.indexOf("$GPGGA") == -1) {
    Serial.println("mensagem inválida");
    delay(1000);
    return;
  }

  String latitudeStr = getField(gpsData, 2);
  String latDir = getField(gpsData, 3);
  String longitudeStr = getField(gpsData, 4);
  String lonDir = getField(gpsData, 5);
  String altitude = getField(gpsData, 9);

  if (latitudeStr == "" || longitudeStr == "" || altitude == "") {
    Serial.println("espera de sinal");
    delay(1000);
    return;
  }

  float latitude = convertToDecimal(latitudeStr, latDir);
  float longitude = convertToDecimal(longitudeStr, lonDir);

  Serial.print("Lat: "); Serial.println(latitude, 6);
  Serial.print("Long: "); Serial.println(longitude, 6);
  Serial.print("Alt: "); Serial.print(altitude); Serial.println(" m");
  Serial.println("-----------------------");

  delay(1000);
}