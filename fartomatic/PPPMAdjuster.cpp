#include "PPPMAdjuster.h"
#include <M5Unified.h>
#include <M5Cardputer.h>
#include "SoundPlayer.h"

PPPMAdjuster::PPPMAdjuster() : thresholdPPPM(nullptr), adjustmentComplete(false) {}
extern SoundPlayer soundPlayer;
void PPPMAdjuster::begin() {
    M5Cardputer.Lcd.setTextFont(1);
    M5Cardputer.Lcd.setTextSize(2);
    M5Cardputer.Lcd.setCursor(0, 0);
    displayCurrentThreshold();
}

void PPPMAdjuster::setThreshold(int& thresholdPPPM) {
    this->thresholdPPPM = &thresholdPPPM;
    displayCurrentThreshold();
}

void PPPMAdjuster::update() {
    M5Cardputer.update();
    if (M5Cardputer.Keyboard.isChange()) {
        if (M5Cardputer.Keyboard.isPressed()) {
            Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
            for (auto i : status.word) {
                if (i == 'q') {
                    soundPlayer.play("/07pSostenido.wav");
                    adjustmentComplete = true;
                    inMainMenu = true;
                    runningPPPMAdjuster = false;
                    return; // Sale de la función para evitar procesar más entradas
                }
                // Maneja la entrada numérica y la tecla de borrado
                if (i != 0 && i != 'q') { // Ignora 'q' y el carácter nulo
                    inputBuffer += i;
                }
            }
            if (status.del && !inputBuffer.isEmpty()) {
                inputBuffer.remove(inputBuffer.length() - 1); // Borra el último carácter
            }
            clearInputArea();
            displayInputBuffer();
            if (status.enter && !inputBuffer.isEmpty() && thresholdPPPM != nullptr) {
                int newValue = inputBuffer.toInt();
                *thresholdPPPM = newValue;
                displayCurrentThreshold(); // Muestra el umbral actualizado
                inputBuffer = ""; // Limpia el buffer de entrada para el próximo valor
                clearInputArea(); // Limpia el área de entrada
                displayInputBuffer(); // Prepara para nueva entrada
            }
        }
    }
}


void PPPMAdjuster::displayInputBuffer() {
    // Esta función asume que se ha limpiaddo el área de entrada con clearInputArea()
    M5Cardputer.Lcd.setCursor(0, 140);  // Ajusta según sea necesario
    M5Cardputer.Lcd.setTextSize(2);
    M5Cardputer.Lcd.print("Nuevo valor PPPM: " + inputBuffer);
}


void PPPMAdjuster::displayCurrentThreshold() {
    M5Cardputer.Lcd.fillScreen(BLACK);
    M5Cardputer.Lcd.setCursor(0, 0);
    if (thresholdPPPM != nullptr) {
        M5Cardputer.Lcd.setTextSize(2);
        M5Cardputer.Lcd.println("Particulas de Pedo\nPor Millon (PPPM)");
        M5Cardputer.Lcd.printf("\nUmbral Actual: %d\n", *thresholdPPPM);
        M5Cardputer.Lcd.println();
        M5Cardputer.Lcd.println("ESCRIBE NUEVO PPPM:\nCONFIRMAR --> ENTER\nSALIR     -->  'q'");
    }
    clearInputArea();
}

void PPPMAdjuster::clearInputArea() {
    int inputAreaY = 140;
    M5Cardputer.Lcd.fillRect(0, inputAreaY, 320, 30, BLACK);
    M5Cardputer.Lcd.setCursor(0, inputAreaY);
    M5Cardputer.Lcd.setTextSize(2);
    M5Cardputer.Lcd.print("Nuevo valor PPPM: " + inputBuffer);
}

bool PPPMAdjuster::isAdjustmentComplete() {
    return adjustmentComplete;
}

void PPPMAdjuster::resetAdjustmentComplete() {
    adjustmentComplete = false;
}
