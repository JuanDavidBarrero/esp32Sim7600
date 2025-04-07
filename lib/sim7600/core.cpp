#include "core.h"
#include <string.h>

CelularModule::CelularModule(unsigned long baudRate, uint8_t rxPin, uint8_t txPin)
    : uartBaud(baudRate), pinRX(rxPin), pinTX(txPin), mCelularModule(1) {
    memset(mResponse, 0, sizeof(mResponse));
}

void CelularModule::init() {
    mCelularModule.begin(uartBaud, SERIAL_8N1, pinRX, pinTX);

    pinMode(LED_PIN, OUTPUT);
    pinMode(POWER_PIN, OUTPUT);
    pinMode(PWR_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(POWER_PIN, HIGH);
    digitalWrite(PWR_PIN, HIGH);
    delay(500);
    digitalWrite(PWR_PIN, LOW);

    if (this->isReady("PB DONE", 20000)) {
        Serial.println("\n====================");
        Serial.println("SIM7600 está listo.");
        Serial.println("====================\n\n");
    } else {
        Serial.println("No se recibió respuesta del SIM7600.");
    }
}

bool CelularModule::isReady(const char* response, uint16_t timeout) {
    uint32_t startTime = millis();
    while ((millis() - startTime) < timeout) {
        if (mCelularModule.available()) {
            String receivedString = mCelularModule.readStringUntil('\n');
            if (strncmp(response, receivedString.c_str(), strlen(response)) == 0) {
                return true;
            }
        }
    }
    return false;
}

void CelularModule::sendRaw(const char* data,  bool newline) {
    if(newline){
        mCelularModule.println(data); 
        return;
    }
    mCelularModule.write(data);
}

bool CelularModule::sendCommand(const char* command, unsigned int timeout) {
    Serial.print("cmd -> ");
    Serial.print(command);
    sendRaw(command);
    delay(100);
    readResponse("", timeout);
    return true; 
}

bool CelularModule::sendCommand(const char* command, const char* expectedResponse, unsigned int timeout, bool newline) {
    const uint8_t maxRetries = 3;
    uint8_t attempts = 0;

    enum State { SEND, WAIT, SUCCESS, FAILURE };
    State state = SEND;

    while (state != FAILURE) {
        switch (state) {
            case SEND:
                Serial.print("command -> ");
                Serial.println(command);
                sendRaw(command,newline);
                state = WAIT;
                break;

            case WAIT:
                readResponse(expectedResponse, timeout);
                Serial.print("mResponse -> ");
                Serial.println(mResponse);
                Serial.print("================ \n\n");

                if (strstr(mResponse, expectedResponse) != nullptr) {
                    state = SUCCESS;
                } else {
                    attempts++;
                    state = (attempts >= maxRetries) ? FAILURE : SEND;
                }
                break;

            case SUCCESS:
                return true;

            case FAILURE:
                return false;
        }
        delay(100);
    }

    return false;
}

const char* CelularModule::readResponse(const char* expectedResponse, unsigned int timeout) {
    memset(mResponse, 0, sizeof(mResponse));
    unsigned long start = millis();
    size_t totalLen = 0;
    bool waitFullTime = (strlen(expectedResponse) == 0);  // si vacío, no salgas anticipadamente

    while (millis() - start < timeout) {
        while (mCelularModule.available()) {
            String line = mCelularModule.readStringUntil('\n');
            line.trim();

            if (line.length() > 0) {
                size_t len = line.length();
                if (totalLen + len + 1 < sizeof(mResponse)) {
                    strncat(mResponse, line.c_str(), len);
                    strcat(mResponse, "\n");
                    totalLen += len + 1;
                }

                if (!waitFullTime && (line.indexOf(expectedResponse) != -1 || line == "ERROR")) {
                    return mResponse;
                }
            }
        }
    }

    return mResponse;
}

const char* CelularModule::getResponse() {
    return mResponse;
}
