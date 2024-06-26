#include <SPI.h>
#include <RFID.h>
#include <EEPROM.h>

#define SS_PIN 10
#define RST_PIN 9

// Codice della chiave master.
#define masnum0 252 
#define masnum1 162
#define masnum2 51
#define masnum3 59
#define masnum4 86

#define ledGreen 7  // Pin su cui colleghiamo il LED Verde
#define ledRed 2  // Pin su cui colleghiamo il LED Rosso
#define resetkey 6
#define pausa 1000 // Costante tempo 

RFID rfid(SS_PIN, RST_PIN); 

boolean ledOn = 0; // Variabile Led acceso
boolean stato = 0; // Variabile stato 
boolean cardmas = 0; // Variabile chiave master
int ledState = 0;  // Variabile stato led
int slave; // Contatore delle chiavi salvate
    
int sernum0;
int sernum1;
int sernum2;
int sernum3;
int sernum4;

void setup()
{ 
  Serial.begin(9600);   // Apriamo la comunicazione con il monitor seriale
  SPI.begin(); 
  rfid.init();
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(resetkey, INPUT);

  digitalWrite(ledGreen, LOW);
  Serial.println("Accensione led con RFID Pronto");
}

void loop() {
 
  slave = EEPROM.read(0);
  
  // Rileva un tag...
  if (rfid.isCard()) {
      // Legge il seriale...
      if (rfid.readCardSerial()) {
        sernum0 = rfid.serNum[0];
        sernum1 = rfid.serNum[1];
        sernum2 = rfid.serNum[2];
        sernum3 = rfid.serNum[3];
        sernum4 = rfid.serNum[4];
        
        // Se il seriale letto corrisponde con il seriale Master
        // attiva o disattiva la modalità Memorizzazione chiavi
        // e in più visualizza l'elenco della chiavi salvate... 
        if (sernum0 == masnum0
            && sernum1 == masnum1
            && sernum2 == masnum2
            && sernum3 == masnum3
            && sernum4 == masnum4) {
            
            if (cardmas==0) {
                Serial.println("CARD MASTER");
                delay(1500);
                Serial.println();
                Serial.println("GESTIONE MEMORIZZAZIONE CHIAVI");
                cardmas = 1;
                Serial.println("Chiavi slave: ");
                Serial.println(slave);
                delay(2000);
            }  else { 
                cardmas = 0;
                Serial.println("Annullata");
                delay(3000);
                standby();
            }
        } 
        // Se invece il seriale letto corrisponde con uno dei tre gruppi 
        // di chiavi memorizzate allora attiva o disattiva il Led
        else if ((sernum0 == EEPROM.read(1) && sernum1 == EEPROM.read(2) && sernum2 == EEPROM.read(3) && sernum3 == EEPROM.read(4) && sernum4 == EEPROM.read(5))
                || (sernum0 == EEPROM.read(6) && sernum1 == EEPROM.read(7) && sernum2 == EEPROM.read(8) && sernum3 == EEPROM.read(9) && sernum4 == EEPROM.read(10))
                || (sernum0 == EEPROM.read(11) && sernum1 == EEPROM.read(12) && sernum2 == EEPROM.read(13) && sernum3 == EEPROM.read(14) && sernum4 == EEPROM.read(15))) {
                  
            if (ledOn == 0) {  
                ledOn = 1;
                Serial.println("CHIAVE VALIDA LED ACCESO");
                digitalWrite(ledGreen, HIGH);
            } else {
                ledOn = 0;
                stato = 0;
                digitalWrite(ledGreen, LOW);
                Serial.println("CHIAVE VALIDA LED SPENTO");
                delay(500);
                standby();
            } 
        } 
        // Se il seriale letto è diverso dal master e non è presente in memoria,
        // e se è attiva la modalita Memorizzazione chiavi, salva il seriale in memoria
        // come slave1, slave2 o slave3.
        else if (cardmas == 1 && slave == 0) {
            Serial.println("Chiave rilevata!");
            EEPROM.write(0, 1);
            EEPROM.write(1, sernum0);
            EEPROM.write(2, sernum1);
            EEPROM.write(3, sernum2);
            EEPROM.write(4, sernum3);
            EEPROM.write(5, sernum4);
            cardmas = 0;
            delay(1000);
            Serial.println("Slave 1 salvata!");
            delay(3000);
            standby();
        } else if (cardmas == 1 && slave == 1) {
            Serial.println("Chiave rilevata!");
           
