// brownNewsletterConfig.cpp

#include "brownNewsletterConfig.h"

BrownNewsletterConfig::BrownNewsletterConfig() : newsletterOn(false) {}

void BrownNewsletterConfig::begin() {
    preferences.begin("brownConfig", false);
    loadConfiguration();
}

void BrownNewsletterConfig::loadConfiguration() {
    // Cargar WiFi guardadas
    size_t networkCount = preferences.getUInt("networkCount", 0);
    for (size_t i = 0; i < networkCount; ++i) {
        String ssid = preferences.getString(("ssid" + String(i)).c_str(), "");
        String password = preferences.getString(("pass" + String(i)).c_str(), "");
        if (ssid != "") {
            savedNetworks.push_back({ssid, password});
        }
    }

    // Cargar Email Sender
    emailSender = preferences.getString("emailSender", "");

    // Cargar Brown List
    size_t brownListSize = preferences.getUInt("brownListSize", 0);
    for (size_t i = 0; i < brownListSize; ++i) {
        String email = preferences.getString(("brownEmail" + String(i)).c_str(), "");
        if (email != "") {
            brownList.push_back(email);
        }
    }

    // Cargar estado de Newsletter ON/OFF
    newsletterOn = preferences.getBool("newsletterOn", false);
}

void BrownNewsletterConfig::saveNetworks() {
    preferences.putUInt("networkCount", savedNetworks.size());
    for (size_t i = 0; i < savedNetworks.size(); ++i) {
        preferences.putString(("ssid" + String(i)).c_str(), savedNetworks[i].ssid);
        preferences.putString(("pass" + String(i)).c_str(), savedNetworks[i].password);
    }
}

void BrownNewsletterConfig::saveEmailSender() {
    preferences.putString("emailSender", emailSender);
}

void BrownNewsletterConfig::saveBrownList() {
    preferences.putUInt("brownListSize", brownList.size());
    for (size_t i = 0; i < brownList.size(); ++i) {
        preferences.putString(("brownEmail" + String(i)).c_str(), brownList[i]);
    }
}

void BrownNewsletterConfig::saveNewsletterStatus() {
    preferences.putBool("newsletterOn", newsletterOn);
}

String BrownNewsletterConfig::getTextInput(const String &prompt) {
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setCursor(0, 0);
    M5Cardputer.Display.println(prompt);

    String input = "";
    while (true) {
        M5Cardputer.update();
        for (char c = 32; c < 127; c++) { // Caracteres imprimibles
            if (M5Cardputer.Keyboard.isKeyPressed(c)) {
                if (c == '\n' || c == '\r' || c == KEY_ENTER) {
                    return input;
                } else if (c == '\b') { // Retroceso
                    if (input.length() > 0) {
                        input.remove(input.length() - 1);
                        // Actualiza la pantalla para reflejar el cambio
                        M5Cardputer.Display.fillRect(0, 20, M5Cardputer.Display.width(), M5Cardputer.Display.height() - 20, BLACK);
                        M5Cardputer.Display.setCursor(0, 20);
                        M5Cardputer.Display.print(input);
                    }
                } else {
                    input += c;
                    M5Cardputer.Display.print(c);
                }
                delay(100); // Pequeño retardo para evitar rebotes
            }
        }
        if (M5Cardputer.Keyboard.isKeyPressed(KEY_ENTER)) {
            return input;
        }
    }
}

void BrownNewsletterConfig::showMenu() {
    bool exit = false;
    while (!exit) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setTextSize(2);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Brown Newsletter Config:");
        M5Cardputer.Display.println("1. WiFi Settings");
        M5Cardputer.Display.println("2. Email Sender");
        M5Cardputer.Display.println("3. Brown List");
        M5Cardputer.Display.println("4. Newsletter ON/OFF");
        M5Cardputer.Display.println("q. Back to Main Menu");

        char key = 0;
        while (true) {
            M5Cardputer.update();
            if (M5Cardputer.Keyboard.isKeyPressed('1')) {
                key = '1';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('2')) {
                key = '2';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('3')) {
                key = '3';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('4')) {
                key = '4';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('q')) {
                key = 'q';
                break;
            }
        }

        switch (key) {
            case '1':
                configureWiFi();
                break;
            case '2':
                configureEmailSender();
                break;
            case '3':
                configureBrownList();
                break;
            case '4':
                toggleNewsletter();
                break;
            case 'q':
                exit = true;
                break;
        }
    }
}


void BrownNewsletterConfig::configureWiFi() {
    bool exit = false;
    while (!exit) {
        // Código para mostrar el menú...

        char key = 0;
        while (true) {
            M5Cardputer.update();
            if (M5Cardputer.Keyboard.isKeyPressed('1')) {
                key = '1';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('2')) {
                key = '2';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('3')) {
                key = '3';
                break;
            } else if (M5Cardputer.Keyboard.isKeyPressed('q')) {
                key = 'q';
                break;
            }
        }

        switch (key) {
            case '1':
                displaySavedNetworks();
                break;
            case '2':
                addWiFiNetwork();
                break;
            case '3':
                removeWiFiNetwork();
                break;
            case 'q':
                exit = true;
                break;
        }
    }
}

