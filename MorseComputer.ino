/**
 * Morse code computer
 */

#include <EEPROM.h>

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include <MorseEnDecoder.h>

#define PIN_KEY 2
#define PIN_SPEAKER 3
#define PIN_MIC A0
#define MORSE_OUT LED_BUILTIN

// menu
#define MODE_MENU -1
#define MODE_SIMON 0
#define MODE_DECODE_KEY 1
#define MODE_DECODE_AUDIO 2
#define MODE_SPEED 3
#define MODE_MUTE 4
int mode = MODE_MENU;
int selected = MODE_SIMON;

// Software SPI (slower updates, more flexible pin options):
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6, 5, 4);
#define PIN_LIGHT 9

morseDecoder morseInput(PIN_KEY, MORSE_KEYER, MORSE_ACTIVE_LOW);
morseDecoder morseAudio(PIN_MIC,  MORSE_AUDIO, MORSE_ACTIVE_HIGH);
morseEncoder morseOutput(MORSE_OUT);

char simonChars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int quizSize = sizeof(simonChars) - 2;

int speed = 13;
boolean mute = false;

// setup a pin CHANGE intterupt for any pin
void pciSetup(byte pin) {
    *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
    PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
    PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}

// one of these will catch changes, based on pciSetup(MORSE_OUT);
ISR (PCINT0_vect){ toneOut(); } // 8-13
ISR (PCINT1_vect){ toneOut(); } // A0-A5
ISR (PCINT2_vect){ toneOut(); } // 0-7

void setup() {
  randomSeed(analogRead(2));
  pinMode(PIN_KEY, INPUT_PULLUP);
  pinMode(PIN_LIGHT, OUTPUT);
  morseInput.setspeed(speed);
  morseAudio.setspeed(speed);
  display.begin();
  display.setContrast(60);
  display.setTextColor(BLACK);
  
  int value = EEPROM.read(0);
  if (value < 100 && value > 0){
    speed = value;
  }
  mute = (EEPROM.read(1) == 255);

  // interrupts to make sound on out/in
  pciSetup(MORSE_OUT);
  attachInterrupt(0,toneIn,CHANGE);

  // turn on screen, show intro
  display.clearDisplay();
  digitalWrite(PIN_LIGHT, LOW);
  display.setTextSize(2);
  display.setCursor(11, 12);
  display.println("MORSE");
  display.setCursor(16, 27);
  display.setTextSize(1);
  display.print("computer");
  display.setCursor(0, 35);
  display.print("-- --- .-. ... ."); // MORSE
  display.display();

  // play demo morse
  delay(500);
  morseOutput.setspeed(30);
  playMorse('M');
  playMorse('O');
  playMorse('R');
  playMorse('S');
  playMorse('E');
  morseOutput.setspeed(speed);
}

// wait for a key down/up then return time that elapsed
unsigned long waitForKey() {
  while(digitalRead(PIN_KEY) == HIGH){ }
  unsigned long time = millis();
  delay(5);
  while(digitalRead(PIN_KEY) == LOW){ }
  return millis() - time;
}

// output a big letter on LCD, clear screen if full
void outputLetter(char letter) {
  static int charcount = 0;
  display.setTextSize(2);
  charcount++;
  if (charcount > 20){
    charcount = 0;
    display.clearDisplay();
    display.setCursor(0,0);
  }
  display.print(letter);
  display.display();
}

// play 1 morse letter
void playMorse(char letter){
  morseOutput.write(letter);
  morseOutput.encode();
  while(!morseOutput.available()){
    morseOutput.encode();
  }
}

// make screen for practice mode
void showSimon(char simonChar) {
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("key this:");
  display.setTextSize(2);
  display.print(simonChar);
  display.setTextSize(1);
  display.display();
  display.setCursor(0, 16);
}

// generic time-ticker
unsigned long timeElapsed;

