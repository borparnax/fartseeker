//fartomatic.ino
#include <M5Cardputer.h>
#include <M5GFX.h>
#include <Wire.h>
#include <SD.h>
#include "FartSeeker.h"
#include <M5Unified.h>
#include "SoundPlayer.h"
#include "PPPMAdjuster.h"
#include <Arduino.h>  
#include "FlatusPedia.h"
//#include "brownNewsletterConfig.h"

//BrownNewsletterConfig brownConfig;

#define ADC_I2C_ADDRESS 0x48
FlatusPedia flatusPedia;
int THRESHOLD_PPPM = 1150;
PPPMAdjuster pppmAdjuster;
bool runningPPPMAdjuster = false;
SoundPlayer soundPlayer;
struct MENU {
  const char* name;
  void (*action)();
};
//void brownNewsletterConfig() {   }


void displayFlatusPediaFact() {
    flatusPedia.displayRandomFact();
}
void adjustPPPMThreshold();
void displayFlatusPediaFact();
void howMuchBat();
void about();
MENU mainMenu[] = {
  { "Activar FartSeeker", activateFartSeeker },
  //{ "Brown Newsletter Configuration", []() { brownConfig.showMenu(); } },
  { "Ajuste Umbral PPPM", adjustPPPMThreshold },
  { "Random FlatusPedia", displayFlatusPediaFact },
  { "Bateria Disponible", howMuchBat },
  { "About Fart-O-Matic", about }
};
const int mainMenuSize = sizeof(mainMenu) / sizeof(MENU);
int currentMenuSelection = 0;
bool inMainMenu = false;
bool waitingForEnterPress = true;
bool checkNextPress();
bool checkSelectPress();
void drawMenu();
void playSound(const char* filename);

void setup() {
    Serial.begin(115200); // Initialize Serial for debugging
    M5.begin();           // Initialize M5Stack components
    M5Cardputer.begin();  // Initialize M5Cardputer

    Wire.begin();
    SD.begin();           // Initialize SD card

    pppmAdjuster.begin();
    pppmAdjuster.setThreshold(THRESHOLD_PPPM);
    M5Cardputer.Display.setRotation(1);
    soundPlayer.begin();
    randomSeed(analogRead(0));
    M5.Speaker.setVolume(255);
    showWelcomeMessage();

  //  brownConfig.begin();
}
void loop() {
  M5Cardputer.update();
  if (runningPPPMAdjuster) {
    pppmAdjuster.update();
    if (pppmAdjuster.isAdjustmentComplete() || !runningPPPMAdjuster) {
      soundPlayer.play("/07pSostenido.wav");
      pppmAdjuster.resetAdjustmentComplete();
      runningPPPMAdjuster = false;
      inMainMenu = true;
      drawMenu();
    }
    return;
  }
  if (!inMainMenu && M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER) && waitingForEnterPress) {

    inMainMenu = true;
    drawMenu();
  } else if (inMainMenu) {
    if (checkNextPress() || checkSelectPress()) {
    }
  }
  if (!runningFartSeeker && !inMainMenu) {
    inMainMenu = true;
    drawMenu();
  }
}
void playSound(const char* filename) {
  soundPlayer.play(filename);
}
void adjustPPPMThreshold() {
  pppmAdjuster.begin();
  pppmAdjuster.setThreshold(THRESHOLD_PPPM);

  while (!pppmAdjuster.isAdjustmentComplete()) {
    M5.update();
    pppmAdjuster.update();
  }

  pppmAdjuster.resetAdjustmentComplete();
  inMainMenu = true;
  drawMenu();
}


void showWelcomeMessage() {
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setTextColor(GREEN, BLACK);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setCursor(5, 30);
  M5Cardputer.Display.println("Henlo");
  M5Cardputer.Display.setTextSize(3);
  M5Cardputer.Display.setCursor(20, 50);
  M5Cardputer.Display.println("Fart Seeker");
  M5Cardputer.Display.setTextSize(1);
  M5Cardputer.Display.setCursor(40, 100);
  M5Cardputer.Display.println("Press ENTER to continue...");
  playSound("/01henloFartSeeker.wav");
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
      playSound("/02pOk.wav");
      showLoadingScreen();

      break;
    }
  }
}

void drawMenu() {
  M5Cardputer.Display.fillScreen(BLACK);
  int startY = 10;
  for (int i = 0; i < mainMenuSize; ++i) {
    M5Cardputer.Display.setTextSize(2.2);
    M5Cardputer.Display.setCursor(4, startY + (20 * i));
    M5Cardputer.Display.setTextColor(i == currentMenuSelection ? RED : GREEN, BLACK);
    M5Cardputer.Display.println(mainMenu[i].name);
  }
}

