// Sound2.h - Gradual sound effects influenced by environmental disturbances
#pragma once
#include <avr/pgmspace.h>
#define SPEAKER_PIN 8

// Global state variable
volatile bool isPlayingNatureDisturbance = false;

struct EnvironmentalSound {
  uint16_t frequency; // Frequency of the sound
  uint16_t duration;  // Duration of the sound
  int16_t  bend;      // Pitch bend amount
};

// Environmental disturbance sound sequence
const PROGMEM EnvironmentalSound nature_disturbance[] = {
  {262, 300, 50},  // C4 Mechanical low-frequency sound
  {330, 150, -20}, // E4 Vibration
  {0, 100, 0},     // Rest
  {392, 500, 30},  // Gradual intensification
  {200, 800, 200}, // Industrial noise
  {150, 400, 150}, // Additional industrial noise
  {800, 300, -100},// Metal friction
  {0, 500, 50},    // Environmental silence
  {392, 500, 30},  // Resuming sound
  {0, 100, 0},     // Rest
  {0, 0, 0}        // End marker
};

void playNatureDisturbance() {
  isPlayingNatureDisturbance = true;
  for (int i = 0; ; i++) {
    uint16_t freq = pgm_read_word(&nature_disturbance[i].frequency);
    uint16_t dur = pgm_read_word(&nature_disturbance[i].duration);
    int16_t bend = pgm_read_word(&nature_disturbance[i].bend);
    
    if (freq + dur + bend == 0) break;
    
    // Dynamic pitch bending
    if (bend != 0) {
      for (int t = 0; t < dur; t += 50) {
        tone(SPEAKER_PIN, freq + bend * sin(t * 0.1));
        delay(70);
      }
    } else if (freq > 0) {
      tone(SPEAKER_PIN, freq);
      delay(dur);
    } else {
      delay(dur);
    }
    noTone(SPEAKER_PIN);
  }
  isPlayingNatureDisturbance = false;
}