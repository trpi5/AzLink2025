
#include <SPI.h>
#include <SdFat.h>

//Define os pinos de ligação CS
#define  SD_SCK  7 //clk
#define  SD_MOSI  6 //mosi
#define  SD_CS     5//cs
#define  SD_MISO  8 //miso
//Define as variáveis utilizadas no código
int num = 0;


// Se der algum erro, lembrar mudar "SPI_DRIVER_SELECT" para 2 na library, no ficheiro SdFatConfig.h
SoftSpiDriver<SD_MISO, SD_MOSI, SD_SCK> softSpi;
#define SD_CONFIG SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &softSpi)

SdFat32 SD;
File data;// = SD.open("teste3.txt", O_WRONLY | O_CREAT | O_AT_END); 





void setup()
{
  // Inicia a comunicação serial
  Serial.begin(9600);
  // Verifica a conexão com o módulo micro SD
  if (!SD.begin(SD_CONFIG))
  {
    Serial.println("Falha ao iniciar o cartão SD");
    while(1);
  }
    Serial.println("Cartão SD inicializado com sucesso!");
}

 
void loop()
{
  data = SD.open("teste3.txt", O_WRONLY | O_CREAT | O_AT_END);
  num++;
  if (data)
  {
    Serial.print("A enviar: ");
    Serial.println(num);
    data.println(num);
    data.close();
  }
  else
  {
    Serial.println("Falha ao criar o arquivo!");
  }
  delay(300);
}









