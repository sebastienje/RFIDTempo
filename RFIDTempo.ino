// Derniere modification: 14/05/2015

#include <VarSpeedServo.h>
#include <SoftwareSerial.h>
#include <SeeedRFIDLib.h>


VarSpeedServo myservo;
int servoPosition = 50;
boolean buttonState = HIGH;
boolean previousButtonState = HIGH;
boolean tempoState = HIGH;
int tempos[] = {4000,4000,4000}; // les trois tempos par d�faut de d�part
int temposMedium = 0;
int lastChangeTime = 0;
int pulseCount = 0;
int vitesse = 0;
int vitesse_i = 0; //vitesse servo invers�


// Connect the Reader's RX to the RX Pin and vice versa for TX
#define RFID_RX_PIN 2
#define RFID_TX_PIN 3

// Configure the Library in UART Mode
SeeedRFIDLib RFID(RFID_RX_PIN, RFID_TX_PIN);
RFIDTag tag;


void setup() {
  pinMode(12,INPUT);//input button
  pinMode(13,OUTPUT);//led tempo
  pinMode(7,OUTPUT);//led boutton
 
  myservo.attach(8,0,179);//output moteur
 
  Serial.begin(9600);

}

void loop() {
  {  // un bout de code exemple qui teste le RFID
  if(RFID.isIdAvailable()) {
    tag = RFID.readId();
    Serial.print("ID:       ");
    Serial.println(tag.id);
    Serial.print("ID (HEX): ");
    Serial.println(tag.raw);
  }
 
 
  // put your main code here, to run repeatedly:
  temposMedium = 0;
  for(int i=0; i<3; i++){
    temposMedium += tempos[i];
  }
  temposMedium /=3;
 
 
  buttonState = digitalRead(12);// lit l'etat du bouton
  if(buttonState != previousButtonState){
    // changement d'etat du bouton
    pulseCount++;
    //
    int timeDifference = millis()-lastChangeTime;
    int temposIndex = pulseCount%3;
    tempos[temposIndex] = timeDifference;
    lastChangeTime = millis();
  }
  previousButtonState = buttonState;
 
  int currentTime = millis()%temposMedium;
   tempoState = currentTime<temposMedium/2;
   
 /* la vitesse du servo  */
 vitesse = temposMedium;
 vitesse = map(vitesse, 1, 6000, 2, 180); // vitesse servo proportionnelle � battement
 vitesse_i = 182 - vitesse; // inverse la proportion
 
 
  if(tempoState){
    myservo.slowmove(180,vitesse_i);
  }
  else{
     myservo.slowmove(0,vitesse_i);
  }
 
  //buttonState = digitalRead(12);
  digitalWrite(13,tempoState);
  digitalWrite(7,buttonState);
 //delay(10);


//if(buttonState != previousButtonState){
Serial.print("V: ");
Serial.println(vitesse_i);
Serial.print("T: ");
Serial.println(tempoState);
Serial.print("M: ");
Serial.println(temposMedium);
Serial.print("B: ");
Serial.println(buttonState);
delay(500);
//}

  }

}
