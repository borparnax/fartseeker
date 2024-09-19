#ifndef ACTIVATEEMAILNOTIFICATIONS_H
#define ACTIVATEEMAILNOTIFICATIONS_H

#include <M5Cardputer.h> // Incluye la librería específica para M5Cardputer
#include <ESP_Mail_Client.h>

// Declara las funciones que serán definidas en activateEmailNotifications.cpp
void setupEmail();
void sendEmail(const String& subject, const String& message, const String& recipient);
void exampleSendEmail();
String waitForInput();

#endif // ACTIVATEEMAILNOTIFICATIONS_H