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
    bool readDone=false;
    
    
  SoftwareSerial apc220(pinRX, pinTX); //defenir os pins tx e rx do apc220

  void setup_apc(void){
      pinMode(pinSET, HIGH); 
      apc220.begin(9600);
  }
                                                   //Caso eu queria meter no modo write meto setEnviar();
  void setEnviar(void){
    digitalWrite(pinSET, LOW);     //  config mode
    delay(10);                     
    apc220.println("WR 437000 3 9 3 0"); // configurações
    delay(10);
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    digitalWrite(pinSET, HIGH); // put apc220 back in operation
    delay(10);
  }  
                                                   //Caso eu queria meter no modo read meto setReceber();
  void setReceber(void) {
    digitalWrite(pinSET, LOW); // config mode
    delay(10);
    apc220.println("WR 434000 3 9 3 0"); // config
    delay(10); 
    while (apc220.available()) {
        Serial.write(apc220.read());
    }
    digitalWrite(pinSET, HIGH); // ativar a antena
    delay(10);
  }


//***********************************************************************************************SETUP*************************************************************
    void setup(){
    setup_apc();
    setReceber();
    //Serial.begin(9600);
    }
//***********************************************************************************************LOOP*************************************************************
  void loop() {
    if (readDone){
        setReceber();
        readDone=false;//para evitar meter no modo read desnecessáriamente
    }
      
    if (apc220.available()) { 
        String receivedData = Serial.readString(); // le a mensagem toda
        setEnviar();
        apc220.println(receivedData);
        readDone=true;
    }
    









