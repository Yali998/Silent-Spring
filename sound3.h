// Sound3.h - Animal alarm sound effects
#pragma once
#include <avr/pgmspace.h>
#define SPEAKER_PIN 8

// Global state variable
volatile bool isPlayingAnimalAlarm = false;

const PROGMEM uint16_t animal_alarm[][2] = {
  {1760, 80},  // A6 Screech
  {0, 40},     // Pause
  {1975, 60},  // B6
  {2349, 100}, // D7
  {1397, 150}, // F6
  {0, 30},     // Short rest
  {830, 180},  // Lower tone
  {0, 0}       // End marker
};

void playAnimalAlarm() {
  isPlayingAnimalAlarm = true;
  static uint8_t phase = 0;
  
  for (int i = 0; ; i++) {
    uint16_t freq = pgm_read_word(&animal_alarm[i][0]);
    uint16_t dur = pgm_read_word(&animal_alarm[i][1]);
    
    if (freq + dur == 0) {
      if (random(10) > 6) {
        tone(SPEAKER_PIN, 3000 + random(500), 50);
      }
      i = -1;
      continue;
    }
    
    // Add vibrato effect
    if (freq > 0) {
      uint16_t baseFreq = freq;
      for (int t = 0; t < dur; t += 20) {
        uint16_t vibFreq = baseFreq + random(-30, 30);
        tone(SPEAKER_PIN, vibFreq);
        delay(18);
      }
    } else {
      delay(dur);
    }
    noTone(SPEAKER_PIN);
  }
  isPlayingAnimalAlarm = false;
}