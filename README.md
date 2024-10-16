Pensamiento sobre 5 segundos

markdown
Copiar código
# FartSeeker Firmware 🖥️💨

Bienvenido al repositorio oficial del FartSeeker, un custom firmware diseñado para el Cardputer de M5 Stack. Este proyecto convierte tu M5 Stack Cardputer en un sofisticado detector de metano.

## 🚧 Requisitos

**Hardware:**
- M5 Stack Cardputer 🖥️
- Sensor de Metano M.4 de Flying-Fish conectado al ADC de M5 Stack, que a su vez irá conectado a la M5 Stack Cardputer por el Grove port
- MicroSD para almacenar los archivos de sonido y la mini enciclopedia FlatusPedia.

**Software:**
- UIFLOW FIRMWARE BURNING TOOL: La herramienta recomendada para flashear tu nuevo, fresco y, un tanto descarado custom firmware de manera sencilla. Puedes descargarla [aquí](https://github.com/espressif/arduino-esp32/issues).

## 📂 Configuración

1. **Instalar el Firmware:**
   - Instala el firmware en tu M5 Stack usando la UIFLOW FIRMWARE BURNING TOOL.

2. **Preparar la Tarjeta MicroSD:**
   - Inserta una tarjeta MicroSD en tu computadora.
   - **Coloca los archivos de sonido** y el archivo `flatusPediaData.txt` **sin cambiar el nombre** dentro de la raíz de la tarjeta MicroSD.
   - Inserta la tarjeta MicroSD en la Cardputer de M5 Stack.

3. **Conectar el Sensor de Metano:**
   - Conecta el sensor de metano al ADC de la Cardputer M5 Stack mediante el Grove port.

## 🎛️ Menú Principal

El FartSeeker cuenta con un menú intuitivo para controlar todas sus funcionalidades:

- **Activar FartSeeker:** Monitorea la concentración de metano en el aire en tiempo real.
- **Brown Newsletter (En Proceso):** Pronto podrás configurar una lista de contactos para que sean notificados por correo cuando se detecte metano. 💨💻
- **Ajuste Umbral PPPM:** Modifica la sensibilidad del sensor según las Partículas de Pedo Por Millón (PPPM) acorde con la sensibilidad del usuario.
- **Random FlatusPedia:** Datos curiosos y aleatorios sobre gases expulsados por seres vivos. 🤓
- **Batería Disponible:** Visualiza el porcentaje exacto restante de batería junto con una barra de estado. 🔋
- **About Fart-O-Matic:** Información sobre el firmware y enlace directo a este repositorio.

## 🤝 Contribuciones

Cualquier mejora o contribución es bienvenida. No dudes en abrir issues o hacer pull requests. Si tienes sugerencias sobre nuevas funciones para el FartSeeker, ¡nos encantaría escucharlas!

Actualmente en la versión v0.9, próximamente la versión v1.0 estará disponible [aquí](https://github.com/theCubicleWizard/FartSeeker).

## 📄 Licencia

Este proyecto está licenciado bajo la **[Licencia Pública General GNU Versión 2 (GPLv2)](LICENSE)**. Consulta el archivo [LICENSE](LICENSE) para más detalles.

---

¡Gracias por usar FartSeeker! 💨🖥️
