// sound1.h - Excerpt from the climax of the theme song of "Spirited Away"
#pragma once
#include <avr/pgmspace.h>
#define SPEAKER_PIN 8

// Global state variable
volatile bool isPlayingSpiritedAway = false;

// Japanese musical scale adjustments
const PROGMEM uint16_t spirited_away_scale[] = {
  0,    // Rest
  392,  // G4
  440,  // A4
  466,  // Bb4
  523,  // C5
  587,  // D5
  622,  // Eb5
  698,  // F5
  784   // G5
};

// Melody sequence for the climax section
const PROGMEM uint16_t melody[][2] = {
  {587, 400},  // D5 
  {523, 200},  // C5
  {587, 600},  // D5
  {523, 200},  // C5
  {466, 200},  // Bb4
  {440, 400},  // A4
  {392, 200},  // G4
  {523, 400},  // C5
  {466, 200},  // Bb4
  {440, 600},  // A4
  {392, 200},  // G4
  {440, 200},  // A4
  {466, 800},  // Bb4
  {587, 400},  // D5
  {622, 150},  // Eb5
  {587, 150},  // D5
  {523, 400},  // C5
  {466, 200},  // Bb4
  {440, 600},  // A4
  {784, 300},  // G5
  {698, 100},  // F5
  {622, 100},  // Eb5
  {587, 1200}, // D5
  {0, 0}       // 结束
};

void playSpiritedAway() {
  isPlayingSpiritedAway = true;
  for(int i=0; ;i++) {
    uint16_t freq = pgm_read_word(&melody[i][0]);
    uint16_t dur = pgm_read_word(&melody[i][1]);
    
    if(freq + dur == 0) break;
    
    // Simulate piano decay effect
    uint16_t decay_base = dur / 4;
    if(freq > 0) {
      tone(SPEAKER_PIN, freq);
      delay(dur - decay_base*3);
      for(int t=0; t<decay_base*3; t+=50){
        if(t % 100 < 50) tone(SPEAKER_PIN, freq);
        else noTone(SPEAKER_PIN);
        delay(50);
      }
    } else {
      delay(dur);
    }
    noTone(SPEAKER_PIN);
    
    // Add "Ma" (silence) unique to Japanese music
    if(i % 4 == 0) delay(0);
  }
  isPlayingSpiritedAway = false;
}