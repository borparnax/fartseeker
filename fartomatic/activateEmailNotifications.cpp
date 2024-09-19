//activateEmailNotifications.cpp
#include "activateEmailNotifications.h"
#include <M5Cardputer.h>
#include <WiFi.h>
#include "ESP_Mail_Client.h"

// Configura los detalles de tu red WiFi
const char* ssid = "yourSSID";
const char* password = "yourPassword";

// Configura los detalles de la sesión SMTP
#define SMTP_HOST "smtp.example.com"
#define SMTP_PORT 587
#define AUTHOR_EMAIL "yourEmail@example.com"
#define AUTHOR_PASSWORD "yourEmailPassword"

// Inicializa la sesión SMTP y el cliente de correo
SMTPSession smtp;
ESP_Mail_Session session;

void setup() {
  Serial.begin(115200);

  // Conecta a la red WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" conectado");

  // Configura los detalles de la sesión SMTP
  session.server.host_name = SMTP_HOST;
  session.server.port = SMTP_PORT;
  session.login.email = AUTHOR_EMAIL;
  session.login.password = AUTHOR_PASSWORD;

  // Opcional: Configura las opciones de seguridad y autenticación
  // Esto depende de tu servidor SMTP
  session.server.starttls = true; // Habilita STARTTLS si es necesario

  // Configura el mensaje de correo electrónico
  SMTP_Message message;
  message.sender.name = "Sender Name";
  message.sender.email = AUTHOR_EMAIL;
  message.subject = "ESP-Mail-Client Test";
  message.addRecipient("Recipient Name", "recipient@example.com");

  // Configura el cuerpo del mensaje
  String textMsg = "Hello from ESP-Mail-Client!";
  message.text.content = textMsg.c_str();

  // Envía el correo electrónico
  if (!smtp.connect(&session)) {
    Serial.println("Error al conectar al servidor SMTP");
    return;
  }

  if (!MailClient.sendMail(&smtp, &message)) {
    Serial.println("Error al enviar el correo electrónico");
  } else {
    Serial.println("Correo electrónico enviado exitosamente");
  }
}

void loop() {
  // Tu código de loop aquí
}