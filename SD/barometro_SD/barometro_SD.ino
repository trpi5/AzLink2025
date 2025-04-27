// barometro

float pressaoNoMar = 1037;// pressao atmosferica no nivel do mar, precisa de ser alterado para calibrar
//https://meteologix.com/pt/observations/7c7e4f0c9abfb092042ebd513fcf12a1/pressure-qnh/20241016-1600z.html


#include <SdFat.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"//versao 2.1.0

#define BMP_SCK 13  //stk
#define BMP_MOSI 12 //sdi
#define BMP_CS 11  //csb
#define BMP_MISO 10 // sdo

#define  SD_SCK  8 //clk
#define  SD_MOSI  7 //mosi
#define  SD_CS     6//cs
#define  SD_MISO  9 //miso

#define SEALEVELPRESSURE_HPA (pressaoNoMar)
int num = 0;

// Se der algum erro, lembrar mudar "SPI_DRIVER_SELECT" para 2 na library, no ficheiro SdFatConfig.h
SoftSpiDriver<SD_MISO, SD_MOSI, SD_SCK> softSpi;
#define SD_CONFIG SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &softSpi)


SdFat32 SD;
File data;



Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(9600);

  while (!Serial);
  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) { //SPI mode usa portas 10 11 12 13 
    Serial.println("BAROMETRO não conectado.");
    while (1);
    }
  if (!SD.begin(SD_CONFIG)){
    Serial.println("Falha ao iniciar o cartão SD");
    while(1);
    }
  Serial.println("Cartão SD inicializado com sucesso!");
  Serial.println("BMP388 BAROMETRO ");
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);
  delay(2000);//tempo de estabilização que posso diminuir 
}


void loop() {
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return;
  }

  data = SD.open("TESTE.txt", O_WRONLY | O_CREAT | O_AT_END);
  num++;
  if (data)
  {
    Serial.print(num);
    Serial.print(" T= ");
    Serial.print(bmp.temperature);
    Serial.print(" *C");
    data.print(num);
    data.print(" T= ");
    data.print(bmp.temperature);
    data.print(" *C");


    Serial.print(" |P= ");
    Serial.print(bmp.pressure / 100.0);
    Serial.print(" hPa");
    data.print(" |P= ");
    data.print(bmp.pressure / 100.0);
    data.print(" hPa");


    Serial.print("|Alt≈ ");
    Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");
    data.print("|Alt≈ ");
    data.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
    data.println(" m");
    data.close();
  }
  else
  {
    Serial.println("Falha ao criar o arquivo!");
  }
  delay(500);
}






















