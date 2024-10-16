//FartSeeker.cpp
#include "FartSeeker.h"
#include "SoundPlayer.h"
#include <M5Cardputer.h>
#include <Wire.h>
#include <SD.h>
#include <M5Unified.h>
//#include "brownNewsletterConfig.h"

// Declare brownConfig as extern
//extern BrownNewsletterConfig brownConfig;
// Definiciones y variables globales
#define ADC_I2C_ADDRESS 0x48
extern bool runningFartSeeker; // Asegúrate de declararla como extern en el .ino
extern SoundPlayer soundPlayer;
bool runningFartSeeker = false; // Define la variable aquí

void activateFartSeeker() {
    runningFartSeeker = true;
    M5Cardputer.Display.fillScreen(BLUE);

    while (runningFartSeeker) {
        M5Cardputer.update(); // Update keyboard state
        if (M5Cardputer.Keyboard.isKeyPressed('q')) {
            soundPlayer.play("/07pSostenido.wav");
            runningFartSeeker = false; // Exit the loop
            break;
        }

        int adcValue = readADC();
        int pppm = convertADCToPPPM(adcValue);
        displayAlert(pppm > THRESHOLD_PPPM);

        delay(1000); // Wait before the next reading
    }
}

// Las demás funciones permanecen iguales

void displayAlert(bool isGasDetected) {
  M5Cardputer.Display.fillRect(0, 20, 320, 40, BLACK);
  M5Cardputer.Display.setTextColor(isGasDetected ? RED : GREEN);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setCursor(10, 20);
  M5Cardputer.Display.print(isGasDetected ? "AIRE SUCIO" : "AIRE LIMPIO");
   if (isGasDetected){
   soundPlayer.play("/03fartSeekerAlert.wav");
 //   brownConfig.sendNotificationEmail();
   delay(5000);
   }
}

int readADC() {
  Wire.beginTransmission(ADC_I2C_ADDRESS);
  Wire.endTransmission();
  Wire.requestFrom(ADC_I2C_ADDRESS, 2);
  if (Wire.available()) {
    int highByte = Wire.read();
    int lowByte = Wire.read();
    return (highByte << 8) | lowByte;
  }
  return 0;
}

int convertADCToPPPM(int adcValue) {
  // Esta función debe ser ajustada según la calibración específica de tu sensor
  return adcValue;
}
