
/*            Frequency: Unit is KHz,for example 434MHz is 434000
              RF Data Rate: 1,2,3 and 4 refer to 2400,4800,9600,19200bps
              Output Power: 0 to 9, 9 means 13dBm(20mW)
              UART Rate: 0,1,2,3,4,5 and 6 refers to 1200,2400,4800,9600, 19200,38400,57600bps
              Series Checkout: Series checkout:0 means no check,1 means even parity,2 means odd parity.
*/
    //bibliotecas
    #include <Servo.h>
    #include <SdFat.h>
    #include <Adafruit_Sensor.h>
    #include "Adafruit_BMP3XX.h"
    #include <SoftwareSerial.h> // bibliotaca que simula portas rx tx
    
    //portas
    
    #define servo_pin 13

    #define buzzer A1

    #define sensorBloqueio A2

    #define  SD_CS     12//cs
    #define  SD_MOSI  11 //mosi
    #define  SD_SCK  10 //clk
    #define  SD_MISO  9 //miso

    #define BMP_SCK  8  //sck
    #define BMP_MOSI 7 //sdi
    #define BMP_CS   6  //csb
    #define BMP_MISO 5 // sdo

    #define pinRX    3//tx apc220
    #define pinTX    4//rx apc220
    #define pinSET   2  //set apc220
    
    //variávies
    int x=0;
    int c=0;   
    int desacopulou = 0; 
    #define SEALEVELPRESSURE_HPA 1024
    
    
    
    //definir modulos
    SoftwareSerial apc220(pinRX, pinTX); //defenir os pins tx e rx do apc220
    
    Adafruit_BMP3XX bmp;
    
    Servo servo;
    
    SoftSpiDriver<SD_MISO, SD_MOSI, SD_SCK> softSpi;// Se der algum erro, lembrar mudar "SPI_DRIVER_SELECT" para 2 na library, no ficheiro SdFatConfig.h
    #define SD_CONFIG SdSpiConfig(SD_CS, DEDICATED_SPI, SD_SCK_MHZ(10), &softSpi)
    SdFat32 SD;
    File data;

  void check_file(void){//para que seja criado um ficheiro novo cada vez q o programa é corrido
    if(SD.exists("AZLink_A.txt"))
       SD.remove("AZLink_A.txt");
    if(SD.exists("AZLink_B.txt"))
       SD.remove("AZLink_B.txt"); 
  }
  void mensagem_A(void){//def da mensagem do mod A
    x++;

    char tempStr[10], presStr[10], altStr[10];
      dtostrf(bmp.temperature, 4, 1, tempStr); // (valor, minWidth, casas decimais, buffer)
      dtostrf(bmp.pressure / 100.0, 6, 2, presStr);
      dtostrf(bmp.readAltitude(SEALEVELPRESSURE_HPA), 4, 1, altStr);
    char mensagem[15];
      sprintf(mensagem, ";A%d|%s|%s|%s|", x, tempStr, presStr, altStr);//mensagem a enviar
    if (data){
      data.println(mensagem);
      data.close();
    }else{
      apc220.println("Err_SD_A");
      }
    apc220.println(mensagem);
  }
  void setup_apc(void){
      pinMode(pinSET, HIGH); 
      apc220.begin(9600);
  }                                                
  void setEnviar(void){//sintoniza com 433,250MHZ ou groundstation
    digitalWrite(pinSET, LOW);     //  config mode
    delay(50);                     
    apc220.println("WR 433250 3 9 3 0"); // configurações
    delay(50);
    while (apc220.available()) {
        Serial.write(apc220.read());
            }
    digitalWrite(pinSET, HIGH); // put apc220 back in operation
    delay(100);
  }                                     
  void setReceber(void) {//sintoniza com 434,750MHZ ou modulo B
    digitalWrite(pinSET, LOW); // config mode
    delay(50);
    apc220.println("WR 434750 3 9 3 0"); // config
    delay(50); 
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    digitalWrite(pinSET, HIGH); // ativar a antena
    delay(100);
  }
  void desacopular(void){
        static float h1=0;
        static float h2=0;
        int estado_desac=0;
        
        if(c==0 && desacopulou == 0){
           h1=bmp.readAltitude(SEALEVELPRESSURE_HPA);      
        }
        
        if(c==20 && desacopulou == 0){//c==a significa se ja passaram a segundos sendo a um int           
          h2=bmp.readAltitude(SEALEVELPRESSURE_HPA);
             
          if(h2-h1<-2){//h2-h1<-a significa se ja passaram a metros, sendo a um float
              int estadoBloqueio= digitalRead(sensorBloqueio);
              servo.write(0);   
              delay(1000);
              if(estadoBloqueio==LOW){
                  apc220.println("des_v");
                  servo.write(180);
                  desacopulou=1;
  
              }else{
                  apc220.println("err_des");
                  servo.write(180);
                  delay(300);
                  servo.write(0);
                  delay(300);
                  servo.write(180);
                  desacopulou=1;

                }
          }else{
              c=-1;
          }
          
        }
        
        if(desacopulou==0){
          c++;
        } 
  }
