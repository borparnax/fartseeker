//FartSeeker.cpp
#include "FartSeeker.h"
#include "SoundPlayer.h"
#include <M5Cardputer.h>
#include <Wire.h>
#include <SD.h>

// Definiciones y variables globales
#define ADC_I2C_ADDRESS 0x48
extern bool runningFartSeeker; // Asegúrate de declararla como extern en el .ino
extern SoundPlayer soundPlayer;
bool runningFartSeeker = false; // Define la variable aquí

void activateFartSeeker() {
    runningFartSeeker = true;
    M5Cardputer.begin();
    Wire.begin();
    SD.begin();
    M5Cardputer.Display.setRotation(1);
    M5Cardputer.Display.fillScreen(BLUE);
    M5Cardputer.Speaker.begin();

    while(runningFartSeeker) {
        M5Cardputer.update(); // Actualiza el estado del teclado
        if (M5Cardputer.Keyboard.isKeyPressed('q')) {
            soundPlayer.play("/07pSostenido.wav");
            runningFartSeeker = false; // Permite salir del bucle
            break;
        }

    int adcValue = readADC();
    int pppm = convertADCToPPPM(adcValue);
    displayAlert(pppm > THRESHOLD_PPPM);

    delay(1000); // Espera antes de la próxima lectura
  }

  // Opcional: Realiza cualquier limpieza o transición necesaria antes de volver al menú principal
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
