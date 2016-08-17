/**
    Super Mario Overworld / Main Theme per Arduino
    Code for Sketch (Arduino) tested on Arduino UNO R3

    Made by Reyboz Blog
    [ http://blog.reyboz.it ]

    Un caloroso ringraziamento a Joseph Karam, autore delle trascrizioni degli
    spartiti per pianoforte a due mani sulle note della composizione originale della
    Nintendo Entertainment System by Koji Kondo, che ci ha permesso di riprodurre
    fedelmente la canzone.
    [ http://www.mariopiano.com ]
*/

//  [ http://code.google.com/p/rogue-code/wiki/ToneLibraryDocumentation ]
#include <Tone.h>

// Define speaker pins (PWM~) [REQUIRED]
#define PIN_PLAYER_1 11
#define PIN_PLAYER_2 10

// Define ground pins for display [OPTIONAL]
#define DISPLAY_1 13
#define DISPLAY_2 12

// Trimmer pins (Analog in) [OPTIONAL]
#define PIN_VOLUME 0

// LED pins [OPTIONAL]
#define LED_1 9
#define LED_2 8
#define LED_3 7
#define LED_4 6
#define LED_5 5
#define LED_6 4
#define LED_7 3
#define LED_8 2

Tone player_1;
Tone player_2;

int tempo;

// Questo valore più è alto più stacca le note fra loro. [OPTIONAL]
int tronca = 15;

// Puoi impostare i limiti di velocità [OPTIONAL]
int tempo_minimo = 78;
int tempo_massimo = 500;

// Definisci l'ordine dei pin nel display affinchè siano ordinati secondo la convenzione universale [OPTIONAL]
int pin_leds[8] = {LED_3, LED_4, LED_5, LED_6, LED_7, LED_2, LED_1, LED_8};

// Note facili da usare (Non modificare)
#define SOL_d2 104
#define LA2    110
#define LA_d2  114
#define SI2    124
#define DO3    131
#define DO_d3  139
#define RE3    147
#define RE_d3  156
#define MI3    165
#define FA3    175
#define FA_d3  185
#define SOL3   196
#define SOL_d3 208
#define LA3    220
#define LA_d3  233
#define SI3    247
#define DO4    262
#define DO_d4  277
#define RE4    294
#define RE_d4  311
#define MI4    330
#define FA4    349
#define FA_d4  370
#define SOL4   392
#define SOL_d4 415
#define LA4    440
#define LA_d4  466
#define SI4    494
#define DO5    523
#define DO_d5  554
#define RE5    587
#define RE_d5  622
#define MI5    659
#define FA5    698
#define FA_d5  740
#define SOL5   784
#define SOL_d5 831
#define LA5    880
#define LA_d5  932
#define SI5    988
#define DO6    1046
#define DO_d6  1109
#define RE6    1175
#define RE_d6  1245
#define MI6    1319
#define FA6    1397
#define FA_d6  1480
#define SOL6   1568
#define SOL_d6 1661
#define LA6   1760
#define LA_d6  1865


