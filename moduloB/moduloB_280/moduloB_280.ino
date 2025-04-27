//Incluindo Bibliotecas
#include <Adafruit_BMP280.h>
#include <SoftwareSerial.h>
// portas
#define pinSET   2  //set apc220
#define pinRX    3//tx apc220
#define pinTX    4//rx apc220

#define BMP_SCK  12  //sck
#define BMP_MISO 11 // sdo    
#define BMP_MOSI 10 //sdi
#define BMP_CS   9  //csb
//variaveis
#define pressao_mar 1016 
int x=0;


Adafruit_BMP280 bmp(BMP_CS,BMP_MOSI, BMP_MISO, BMP_SCK);//defenicao da ligação spi
SoftwareSerial apc220(pinRX, pinTX);

void setup_apc(void){
      pinMode(pinSET, HIGH); 
      apc220.begin(9600);
  }
                                                   //Caso eu queria meter no modo write meto modoEnviar();
void modoEnviar(void){
    digitalWrite(pinSET, LOW);     //  config mode
    delay(30);                     
    apc220.println("WR 434350 3 9 3 0"); // configurações
    delay(30);
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    digitalWrite(pinSET, HIGH); // put apc220 back in operation
    delay(200);
  }


void setup() {
  //Iniciando a comunicação serial
  Serial.begin(9600);
  setup_apc();
  modoEnviar();
  // Imprimindo Mensagem de teste no Monitor Serial
  while (!apc220);
      apc220.println("com_ver");//alterar para V1
      if (!bmp.begin()) { //SPI mode usa portas 10 11 12 13 
        apc220.println("bmp280_err");//alterar para E1 por exemplo
        while(1);
      }else{
        apc220.println("bmp280_ver");//alterar para V2 por exemplo
        bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                      Adafruit_BMP280::SAMPLING_X8,     /* Temp. oversampling */
                      Adafruit_BMP280::SAMPLING_X8,    /* Pressure oversampling */
                      Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                      Adafruit_BMP280::STANDBY_MS_250);
    }
  }
      

void loop() {
    x++;
    char tempStr[10], presStr[10], altStr[10];
    dtostrf(bmp.readTemperature(), 4, 1, tempStr); // (valor, minWidth, casas decimais, buffer)
    dtostrf(bmp.readPressure() / 100.0, 6, 2, presStr);
    dtostrf(bmp.readAltitude(pressao_mar), 0, 2, altStr);
    char mensagem[30];
    sprintf(mensagem, ";B%d_%s|%s|%s|#", x, tempStr, presStr,altStr);//mensagem a enviar se nao funcionar, retirar o "B"
    
    apc220.println(mensagem);
    delay(1000);
}