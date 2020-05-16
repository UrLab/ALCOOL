/*
  Author : minigrim0
  last Update : 17/03/2019
*/

#include <math.h>
#include <FAB_LED.h>
#include "speaker_pcm.h"

#define NUM_LEDS 26
#define SPEAKER_PIN 3

grbw pixels[NUM_LEDS];
int counter = 0;
int millisSinceLastRun = millis();
bool goingUp = true;

sk6812b<D,2> myLeds;

void setup() {
  //Initialisation du random seed pour avoir un aléatoire correct
  randomSeed(analogRead(0));
  myLeds.clear(1000);

  //initialisation des pins d'entrée/sortie de l'arduino
  pinMode(4, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
}

//Fais clignoter 5 fois en attendant un certain temps une certaine led en rouge
void blinkLed(int ledNb, int timeToSleep){
  for(int x=0;x<5;x++){
    pixels[ledNb].r = 200;
    pixels[ledNb].g = 0;
    pixels[ledNb].b = 0;

    myLeds.sendPixels(NUM_LEDS, pixels);
    delay(timeToSleep);

    pixels[ledNb].r = 0;
    pixels[ledNb].g = 0;
    pixels[ledNb].b = 0;

    myLeds.sendPixels(NUM_LEDS, pixels);
    delay(timeToSleep);
  }
}

//Débute le randomizer
void StartRandomizer(){

  startPlayback(); // Lance la "musique" sur le baffle
  // Fais clignoter 48 leds avant de sortir la led finale
  for(int tour=0;tour<48;tour++){

    uint16_t pos = random(NUM_LEDS);
    pixels[pos].r = 0;
    pixels[pos].g = 200;
    pixels[pos].b = 0;

    myLeds.sendPixels(NUM_LEDS, pixels);
    delay(100);

    pixels[pos].r = 0;
    pixels[pos].g = 0;
    pixels[pos].b = 0;

    myLeds.sendPixels(NUM_LEDS, pixels);
  }

  clearLeds();
  uint16_t pos = random(NUM_LEDS);
  blinkLed(pos, 300);
  pixels[pos].r = 200;
  pixels[pos].g = 0;
  pixels[pos].b = 0;
  myLeds.sendPixels(NUM_LEDS, pixels);
  delay(5);
  //On attends que le bouton soit réinitialisé
  while(digitalRead(4)){
    delay(5);
  }
  delay(500);
  stopPlayback(); //On s'assure que la musique est bien finie
}

// Permet d'éteindre toutes les leds
void clearLeds(){
  for(uint16_t pos=0;pos<NUM_LEDS;pos++){
    pixels[pos].r = 0;
    pixels[pos].g = 0;
    pixels[pos].b = 0;
    myLeds.sendPixels(NUM_LEDS, pixels);
  }
}

void loop() {

  // On met à jour les leds toutes les 30 millisecondes, en fonction de leur position, d'un compter initialisé avant et de l'état actuel (montée ou descente)
  if(millis()%30 == 0){

    for(int pos=0;pos<NUM_LEDS;pos++){
      pixels[pos].r = abs(((-1)*!goingUp)*(int)(220.0*sin((counter+pos)/10)))/40;
      pixels[pos].g = abs(((-1)*!goingUp)*(int)(220.0*sin((counter+pos)/100)))/40;
      pixels[pos].b = abs(((-1)*!goingUp)*(int)(220.0*sin((counter+pos)/1000)))/40;
    }
    counter ++;
    if (counter >= 360) goingUp = false;
    if (counter <= 0) goingUp = true;

    myLeds.sendPixels(NUM_LEDS, pixels);
  }

  //Si on détecte un appui sur le boutton, on lnce le randomizer (Mais en s'assurant qu'il a été terminé pour la dernière fois il y a plus d'une demi seconde, afin d'éviter les signaux brouillés)
  if(digitalRead(4) && millis()-millisSinceLastRun > 500){
    clearLeds();
    StartRandomizer();
    millisSinceLastRun = millis();
  }
}
