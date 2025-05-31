#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#include "sound1.h"
#include "sound2.h"
#include "sound3.h"

// NeoPixel Setting
#define LED_PIN 6          // Pin connected to NeoPixel data
#define NUM_LEDS 60        // 12 LEDs per group, with 5 groups in total

// Speaker Setting
#define SPEAKER_PIN 8      // Pin connected to Speaker data

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRBW + NEO_KHZ800);

// Sensor Pins
const int pirPin = 2;         // Motion detection sensor
const int touchPin = 3;       // Touch sensor
const int rainPin = A2;       // MH-RD rain sensor (connected to analog pin A2)

bool isPlayingSound1 = false; // Flag to control whether sound1 is playing
bool isPlayingSound2 = false; // Flag to control whether sound2 is playing
bool isPlayingSound3 = false; // Flag to control whether sound3 is playing
int activeGroups = 5;         // 5 groups of lights
int brightness = 255;         // Initial brightness
bool motionDetectedOnce = false; // Flag to indicate whether the first motion has been detected, making the motion sensor act as a switch


// Light up all LEDs
void lightUpAll() {
    strip.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // White light
    } 
    strip.show();
}


// Change each group of lights to red one by one
void turnToRed() {
    if (activeGroups > 0) {
        for (int i = (activeGroups - 1) * 12; i < activeGroups * 12; i++) {
            strip.setPixelColor(i, strip.Color(255, 0, 0, 0)); 
        }
        strip.show();
        activeGroups--;
    }
}


void setup() {
  pinMode(pirPin, INPUT);
  pinMode(touchPin, INPUT);
  pinMode(rainPin, INPUT); 

  Serial.begin(9600);
  strip.begin();
  strip.show();
  delay(1000);

  Serial.println("System Ready");
}

void loop() {
  int motionState = digitalRead(pirPin);  // Motion state detection
  int touchState = digitalRead(touchPin); // Touch state detection
  int rainValue = analogRead(rainPin);    // Rain sensor detection

  // 打印雨水传感器值
  Serial.print("Rain sensor value:");
  Serial.println(rainValue);

  // When motion is detected for the first time
  if (motionState == HIGH && motionDetectedOnce== false){
    Serial.println("Motion detected for the first time! Playing sound and turning on lights...");
    lightUpAll();
    
    if (isPlayingSound1 == false) {
      playSpiritedAway();  // Start sound1
      isPlayingSound1 = true;
    }
    activeGroups = 5;
    motionDetectedOnce = true;
  }

  // Continuously play Sound1 until switched to another sound
  if (isPlayingSound1){
    playSpiritedAway();
  }

  // Continuously play Sound2 in the background until switched to another sound
  if(isPlayingSound2){
    playNatureDisturbance();
  }

  // When the touch sensor is triggered, turn off one group of lights
  if (touchState == HIGH && motionDetectedOnce == true){
    Serial.println("Touch detected! Turning off one group of lights...");
    turnToRed();
    if(isPlayingSound1 == true){
      noTone(SPEAKER_PIN);
      isPlayingSound1 = false;
    }
    if(isPlayingSound2 == false){
      playNatureDisturbance();
      isPlayingSound2 = true;
    }
  }

  // Check if rain is detected
  if (rainValue < 850 && motionDetectedOnce == true && !isPlayingSound3) {
    Serial.println("Rain detected! Switching to sound 3.");
    playAnimalAlarm();
    isPlayingSound3 = true;
    isPlayingSound1 = false;
    isPlayingSound2 = false;
  } 

  // When no motion is detected and the switch is on
  if (motionState == LOW && motionDetectedOnce == true){
    Serial.println("No motion! Stopping sound and turning off lights...");
    if(isPlayingSound1 == true){
      noTone(SPEAKER_PIN);
      isPlayingSound1 = false;
    }
    if(isPlayingSound2 == true){
      noTone(SPEAKER_PIN);
      isPlayingSound2 = false;
    }
    if(isPlayingSound3 == true){
      noTone(SPEAKER_PIN);
      isPlayingSound3 = false;
    }

    for (int i = 0; i<NUM_LEDS; i++){
      strip.setPixelColor(i, strip.Color(0, 0, 0, 0));
    }
    strip.show();
    activeGroups = 0;
    motionDetectedOnce = false;
  }
}
