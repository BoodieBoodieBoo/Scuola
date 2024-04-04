#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN); 

// Variabili per memorizzare il codice della tessera
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;

void setup()
{ 
  // Inizializzazione della comunicazione seriale
  Serial.begin(9600);
  // Inizializzazione della comunicazione SPI
  SPI.begin(); 
  // Inizializzazione del lettore RFID
  rfid.init();  
}

void loop()
{
    // Verifica se è presente una tessera
    if (rfid.isCard()) {
        // Legge il codice della tessera
        if (rfid.readCardSerial()) {
            // Controlla se il codice è diverso da quello memorizzato
            if (rfid.serNum[0] != serNum0
                && rfid.serNum[1] != serNum1
                && rfid.serNum[2] != serNum2
                && rfid.serNum[3] != serNum3
                && rfid.serNum[4] != serNum4
            ) {
                // Stampa il messaggio "Card found" se la tessera è nuova
                Serial.println(" ");
                Serial.println("Card found");
                // Aggiorna i valori del codice memorizzato
                serNum0 = rfid.serNum[0];
                serNum1 = rfid.serNum[1];
                serNum2 = rfid.serNum[2];
                serNum3 = rfid.serNum[3];
                serNum4 = rfid.serNum[4];
               
                // Stampa il codice decimale della tessera
                Serial.println(" ");
                Serial.println("Cardnumber:");
                Serial.print("Dec: ");
		Serial.print(rfid.serNum[0],DEC);
                Serial.print(", ");
		Serial.print(rfid.serNum[1],DEC);
                Serial.print(", ");
		Serial.print(rfid.serNum[2],DEC);
                Serial.print(", ");
		Serial.print(rfid.serNum[3],DEC);
                Serial.print(", ");
		Serial.print(rfid.serNum[4],DEC);
                Serial.println(" ");
                        
                // Stampa il codice esadecimale della tessera
                Serial.print("Hex: ");
		Serial.print(rfid.serNum[0],HEX);
                Serial.print(", ");
		Serial.print(rfid.serNum[1],HEX);
                Serial.print(", ");
		Serial.print(rfid.serNum[2],HEX);
                Serial.print(", ");
		Serial.print(rfid.serNum[3],HEX);
                Serial.print(", ");
		Serial.print(rfid.serNum[4],HEX);
                Serial.println(" ");
             } else {
               // Stampa un punto se la tessera è la stessa
               Serial.print(".");
             }
          }
    }
    
    // Pulisce il lettore RFID in attesa di una nuova lettura
    rfid.halt();
}
