#include "FlatusPedia.h"
#include "SoundPlayer.h"
#include <SD.h>
#include <FS.h>

extern SoundPlayer soundPlayer;

FlatusPedia::FlatusPedia() {

}

String FlatusPedia::readRandomFact() {
    File file = SD.open("/flatusPediaData.txt");
    if (!file) {
        Serial.println("Failed to open flatusPediaData.txt.");
        return "Error al abrir archivo";
    }

    int totalLines = 0;
    while (file.available()) {
        file.readStringUntil('\n');
        totalLines++;
    }

    // Vuelve al inicio del archivo
    file.seek(0);
    int randomLine = random(1, totalLines / 2) * 2 - 1; // Selecciona una línea impar aleatoria
    for (int i = 0; i < randomLine - 1; i++) {
        file.readStringUntil('\n'); // Avanza hasta la línea deseada
    }
    String fact = file.readStringUntil('\n');
    file.close();
    return fact;
}

void FlatusPedia::displayRandomFact() {
    String fact = readRandomFact();
    int colonIndex = fact.indexOf(':');
    String theme = fact.substring(0, colonIndex + 1);
    String detail = fact.substring(colonIndex + 2);

    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setTextSize(1.8);
    M5Cardputer.Display.setCursor(3, 5);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.println(theme);
    M5Cardputer.Display.setTextSize(1.5);
    M5Cardputer.Display.setCursor(3, 37);
    M5Cardputer.Display.setTextColor(GREEN);
    M5Cardputer.Display.println(detail);

    while (true) {
        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isKeyPressed('q')) {
            soundPlayer.play("/06pO.wav");
            break;
        }
    }
}
