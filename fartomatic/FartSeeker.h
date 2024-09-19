// FartSeeker.h
#ifndef FARTSEEKER_H
#define FARTSEEKER_H

extern bool runningFartSeeker;
extern const char* alertSound;
extern int THRESHOLD_PPPM;

void activateFartSeeker();
void displayAlert(bool isGasDetected);
void playAlertSound();
int readADC();
int convertADCToPPPM(int adcValue);

#endif