void BrownNewsletterConfig::displaySavedNetworks() {
    bool exit = false;
    while (!exit) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setTextSize(2);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Saved WiFis:");
        int y = 30;
        for (size_t i = 0; i < savedNetworks.size(); ++i) {
            M5Cardputer.Display.setCursor(0, y);
            M5Cardputer.Display.printf("%d. %s\n", i + 1, savedNetworks[i].ssid.c_str());
            y += 20;
        }
        M5Cardputer.Display.println("\nPress 'q' to go back.");

        M5Cardputer.update();
        if (M5Cardputer.Keyboard.isKeyPressed('q')) {
            exit = true;
        }
    }
}

void BrownNewsletterConfig::addWiFiNetwork() {
    String ssid = getTextInput("Enter SSID (or 'q' to cancel):");
    if (ssid == "q") return;

    String password = getTextInput("Enter Password (or 'q' to cancel):");
    if (password == "q") return;

    savedNetworks.push_back({ssid, password});
    saveNetworks();

    M5Cardputer.Display.println("\nWiFi added!");
    delay(1000);
}

void BrownNewsletterConfig::removeWiFiNetwork() {
    if (savedNetworks.empty()) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("No WiFis saved.");
        delay(1000);
        return;
    }

    bool exit = false;
    while (!exit) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setTextSize(2);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Select WiFi to remove:");
        int y = 30;
        for (size_t i = 0; i < savedNetworks.size(); ++i) {
            M5Cardputer.Display.setCursor(0, y);
            M5Cardputer.Display.printf("%d. %s\n", i + 1, savedNetworks[i].ssid.c_str());
            y += 20;
        }
        M5Cardputer.Display.println("\nPress number or 'q' to cancel.");

        bool keyPressed = false;
        char key = 0;

        while (!keyPressed) {
            M5Cardputer.update();
            for (char c = '1'; c <= '9'; ++c) {
                if (M5Cardputer.Keyboard.isKeyPressed(c)) {
                    key = c;
                    keyPressed = true;
                    break;
                }
            }
            if (M5Cardputer.Keyboard.isKeyPressed('q')) {
                key = 'q';
                keyPressed = true;
            }
            delay(100); // Pequeño retardo para evitar rebotes
        }

        if (key >= '1' && key <= '9') {
            int index = key - '1';
            if (index < savedNetworks.size()) {
                savedNetworks.erase(savedNetworks.begin() + index);
                saveNetworks();
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(0, 0);
                M5Cardputer.Display.println("WiFi removed!");
                delay(1000);
                exit = true;
            } else {
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(0, 0);
                M5Cardputer.Display.println("Invalid selection.");
                delay(1000);
            }
        } else if (key == 'q') {
            exit = true;
        }
    }
}


void BrownNewsletterConfig::configureEmailSender() {
    String newEmail = getTextInput("Enter new Email or 'q' to cancel:");
    if (newEmail == "q") return;

    if (newEmail != "") {
        emailSender = newEmail;
        saveEmailSender();
        M5Cardputer.Display.println("\nEmail Sender updated!");
        delay(1000);
    }
}

void BrownNewsletterConfig::configureBrownList() {
    bool exit = false;
    while (!exit) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setTextSize(2);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Brown List:");
        M5Cardputer.Display.println("1. View Brown List");
        M5Cardputer.Display.println("2. Add Email");
        M5Cardputer.Display.println("3. Remove Email");
        M5Cardputer.Display.println("q. Back");

        char key = 0;
        bool keyPressed = false;

        while (!keyPressed) {
            M5Cardputer.update();
            if (M5Cardputer.Keyboard.isKeyPressed('1')) {
                key = '1';
                keyPressed = true;
            } else if (M5Cardputer.Keyboard.isKeyPressed('2')) {
                key = '2';
                keyPressed = true;
            } else if (M5Cardputer.Keyboard.isKeyPressed('3')) {
                key = '3';
                keyPressed = true;
            } else if (M5Cardputer.Keyboard.isKeyPressed('q')) {
                key = 'q';
                keyPressed = true;
            }
            delay(100); // Pequeño retardo para evitar rebotes
        }

        switch (key) {
            case '1':
                // View Brown List
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(0, 0);
                M5Cardputer.Display.println("Brown List:");
                for (size_t i = 0; i < brownList.size(); ++i) {
                    M5Cardputer.Display.printf("%d. %s\n", i + 1, brownList[i].c_str());
                }
                M5Cardputer.Display.println("\nPress 'q' to go back.");
                while (true) {
                    M5Cardputer.update();
                    if (M5Cardputer.Keyboard.isKeyPressed('q')) break;
                    delay(100);
                }
                break;
            case '2':
                addEmailToBrownList();
                break;
            case '3':
                removeEmailFromBrownList();
                break;
            case 'q':
                exit = true;
                break;
        }
    }
}