//***********************************************************************************************SETUP*************************************************************
  void setup(){
    Serial.begin(9600);
    setup_apc();
    setEnviar();   
    pinMode(sensorBloqueio,INPUT);
    pinMode(buzzer,OUTPUT);  
    servo.attach(servo_pin);
    servo.write(0);
    while (!apc220);
      apc220.println("V_COM");
      
      if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) { //SPI mode usa portas 5 6 7 8
        apc220.println("Err_BMP388");
        while (1);
      }
      
      apc220.println("V_BMP388");
      
      if (! SD.begin(SD_CONFIG)){
        apc220.println("Err SD");
      }else{
      apc220.println("V_SD");
      }
      
      check_file();

      bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
      bmp.setPressureOversampling(BMP3_OVERSAMPLING_8X);
      bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
      bmp.setOutputDataRate(BMP3_ODR_50_HZ);
      
      bmp.readAltitude(SEALEVELPRESSURE_HPA);//debug
      
      tone(buzzer,500);
      
      delay(5000);
      
      servo.write(180);
  }
    
//***********************************************************************************************LOOP*************************************************************
  void loop() { 
    desacopular();
    setReceber();
    if(desacopulou==1){
        tone(buzzer, 2000);
    }
    static String lastData = "";
    String receivedData = apc220.readStringUntil('#'); //lê até '#'
          // se recebe nova informação ent envia a nova informação e guarda latestedata com a nova informação
    if (receivedData.length() > 0) {
        lastData = receivedData;
        setEnviar();
        data = SD.open("AZLink_A.txt", O_WRONLY | O_CREAT | O_AT_END);
        mensagem_A();
        apc220.println(receivedData);
        delay(50);
        noTone(buzzer);

    }else{
        
        if (lastData.length() > 0){
          setEnviar();
          data = SD.open("AZLink_A.txt", O_WRONLY | O_CREAT | O_AT_END);
          mensagem_A();
          apc220.println(lastData); 
          delay(50);
          noTone(buzzer);
        
        }
    }
        
    data = SD.open("AZLink_B.txt", O_WRONLY | O_CREAT | O_AT_END);   
    if (data){
      data.println(lastData);
      data.close();
    }else{
      apc220.println("Err_SD_B");
    }
    delay(80);
 }
//alterar esta info

/*na primeria vez se nao receber mensagem nenhuma ,sobra me 870 milesimos de segundo para que o codigo corra sozinho ,
 o codigo demora 130 milesimas para estabilizar*/
//se receber mensagem sobra me 740 , sou seja 260 para estabilizar o codigo 
// se tudo correr bem o tempo minimo sao 260 milissegundos, recebendo mesnagem sobrando 740 ms para enviar e processar a mensagem
