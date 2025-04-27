/*            Frequency: Unit is KHz,for example 434MHz is 434000
              RF Data Rate: 1,2,3 and 4 refer to 2400,4800,9600,19200bps
              Output Power: 0 to 9, 9 means 13dBm(20mW)
              UART Rate: 0,1,2,3,4,5 and 6 refers to 1200,2400,4800,9600, 19200,38400,57600bps
              Series checkout:0 means no check,1 means even parity,2 means odd parity.
              WR é para definir as defenições
              RD é para verificar as defenições, não há modo especifico de read ou write
*/
    //bibliotecas
    #include <Adafruit_Sensor.h>
    #include "Adafruit_BMP3XX.h"
    #include <SoftwareSerial.h> // bibliotaca que simula portas rx tx
    //portas
        //apc220
    #define pinRX    3//tx apc220
    #define pinTX    4//rx apc220
    #define pinSET   2  //set apc220
        //bmp388
    #define BMP_SCK  12  //sck
    #define BMP_MOSI 11 //sdi
    #define BMP_CS   10  //csb
    #define BMP_MISO 9 // sdo
    //variávies
    #define  pressaoNoMar  1024//alterar para calibrar no momento
    int x=0;

Adafruit_BMP3XX bmp;//definir o barometro com nome bmp
SoftwareSerial apc220(pinRX, pinTX); //defenir a ligação tx e rx do apc220

  void setup_apc(void){//inicia o apc220
      pinMode(pinSET, HIGH); 
      apc220.begin(9600);
  }
  
  void modoEnviar(void){//434750hz
    digitalWrite(pinSET, LOW);     //  config mode
    delay(30);                     
    apc220.println("WR 434750 3 9 3 0"); // configurações
    delay(30);
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    digitalWrite(pinSET, HIGH); // funcionamento
    delay(200);
  }
//***********************************************************************************************SETUP*************************************************************
    void setup(){
    Serial.begin(9600);

    setup_apc();
    modoEnviar();
    
    while (!apc220);
      apc220.println("com_bv");
      if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) { //SPI mode usa portas 9 10 11 12  
        apc220.println("bar_bf");
        while(1);
      }else{
        apc220.println("bar_bv");
      }
      bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
      bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
      bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
      bmp.setOutputDataRate(BMP3_ODR_50_HZ);
    }
//***********************************************************************************************LOOP*************************************************************
    void loop(){
    if (! bmp.performReading()) {
      apc220.println("errReadBarB");
      return;
    }
    
    x=x+1;//numero da leitura
    
    //defenição da mensagem a enviar para que esta seja enviada numa única vez e nao aos pedaços. mensagem ou é toda enviada ou entao nao.
    char tempStr[10], presStr[10], altStr[10];
    dtostrf(bmp.temperature, 4, 1, tempStr); // (valor, minWidth, casas decimais, buffer)
    dtostrf(bmp.pressure / 100.0, 6, 2, presStr);
    dtostrf(bmp.readAltitude(pressaoNoMar), 4, 1, altStr);
    
    char mensagem[15];
    sprintf(mensagem, ";B%d|%s|%s|%s|#", x, tempStr, presStr, altStr);
    apc220.println(mensagem);
    
    delay(500);
}


