void BrownNewsletterConfig::addEmailToBrownList() {
    String email = getTextInput("Enter Email to add (or 'q' to cancel):");
    if (email == "q") return;

    if (email != "") {
        brownList.push_back(email);
        saveBrownList();
        M5Cardputer.Display.println("\nEmail added!");
        delay(1000);
    }
}

void BrownNewsletterConfig::removeEmailFromBrownList() {
    if (brownList.empty()) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Brown List is empty.");
        delay(1000);
        return;
    }

    bool exit = false;
    while (!exit) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Select Email to remove:");
        for (size_t i = 0; i < brownList.size(); ++i) {
            M5Cardputer.Display.printf("%d. %s\n", i + 1, brownList[i].c_str());
        }
        M5Cardputer.Display.println("\nPress number or 'q' to cancel.");

        char key = 0;
        bool keyPressed = false;

        while (!keyPressed) {
            M5Cardputer.update();
            for (char c = '1'; c <= '9'; ++c) {
                if (M5Cardputer.Keyboard.isKeyPressed(c)) {
                    key = c;
                    keyPressed = true;
                    break;
                }
            }
            if (M5Cardputer.Keyboard.isKeyPressed('q')) {
                key = 'q';
                keyPressed = true;
            }
            delay(100); // Pequeño retardo para evitar rebotes
        }

        if (key >= '1' && key <= '9') {
            int index = key - '1';
            if (index < brownList.size()) {
                brownList.erase(brownList.begin() + index);
                saveBrownList();
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(0, 0);
                M5Cardputer.Display.println("Email removed!");
                delay(1000);
                exit = true;
            } else {
                M5Cardputer.Display.fillScreen(BLACK);
                M5Cardputer.Display.setCursor(0, 0);
                M5Cardputer.Display.println("Invalid selection.");
                delay(1000);
            }
        } else if (key == 'q') {
            exit = true;
        }
    }
}


void BrownNewsletterConfig::toggleNewsletter() {
    newsletterOn = !newsletterOn;
    saveNewsletterStatus();
    M5Cardputer.Display.fillScreen(BLACK);
    M5Cardputer.Display.setCursor(0, 0);
    M5Cardputer.Display.println(newsletterOn ? "Newsletter is ON" : "Newsletter is OFF");
    delay(1000);
}

bool BrownNewsletterConfig::connectToWiFi() {
    for (const auto& network : savedNetworks) {
        WiFi.begin(network.ssid.c_str(), network.password.c_str());
        unsigned long startAttemptTime = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
            delay(100);
        }
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
    }
    return false;
}

void BrownNewsletterConfig::sendNotificationEmail() {
    if (!newsletterOn || emailSender.isEmpty() || brownList.empty()) {
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        if (!connectToWiFi()) {
            M5Cardputer.Display.fillScreen(BLACK);
            M5Cardputer.Display.setCursor(0, 0);
            M5Cardputer.Display.println("Failed to connect to WiFi.");
            delay(2000);
            return;
        }
    }

    // Crear una sesión SMTP
    SMTPSession smtp;

    // Configurar el servidor SMTP y las credenciales
    ESP_Mail_Session session;
    session.server.host_name = "smtp.gmail.com";
    session.server.port = 465; // Puerto para SSL
    session.login.email = emailSender.c_str();
    session.login.password = "tu_contraseña_de_aplicación"; // Usa la contraseña de aplicación generada
    session.secure.startTLS = false; // No es necesario para SSL

    // Habilitar SSL
    smtp.setSecure(true);

    // Conectar al servidor SMTP
    if (!smtp.connect(&session)) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Failed to connect to SMTP server.");
        delay(2000);
        return;
    }

    // Configurar el mensaje
    SMTP_Message message;
    message.sender.name = "Fart Seeker";
    message.sender.email = emailSender.c_str();
    message.subject = "FartSeeker detectó gas metano";

    // Añadir destinatarios
    for (const auto& recipient : brownList) {
        message.addRecipient("", recipient.c_str());
    }

    // Establecer el contenido del mensaje
    message.text.content = "El FartSeeker detectó gas metano en el ambiente.";
    message.text.charSet = "utf-8";
    message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

    // Enviar el correo electrónico
    if (!smtp.sendMail(&message)) {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Error sending email:");
        M5Cardputer.Display.println(smtp.errorReason().c_str());
        delay(2000);
    } else {
        M5Cardputer.Display.fillScreen(BLACK);
        M5Cardputer.Display.setCursor(0, 0);
        M5Cardputer.Display.println("Notification email sent!");
        delay(2000);
    }

    // Cerrar la sesión SMTP
    smtp.closeSession();
}