// DON'T EDIT SETUP
void setup() {
  //Serial.begin(9600);
  pinMode(DISPLAY_1, OUTPUT);
  pinMode(DISPLAY_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  pinMode(LED_5, OUTPUT);
  pinMode(LED_6, OUTPUT);
  pinMode(LED_7, OUTPUT);
  pinMode(LED_8, OUTPUT);
  player_1.begin(PIN_PLAYER_1);
  player_2.begin(PIN_PLAYER_2);
}
void loop() {
  idraya2();
  while (1); // Shut up!
}

void enableDisplay(int pin) {
  digitalWrite(pin, LOW);
}

void disableDisplay(int pin) {
  digitalWrite(pin, HIGH);
}

int inputTempo() {
  //global tempo_massimo, tempo_minimo;
  return (analogRead(PIN_VOLUME) / 1023.0) * (tempo_massimo - tempo_minimo) + tempo_minimo;
  Serial.println((analogRead(PIN_VOLUME) / 1023.0) * (tempo_massimo - tempo_minimo) + tempo_minimo);
}


int getAmericanIndexOfLetterFromFrequency(int frequency) {
  double nota = 12 * (log(((double)frequency) / 440) / log(2));
  //Serial.println();
  //Serial.println(nota);
  while (nota >= 12) {
    nota = nota - 12;
  }
  while (nota < 0 ) {
    nota = nota + 12;
  }
  //Serial.println(nota);
  // Arrotondamento
  float virgola = nota - ((int) nota);
  if (virgola >= 0) {
    return (virgola < 0.5) ? nota : ++nota;
  } else {
    return (virgola < -0.5) ? nota : --nota;
  }
}

void setLetter(int index_letter) {
  // global pin_leds;
  boolean letter_table[12][8] = {
    {true, true, true, false, true, true, true, false}, // A
    {true, true, true, false, true, true, true, true}, // A_d
    {false, false, true, true, true, true, true, false}, // B
    {true, false, false, true, true, true, false, true}, // C_d
    {true, false, false, true, true, true, false, false}, // C
    {false, true, true, true, true, false, true, false}, // D
    {false, true, true, true, true, false, true, true}, // D_d
    {true, false, false, true, true, true, true, false}, // E
    {true, false, false, false, true, true, true, false}, // F
    {true, false, false, false, true, true, true, true}, // F_d
    {true, false, true, true, true, true, true, false}, // G
    {true, false, true, true, true, true, true, true} // G_d
  };
  for (int i = 0; i < 8; i++) {
    digitalWrite(pin_leds[i], (letter_table[index_letter][i]) ? HIGH : LOW);
  }
}

void suona(int frequency, float fract) { // 1 buzzer 1 nada
  setLetter(getAmericanIndexOfLetterFromFrequency(frequency));
  player_2.play(frequency, inputTempo() * fract - tronca);
  enableDisplay(DISPLAY_1);
  while (player_2.isPlaying());
  disableDisplay(DISPLAY_1);
  delay(tronca);
}

void suonaCoppia(int frequency, float fract) { // 2 buzzer 1 nada
  int index_letter = getAmericanIndexOfLetterFromFrequency(frequency);
  int ms = inputTempo() * fract - tronca;
  player_1.play(frequency, ms);
  player_2.play(frequency, ms);
  while (player_1.isPlaying() || player_2.isPlaying()) {
    enableDisplay(DISPLAY_1);
    setLetter(index_letter);
    delay(4);
    disableDisplay(DISPLAY_1);
    delay(1);
    enableDisplay(DISPLAY_2);
    setLetter(index_letter);
    delay(4);
    disableDisplay(DISPLAY_2);
  }
  delay(tronca);
}

void suonaDoppio(int frequency_1, int frequency_2, float fract) { // 2 buzzer 2 nada
  int index_letter_1 = getAmericanIndexOfLetterFromFrequency(frequency_1);
  int index_letter_2 = getAmericanIndexOfLetterFromFrequency(frequency_2);
  int ms = inputTempo() * fract - tronca;
  player_1.play(frequency_1, ms);
  player_2.play(frequency_2, ms);
  while (player_1.isPlaying() || player_2.isPlaying()) {
    enableDisplay(DISPLAY_1);
    setLetter(index_letter_1);
    delay(4);
    disableDisplay(DISPLAY_1);
    delay(1);
    enableDisplay(DISPLAY_2);
    setLetter(index_letter_2);
    delay(4);
    disableDisplay(DISPLAY_2);
  }
  delay(tronca);
}

void pausa(float fract) { // berhenti
  delay(inputTempo() * fract);
}


void idraya2() {
  //baris 1
  
  suonaDoppio(MI4,DO4, 0.75);
  suonaDoppio(FA4,RE4, 0.25);

  suonaDoppio(SOL4,DO4, 1);
  suonaCoppia(MI5, 1.75);
  suonaCoppia(MI5, 0.25);
  suonaCoppia(RE5, 0.75);
  suonaCoppia(RE5, 0.25);

  suonaCoppia(DO5, 1);
  suonaDoppio(SOL4, MI4, 1.5);
  pausa(0.5);
  suonaCoppia(SOL4, 0.75);
  suonaCoppia(SOL4, 0.25);

  suonaDoppio(LA4, FA4, 1);
  suonaDoppio(SOL4, MI4, 1);
  suonaDoppio(FA4, RE4, 1);
  suonaDoppio(MI4, DO4, 1);

  suonaDoppio(RE4, SI3, 1.75);
  suonaCoppia(LA3, 0.25);
  suonaCoppia(SOL3, 0.75);
  pausa(0.25);
  suonaCoppia(RE4, 0.75);
  suonaCoppia(MI4, 0.25);
  //baris2

  suonaDoppio(FA4, RE4, 1);
  suonaCoppia(RE5, 1.75);
  suonaCoppia(RE5, 0.25);
  suonaCoppia(DO5, 0.75);
  suonaCoppia(DO5, 0.25);

  suonaDoppio(SI4, SOL4, 1);
  suonaDoppio(LA4, FA4, 1.5);
  pausa(0.5);
  suonaCoppia(SOL4, 0.75);
  suonaCoppia(SOL4, 0.25);

  suonaDoppio(SI4, SOL4, 1);
  suonaDoppio(LA4, FA4, 1);
  suonaDoppio(SOL4, MI4, 1);
  suonaDoppio(FA4, RE4, 1);

  suonaDoppio(MI4, DO4, 2.5);
  pausa(0.5);
  suonaCoppia(MI4, 0.75);
  suonaCoppia(FA4, 0.25);

  //baris3
  suonaDoppio(SOL4,DO4, 1);
  suonaCoppia(MI5, 1.75);
  suonaCoppia(MI5, 0.25);
  suonaCoppia(RE5, 0.75);
  suonaCoppia(RE5, 0.25);

  suonaCoppia(DO5, 1);
  suonaDoppio(SOL4, MI4, 1.5);
  pausa(0.5);
  suonaCoppia(SOL4, 0.75);
  suonaCoppia(SOL4, 0.25);

  suonaDoppio(LA4, FA4, 1);
  suonaDoppio(SOL4, MI4, 1);
  suonaDoppio(DO5, LA4, 1);
  suonaDoppio(RE5, SI4, 1);

  suonaDoppio(SI4, SOL4, 2);
  suonaDoppio(LA4, FA4, 0.5);
  pausa(0.5);
  suonaCoppia(LA4, 0.75);
  suonaCoppia(LA4, 0.25);

  //baris4
  suonaCoppia(FA5, 1);
  suonaCoppia(FA5, 1);
  suonaCoppia(MI5, 1);
  suonaCoppia(RE5, 1);

  suonaDoppio(SOL5, MI5, 2);
  suonaDoppio(DO5, SOL4, 0.5);
  pausa(0.5);
  suonaCoppia(SI4, 0.75);
  suonaCoppia(LA4, 0.25);

  suonaDoppio(SOL4,MI4, 1);
  suonaDoppio(FA5, RE5, 1);
  suonaDoppio(MI5, DO5, 1);
  suonaDoppio(RE5, SI4, 1);

  suonaDoppio(DO5, SOL4, 2.5);
  pausa(0.5);
  suonaCoppia(SOL4, 0.75);
  suonaCoppia(SOL4, 0.25);

  //baris 5
  suonaDoppio(LA4,FA4, 1);
  suonaCoppia(FA5, 0.75);
  suonaCoppia(FA5, 0.25);
  suonaDoppio(FA5, FA4, 1);
  suonaDoppio(FA5, LA4, 0.75);
  suonaDoppio(FA5, SI4, 0.25);

  suonaDoppio(MI5, DO5, 1);
  suonaCoppia(DO5, 0.75);
  suonaCoppia(DO5, 0.25);
  suonaDoppio(DO5, SOL4, 1);
  suonaDoppio(SI4, SOL4, 0.75);
  suonaDoppio(DO5, LA4, 0.25);

  suonaDoppio(RE5, SI4, 1);
  suonaCoppia(SOL5, 0.75);
  suonaCoppia(SOL5 , 0.25);
  suonaDoppio(SOL5 , SI4, 1);
  suonaDoppio(FA5, LA4, 1);

  //BARIS6
  suonaDoppio(MI5, SOL4, 2);
  suonaDoppio(DO5, SOL4, 0.5);
  pausa(0.5);
  suonaCoppia(SOL4, 0.75);
  suonaCoppia(SOL4, 0.25);

  suonaDoppio(LA4, FA4, 1);
  suonaCoppia(FA5, 0.75);
  suonaCoppia(FA5, 0.25);
  suonaDoppio(FA5, FA4, 1);
  suonaDoppio(FA5, LA4, 0.75);
  suonaDoppio(FA5, SI4, 0.25);

  suonaDoppio(MI5, DO5, 1);
  suonaCoppia(DO5, 0.75);
  suonaCoppia(DO5, 0.25);
  suonaDoppio(DO5, SOL4, 1);
  suonaDoppio(SI4, SOL4, 0.75);
  suonaDoppio(DO5, LA4, 0.25);

  //BARIS 7
  suonaDoppio(RE5, SI4, 1);
  suonaCoppia(SOL5, 1);
  suonaDoppio(SOL5, SI4, 1);
  suonaCoppia(MI5, 0.75);
  suonaCoppia(RE5, 0.25);

  suonaDoppio(DO5, SOL4, 2);
  pausa(1);
  suonaCoppia(DO5, 0.75);
  suonaCoppia(DO5, 0.25);


  //2X (REFF)
  for (int i = 0; i < 2; i++) {
    suonaDoppio(FA5, FA4, 1);
    suonaCoppia(LA5, 0.75);
    suonaCoppia(LA5, 0.25);
    suonaDoppio(LA5, FA4,  1);
    suonaDoppio(LA5, LA4, 0.75);
    suonaDoppio(LA5, SI4, 0.25);

    //BARIS 8
    suonaDoppio(SOL5, DO5, 1);
    suonaCoppia(MI5, 0.75);
    suonaCoppia(MI5, 0.25);
    suonaDoppio(MI5, SOL4, 1);
    suonaDoppio(SOL5,SOL4, 0.75);
    suonaDoppio(SOL5,LA4, 0.25);

    suonaDoppio(FA5, SI4, 1);
    suonaCoppia(RE5, 0.75);
    suonaCoppia(RE5, 0.25);
    suonaDoppio(RE5, SOL4, 1);
    suonaDoppio(SOL5, SOL4, 0.75);
    suonaDoppio(FA5, LA4, 0.25);

    suonaDoppio(MI5, SOL4, 2);
    suonaDoppio(DO5, DO4, 0.5);
    pausa(0.5);
    suonaCoppia(DO5, 0.75);
    suonaCoppia(DO5, 0.25);

    //baris 9
    suonaDoppio(FA5, FA4, 1);
    suonaCoppia(LA5, 0.75);
    suonaCoppia(LA5, 0.25);
    suonaDoppio(LA5,FA4, 1);
    suonaDoppio(LA5, LA4, 0.75);
    suonaDoppio(LA5, SI4, 0.25);

    suonaDoppio(SOL5, DO5, 1);
    suonaCoppia(MI5, 0.75);
    suonaCoppia(MI5, 0.25);
    suonaDoppio(MI5, SOL4, 1);
    suonaDoppio(SOL5, SOL4, 0.75);
    suonaDoppio(SOL5, LA4, 0.25);

    suonaDoppio(SOL5, SI4, 1);
    suonaCoppia(FA5, 0.75);
    suonaCoppia(MI5, 0.25);
    suonaDoppio(RE5, SOL4, 1);
    suonaDoppio(MI5, SOL4, 0.75);
    suonaDoppio(RE5, LA4, 0.25);
    if (i == 0) {
      //BARIS 10
      suonaDoppio(DO5, MI4, 2.5);
      pausa(0.5);
      suonaCoppia(DO5, 0.75);
      suonaCoppia(DO5, 0.25);
    }

  }
  suonaDoppio(DO5, SOL4, 2.5);

}


