/// Librairies
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "SevenSegmentFun.h"
#include "RTClib.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"


//// Module MP3
static const uint8_t PIN_MP3_TX = 6; // Connects to module's RX 
static const uint8_t PIN_MP3_RX = 7; // Connects to module's TX 
SoftwareSerial softwareSerial(PIN_MP3_RX, PIN_MP3_TX);
DFRobotDFPlayerMini player;


//// Afficheur TM1637
const byte PIN_CLK_Screen = A1;   // CLK pin 
const byte PIN_DIO_Screen = 8; // DIO pin
//SevenSegmentExtended TM1637(PIN_CLK_Screen, PIN_DIO_Screen);
SevenSegmentFun TM1637(PIN_CLK_Screen, PIN_DIO_Screen);
RTC_DS3231 rtc;
int backlight = 40; // Luminosité TM1637

///// bouttons 
int buttonPinHour=2; // Heure +
int buttonPinMin=3; // Min +

//// variables année mois date heure 
int yearupg;
int monthupg;
int dayupg;
int hourupg;
int minupg;
int secupg;
long lastShowTime = 0;  
int showTime=1000 ;// Delay affichage Heure -> millis

void setup() {
  pinMode(PIN_CLK_Screen, OUTPUT);
  pinMode(PIN_DIO_Screen, OUTPUT); 
  
// buttons 
  pinMode(buttonPinHour,INPUT);
  digitalWrite(buttonPinHour,HIGH);
  pinMode(buttonPinMin,INPUT);
  digitalWrite(buttonPinMin,HIGH);

// initialisation du port série @9600 bauds      
  Serial.begin(9600);
  softwareSerial.begin(9600);          
  TM1637.begin();               
  TM1637.setBacklight(backlight);
  TM1637.clear();  
  byte repeats = 2;
  TM1637.scrollingText("r2d2 fun", repeats);
  //TM1637.print("R2D2");
  //delay(3000);
  
  
// initialisation du module RTC      
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");   
  } 

   //Memorisation en dur de la date et l'heure : 18:03:30 02/04/2023   
   //rtc.adjust(DateTime(2023, 4, 02, 18, 03, 30)); 
   
// initialisation du lecteur MP3     
   player.begin(softwareSerial);
   player.volume(15);
   //player.play(); //Play the first MP3 file on the SD card
}


void loop() {
  
//// variables upgrade     
    DateTime now = rtc.now();
    yearupg=now.year();
    monthupg=now.month();
    dayupg=now.day();
    hourupg=now.hour();
    minupg=now.minute();
    secupg=now.second();

//// boucle millis 1 seconde affichage de l'écran  
    if(abs(millis() - lastShowTime) > showTime){
        TM1637.printTime(hourupg, minupg, true);
        lastShowTime = millis();   
    }

////  fonctions 
    hourupdate();
    minupdate();
    annoncevocale(); 
}



//// fonctions 
void hourupdate() { // bouton heure    
    if(digitalRead(buttonPinHour)==0) {
          hourupg++;
          if(hourupg>23)hourupg=0;
          rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
          TM1637.printTime(hourupg, minupg, true);
    }               
}

void minupdate() {   // bouton minute    
     if(digitalRead(buttonPinMin)==0) {
         minupg++;
         if(minupg>59)minupg=0;
         rtc.adjust(DateTime(yearupg, monthupg, dayupg, hourupg, minupg, secupg));
         TM1637.printTime(hourupg, minupg, true);
    }
}

void annoncevocale() {
   // conditions annonce vocale heure
   
   if(hourupg==07 && minupg==00 && secupg==00){
      player.play(0001);  // joue 07h00
   }
   if(hourupg==07 && minupg==30 && secupg==00){
      player.play(0003);  // joue 07h30
   } 
   if(hourupg==8 && minupg==00 && secupg==00){
      player.play(0004);  // joue 08h00
   }
   
   if(hourupg==8 && minupg==30 && secupg==00){
     player.play(0004);  // joue 08h00
   }
   if(hourupg==9 && minupg==00 && secupg==00){
      player.play(0004);  // joue 08h00
   }

   /*
   if(hourupg==9 && minupg==30 && secupg==00){
      mp3_play.play(0004);  // joue 08h00
   }
   if(hourupg==10 && minupg==00 && secupg==00){ // ko
      mp3_play(0100);  // joue 10h00
   } 
   if(hourupg==10 && minupg==30 && secupg==00){ // ko
      mp3_play(0130);  // joue 10h30
  } 
  if(hourupg==11 && minupg==00 && secupg==00){ // il est 08h00 ?
      mp3_play(0010);  // joue 11h00
  }
  if(hourupg==11 && minupg==30 && secupg==00){ // il est 07h00 ?
      mp3_play(0011);  // joue 11h30
  }
  if(hourupg==12 && minupg==00 && secupg==00){ // il est 11h00 ?
      mp3_play(0012);  // joue 12h00
  }
  if(hourupg==12 && minupg==30 && secupg==00){
      mp3_play(0013);  // joue 12h30
  }
  if(hourupg==13 && minupg==0 && secupg==00){
      mp3_play(0014);  // joue 13h00
  }
  if(hourupg==13 && minupg==30 && secupg==00){
      mp3_play(0015);  // joue 13h30
  }
  if(hourupg==14 && minupg==00 && secupg==00){
      mp3_play(0016);  // joue 14h00
  }
  if(hourupg==14 && minupg==30 && secupg==00){
      mp3_play(0017);  // joue 11h00
  }
  if(hourupg==15 && minupg==00 && secupg==00){
      mp3_play(00150);  // joue 11h00
  }
  if(hourupg==15 && minupg==30 && secupg==00){
     mp3_play(00170);  // joue 11h00
  }
     if(hourupg==16 && minupg==00 && secupg==00){
      mp3_play(0020);  // joue 11h00
  }
  if(hourupg==16 && minupg==30 && secupg==00){
      mp3_play(0021);  // joue 11h00
  }
  if(hourupg==17 && minupg==00 && secupg==00){
      mp3_play(0022);  // joue 11h00
  }
  if(hourupg==17 && minupg==30 && secupg==00){  
      mp3_play(0023);  // joue 11h00
  }
*/
}

void debug() {
    DateTime now = rtc.now();
    hourupg=now.hour();
    minupg=now.minute();
    Serial.print("Heure rtc: "); Serial.println(now.hour(), DEC);
    Serial.print("Heure upgrade: "); Serial.println(hourupg);
    Serial.print("Minute rtc: "); Serial.println(now.minute(), DEC);
    Serial.print("Minute upgrade: "); Serial.println(minupg);
    delay(1000); 
}
