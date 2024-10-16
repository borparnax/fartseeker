#ifndef PPPMADJUSTER_H
#define PPPMADJUSTER_H

#include <M5Unified.h>

extern bool inMainMenu;
extern bool runningPPPMAdjuster;


class PPPMAdjuster {
public:
    PPPMAdjuster();
    void begin();
    void update();
    void setThreshold(int& thresholdPPPM);
    bool isAdjustmentComplete();
    void resetAdjustmentComplete();
private:
    int* thresholdPPPM;
    String inputBuffer;
    void processInput();
    void displayCurrentThreshold();
    void clearInputArea();
    void displayInputBuffer();
    bool adjustmentComplete;
};

#endif
