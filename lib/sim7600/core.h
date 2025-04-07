#ifndef CELULAR_MODULE_H
#define CELULAR_MODULE_H

#include <Arduino.h>

class CelularModule {
public:
    CelularModule(unsigned long baudRate, uint8_t rxPin, uint8_t txPin);
    void init();

    void sendRaw(const char* data, bool newline = true);
    bool sendCommand(const char* command, const char* expectedResponse, unsigned int timeout = 1000, bool newline = true);
    bool sendCommand(const char* command, unsigned int timeout = 1000);
    const char* readResponse(const char* expectedResponse, unsigned int timeout = 1000);
    const char* getResponse();
    

private:
    HardwareSerial mCelularModule;
    bool isReady(const char* response, uint16_t timeout);
    unsigned long uartBaud;
    char mResponse[512];
    uint8_t pinRX;
    uint8_t pinTX;
    const uint8_t PWR_PIN = 4;
    const uint8_t LED_PIN = 12;
    const uint8_t POWER_PIN = 25;
};

#endif // CELULAR_MODULE_H