void loop() {
  morseInput.decode();
  morseAudio.decode();

  switch(mode){
    case MODE_MENU:
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.write((selected == MODE_SIMON) ? 7 : 0);        display.print("simon\n");
      display.write((selected == MODE_DECODE_KEY) ? 7 : 0);   display.print("show keyed\n");
      display.write((selected == MODE_DECODE_AUDIO) ? 7 : 0); display.print("show audio\n");
      display.write((selected == MODE_SPEED) ? 7 : 0);        display.print("set wpm ("); display.print(speed); display.print(")\n");
      display.write((selected == MODE_MUTE) ? 7 : 0);         display.print("mute ("); display.print(mute ? "on" : "off"); display.print(")");
      display.display();

      timeElapsed = waitForKey();
      if(timeElapsed > 50){ // too short
        if(timeElapsed < 200){ // short press
          selected++;
          if (selected > MODE_MUTE){
            selected = 0;
          }
        }else{ //long press
          mode = selected;
          display.clearDisplay();
          display.setCursor(0,0);
          display.display();
        }
      }
      break;

    case MODE_MUTE:
      mute = !mute;
      if (mute){
        // just in case...
        noTone(PIN_SPEAKER);
        EEPROM.write(1, 255);
      }else{
        EEPROM.write(1, 0);
      }
      mode = MODE_MENU;
      break;

    case MODE_SPEED:
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0,0);
      display.print("current speed:");
      display.setTextSize(2);
      display.print(speed);
      if (speed<10){ display.print(" "); }
      display.setTextSize(1);
      display.print(" wpm\n\nset speed by  keying dots.");
      display.display();

      timeElapsed = waitForKey();
      if (timeElapsed > 20){
        if(timeElapsed < 400){ // short press
          speed = 1200 / timeElapsed;
          morseInput.setspeed(speed);
          morseAudio.setspeed(speed);
          morseOutput.setspeed(speed);
          EEPROM.write(0, speed);
        }else{
          mode = MODE_MENU;
        }
      }
      break;

    case MODE_SIMON:
      static char simonChar = simonChars[random(quizSize)];
      static boolean makeLetter = true;
      static int charcount = 0;
      
      if (makeLetter){
        showSimon(simonChar);
        playMorse(simonChar);
        makeLetter = false;
      }

      if (morseInput.available()){
        char input = morseInput.read();
        if (input == simonChar){
          // they got it right, give them a new letter
          display.clearDisplay();
          display.setCursor(0,12);
          display.setTextSize(2);
          display.print("GOOD!");
          display.display();
          simonChar = simonChars[random(random(quizSize))];
          makeLetter = true;
          
          // re-use charcount for flashing LCD-light
          charcount = 20;
          while(charcount){
            digitalWrite(PIN_LIGHT, HIGH);
            delay(50);
            digitalWrite(PIN_LIGHT, LOW);
            delay(50);
            charcount--;
          }
        }else{
          // they got it wrong, output letter, wordwrap & display on bottom
          charcount++;
          if (charcount > 56){
            charcount = 0;
            showSimon(simonChar);
          }
          if (input != ' '){
            display.setTextSize(1);
            display.print(input);
            display.display();
            playMorse(simonChar);
          }
        }
      }
      break;

    case MODE_DECODE_KEY:
      if (morseInput.available()){
        outputLetter(morseInput.read());
      }
      break;

    case MODE_DECODE_AUDIO:
      if (morseAudio.available()){
        outputLetter(morseAudio.read());
      }
      break;

  }
}

// play tone when morse LED is lit
void toneOut() {
  if (mute) return;
  if (digitalRead(MORSE_OUT)){
    tone(PIN_SPEAKER, 200);
  }else{
    noTone(PIN_SPEAKER);
  }
}

// play tone when user presses morse key, if not outputting
void toneIn() {
  if (mute || digitalRead(MORSE_OUT)) return;
  if (!digitalRead(PIN_KEY)){
    tone(PIN_SPEAKER, 400);
  }else{
    noTone(PIN_SPEAKER);
  }
}
