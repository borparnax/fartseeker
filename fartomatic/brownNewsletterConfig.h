// brownNewsletterConfig.h

#ifndef BROWN_NEWSLETTER_CONFIG_H
#define BROWN_NEWSLETTER_CONFIG_H

#include <Arduino.h>
#include <Preferences.h>  // Add this line
#include <vector>
#include <WiFi.h>
#include <M5Cardputer.h>
#include <ESP_Mail_Client.h>

// Define the WiFiNetwork struct
struct WiFiNetwork {
    String ssid;
    String password;
};

class BrownNewsletterConfig {
public:
    BrownNewsletterConfig();
    void begin();
    void showMenu();
    void sendNotificationEmail();

    String getTextInput(const String &prompt);

private:
    void loadConfiguration();
    void saveNetworks();
    void saveEmailSender();
    void saveBrownList();
    void saveNewsletterStatus();

    void configureWiFi();
    void displaySavedNetworks();
    void addWiFiNetwork();
    void removeWiFiNetwork();

    void configureEmailSender();
    void configureBrownList();
    void addEmailToBrownList();
    void removeEmailFromBrownList();
    void toggleNewsletter();

    bool connectToWiFi();

    // Member variables
    Preferences preferences;
    bool newsletterOn;
    String emailSender;
    std::vector<WiFiNetwork> savedNetworks;
    std::vector<String> brownList;

    ESP_Mail_Session sessionConfig;
    SMTPSession smtp;
};

#endif // BROWN_NEWSLETTER_CONFIG_H
