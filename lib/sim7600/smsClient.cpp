#include "smsClient.h"
#include "utils.h"

SmsClient::SmsClient(CelularModule &celular): celularRef(celular)
{
}

bool SmsClient::sendSMS(const char * number, const char* msg)
{
    char buffer[256];

    if (!celularRef.sendCommand("AT+CMGF=1", "OK", 1000)) {
        return false;
    }

    sprintf(buffer, "AT+CMGS=\"%s\"", number);
    if (!celularRef.sendCommand(buffer, ">", 1000)) {
        return false;
    }

    sprintf(buffer, "%s\x1A", msg);
    if (!celularRef.sendCommand(buffer, "OK", 10000)) {
        return false;
    }

    return true;
}

bool SmsClient::listSMS()
{
    if (!celularRef.sendCommand("AT+CMGF=1", "OK", 1000)) {
        Serial.println("Fallo al poner en modo texto");
        return false;
    }

    if (!celularRef.sendCommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000)){
        return false;
    }

    if (!celularRef.sendCommand("AT+CMGL=\"ALL\"", "OK", 2000)) {
        Serial.println("No se pudieron listar los mensajes");
        return false;
    }

    // Serial.println("Mensajes:");
    // Serial.println(celularRef.getResponse());

    return true;
}


bool SmsClient::deleteSMS(int index)
{
    char cmd[32];
    sprintf(cmd, "AT+CMGD=%d", index);

    if (!celularRef.sendCommand(cmd, "OK", 1000)) {
        return false;
    }

    return true;
}

bool SmsClient::deleteAllSMS()
{
    if (!celularRef.sendCommand("AT+CMGD=1,4", "OK", 2000)) {
        return false;
    }

    return true;
}

bool SmsClient::readSMS(int index)
{
    char cmd[32];
    sprintf(cmd, "AT+CMGR=%d", index);

    if (!celularRef.sendCommand("AT+CPMS=\"SM\",\"SM\",\"SM\"", "OK", 1000)){
        return false;
    }

    if (!celularRef.sendCommand(cmd, "OK", 5000)) {
        return false;
    }

    char number[32];
    char msg[100];
    const char* response = celularRef.getResponse();

    Utils::extractCSVValue(response,1,number,sizeof(number));
    Utils::extractLineBetweenNewlines(response,msg,sizeof(msg));

    Serial.print("El celular --->");
    Serial.print(number);
    Serial.print(" mando el mensajes --->");
    Serial.println(msg);

    return true;
}

