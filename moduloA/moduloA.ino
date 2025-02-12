/*            Frequency: Unit is KHz,for example 434MHz is 434000
              RF Data Rate: 1,2,3 and 4 refer to 2400,4800,9600,19200bps
              Output Power: 0 to 9, 9 means 13dBm(20mW)
              UART Rate: 0,1,2,3,4,5 and 6 refers to 1200,2400,4800,9600, 19200,38400,57600bps
              Series Checkout: Series checkout:0 means no check,1 means even parity,2 means odd parity.
*/
    //bibliotecas
    #include <Wire.h>
    #include <SPI.h>
    #include <SoftwareSerial.h> // bibliotaca que simula portas rx tx
    //portas
    #define pinRX    3//tx apc220
    #define pinTX    4//rx apc220
    #define pinSET   2  //set apc220
    //variávies
    //bool leitura=false;
    
  SoftwareSerial apc220(pinRX, pinTX); //defenir os pins tx e rx do apc220

  void setup_apc(void){
      pinMode(pinSET, HIGH); 
      apc220.begin(9600);
  }
  //sintoniza com 400MHZ ou groundstation                                                
  void setEnviar(void){
    digitalWrite(pinSET, LOW);     //  config mode
    delay(10);                     
    apc220.println("WR 433825 3 9 3 0"); // configurações
    delay(30);
    //Serial.println("setEnviar: ");
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    //Serial.println("");
    //Serial.println("AcabouE.");
    digitalWrite(pinSET, HIGH); // put apc220 back in operation
    delay(100);
  }  
  //sintoniza com 434,35MHZ ou modulo B                                   
  void setReceber(void) {
    digitalWrite(pinSET, LOW); // config mode
    delay(10);
    apc220.println("WR 434350 3 9 3 0"); // config
    delay(30); 
    //Serial.println("setReceber: ");
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    //Serial.println("");
    //Serial.println("AcabouR.");
    digitalWrite(pinSET, HIGH); // ativar a antena
    delay(100);
  }
//***********************************************************************************************SETUP*************************************************************
    void setup(){
    Serial.begin(19200);
    setup_apc();
    setReceber();
    //setEnviar();
    }
//***********************************************************************************************LOOP*************************************************************
  void loop() {
    //Envia sem #final
    String receivedData = "";
    // Data 434350 KHz
    if (apc220.available()) {
        receivedData = apc220.readStringUntil('#');   
        //receivedData+='#';//adiciona o # no final pois ele não é lido
        Serial.println(receivedData);

        if (receivedData.length() > 0) {
          //Modo Transmitir 400000 kHz
            setEnviar();
            
            //Serial.println(receivedData);
            apc220.println(receivedData);
            delay(100);
            
            //mensagem ja foi transmitida e volta a sintonizar com 434350 kHz
            setReceber();
        } 
    }
 }
/*na primeria vez se nao receber mensagem nenhuma ,sobra me 870 milesimos de segundo para que o codigo corra sozinho ,
 o codigo demora 130 milesimas para estabilizar*/
//se receber mensagem sobra me 740 , sou seja 260 para estabilizar o codigo 
// se tudo correr bem o tempo minimo sao 260 milissegundos, recebendo mesnagem sobrando 740 ms para enviar e processar a mensagem