bool checkNextPress() {
  static unsigned long lastPressTime = 0;
  const unsigned long debounceTime = 250;
  M5Cardputer.update();
  bool hasPressed = false;
  unsigned long currentTime = millis();
  if ((currentTime - lastPressTime) > debounceTime) {
    if (M5Cardputer.Keyboard.isKeyPressed('w') || M5Cardputer.Keyboard.isKeyPressed(';')) {
      currentMenuSelection = (currentMenuSelection ? currentMenuSelection - 1 : mainMenuSize - 1);
      hasPressed = true;
    } else if (M5Cardputer.Keyboard.isKeyPressed('s') || M5Cardputer.Keyboard.isKeyPressed('.')) {
      currentMenuSelection = (currentMenuSelection + 1) % mainMenuSize;
      hasPressed = true;
    }
    if (hasPressed) {
      drawMenu();
      lastPressTime = currentTime;
    }
  }
  return hasPressed;
}
bool checkSelectPress() {
  static unsigned long lastPressTime = 0;
  const unsigned long debounceTime = 250;
  M5Cardputer.update();
  unsigned long currentTime = millis();
  if ((currentTime - lastPressTime) > debounceTime) {
    if (M5Cardputer.Keyboard.isKeyPressed('d') || M5Cardputer.Keyboard.isKeyPressed('/') || M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
      playSound("/02pOk.wav");
      mainMenu[currentMenuSelection].action();
      inMainMenu = false;
      lastPressTime = currentTime;
      return true;
    }
  }
  return false;
}
  /// BATTERY INFO ///
void battery_setup() {
  showBatteryLevel();
}
void showBatteryLevel() {
  float batteryLevel = M5.Power.getBatteryLevel();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 15);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("* Nivel de Bateria *");
  // Configuración de la barra de progreso
  int progressBarWidth = 200;
  int progressBarHeight = 10;
  int x = (M5.Lcd.width() - progressBarWidth) / 2;
  int y = M5.Lcd.height() / 2 - progressBarHeight / 2; // Centrar verticalmente

  // Dibujar contorno de la barra de progreso
  M5.Lcd.drawRect(x, y, progressBarWidth, progressBarHeight, WHITE);

  // Calcular ancho de la barra de progreso basado en el nivel de batería
  int fillWidth = (int)(batteryLevel * progressBarWidth / 100.0);
  M5.Lcd.fillRect(x, y, fillWidth, progressBarHeight, GREEN);

  // Mostrar el porcentaje de batería como texto
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(GREEN, BLACK);
  String percentText = String(batteryLevel) + "%";
  int textWidth = 12 * percentText.length();
  int textX = (M5.Lcd.width() - textWidth) / 2; // Centrar horizontalmente
  int textY = y + progressBarHeight + 5; // Posición debajo de la barra
  M5.Lcd.setCursor(textX, textY);
  M5.Lcd.print(percentText);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(10, M5.Lcd.height() - 20);
  M5.Lcd.println("PRESIONA 'q' PARA IR A MENU PRINCIPAL");
}

void howMuchBat() {
  battery_setup();
  while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isKeyPressed('q')) {
       soundPlayer.play("/05pGrave.wav");
      inMainMenu = true;
      drawMenu();
      break;
    }
  }
}
void showLoadingScreen() {
  M5Cardputer.Display.fillScreen(BLACK);
  M5Cardputer.Display.setTextColor(GREEN);
  M5Cardputer.Display.setTextSize(2);
  M5Cardputer.Display.setCursor(10, 10);
  M5Cardputer.Display.println("CARGANDO...");
  int progressBarWidth = 200;
  int progressBarHeight = 10;
  int x = (M5Cardputer.Display.width() - progressBarWidth) / 2;
  int y = M5Cardputer.Display.height() / 2;
  for (int i = 0; i <= 100; i += 1) {
    M5Cardputer.Display.drawRect(x, y, progressBarWidth, progressBarHeight, WHITE);
    M5Cardputer.Display.fillRect(x, y, (i * progressBarWidth) / 100, progressBarHeight, GREEN);
    M5Cardputer.Display.setTextSize(2);
    M5Cardputer.Display.setTextColor(GREEN, BLACK);
    String percentText = String(i) + "%";
    int textWidth = 12 * percentText.length();
    int textX = M5Cardputer.Display.width() - textWidth - 10;
    int textY = M5Cardputer.Display.height() - 20;
    M5Cardputer.Display.fillRect(textX, textY, textWidth, 20, BLACK);
    M5Cardputer.Display.setCursor(textX, textY);
    M5Cardputer.Display.print(percentText);
    if (i > 13 && i < 24) {
      delay(15);
    } else if (i > 49 && i < 64) {
      delay(10);
    } else if (i == 79 || i == 81 || i == 83) {
      delay(250);
    } else if (i == 95) {
      delay(1400);
    } else if (i == 98) {
      delay(1000);
    } else if (i == 99) {
      delay(650);
    } else {
      delay(50);
    }
  }
  M5Cardputer.Display.setTextSize(1);
  M5Cardputer.Display.setCursor(40, 93);
  M5Cardputer.Display.println("FEATURES CARGADAS CON EXITO!");
  delay(2000);
}
void about() {
  M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setTextSize(1.9);
    M5Cardputer.Display.setCursor(3, 5);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.println("FART-O-MATIC DETECTOR"); // Muestra la temática en mayúsculas
    M5Cardputer.Display.setTextSize(1.4);
    M5Cardputer.Display.setCursor(3, 27); // Ajusta según el tamaño del texto de la temática
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.println("");
    M5Cardputer.Display.println("");
    M5Cardputer.Display.println("More info at...");
    M5Cardputer.Display.println("https://github.com/theCubicleWizard"); // Muestra el hecho 


   while (true) {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isKeyPressed('q')) {
       playSound("/04pFin.wav");
      inMainMenu = true;
      drawMenu();
      break;
    }
  }
}
