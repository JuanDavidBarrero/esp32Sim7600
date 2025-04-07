#include "HttpClientModule.h"
#include "utils.h"

HttpClientModule::HttpClientModule(CelularModule &celular)
    : celularRef(celular) {}

bool HttpClientModule::activate()
{

    while (state < 4)
    {
        switch (state)
        {
        case 0:
            mSuccess = celularRef.sendCommand("AT+CPIN?", "+CPIN: READY", 2000);
            break;
        case 1:
            mSuccess = celularRef.sendCommand("AT+CGPADDR=1", "OK", 5000);

            break;
        case 2:
            mSuccess = celularRef.sendCommand("AT+CGACT=1,1", "OK", 5000);
            break;
        case 3:
            mSuccess = celularRef.sendCommand("AT+CGREG?", "+CGREG: 0,1", 2000);
            break;
        }

        if (mSuccess)
        {
            state++;
            tries = 0;
        }
        else
        {
            Serial.println("Error al activar la conexión de datos.");
            return false;
        }
    }

    Serial.println("Conexión de datos activada correctamente.");
    return true;
}

String HttpClientModule::getIP()
{
    bool response = celularRef.sendCommand("AT+CGPADDR=1", "+CGPADDR: ", 2000);
    if (response)
    {
        const String data = celularRef.getResponse();
        Utils::extractCSVValue(data.c_str(), 1, mResponse, sizeof(mResponse));
        return String(mResponse);
    }
    return "";
}

int HttpClientModule::getSignalQuality()
{
    bool response = celularRef.sendCommand("AT+CSQ", "+CSQ: ", 2000);

    if (response)
    {
        const String data = celularRef.getResponse();

        Utils::extractAfterColon(data.c_str(), mResponse, sizeof(mResponse));
        return atoi(mResponse);
    }

    return -1;
}

bool HttpClientModule::sendHttpGetTo(const String &url)
{
    String response;

    if (!celularRef.sendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", 2000))
    {
        return false;
    }

    if (!celularRef.sendCommand("AT+HTTPINIT", "OK", 2000))
    {
        return false;
    }

    {
        String urlCmd = "AT+HTTPPARA=\"URL\",\"" + url + "\"";
        if (!celularRef.sendCommand(urlCmd.c_str(), "OK", 2000))
        {
            return false;
        }
    }

    if (!celularRef.sendCommand("AT+HTTPACTION=0", "+HTTPACTION: 0,200", 15000))
    {
        return false;
    }
    else
    {
        const String data = celularRef.getResponse();
        Utils::extractCSVValue(data.c_str(), 2, mHttpLength, sizeof(mHttpLength));
    }

    {
        String readCmd = "AT+HTTPREAD=0," + String(mHttpLength);
        if (!celularRef.sendCommand(readCmd.c_str(), "+HTTPREAD: 0", 50000))
        {
            return false;
        }
        else
        {
            const String data = celularRef.getResponse();
            Utils::extractHttpJsonPayload(data.c_str(), mResponse, sizeof(mResponse));
        }
    }

    if (!celularRef.sendCommand("AT+HTTPTERM", "OK", 2000))
    {
        return false;
    }

    Serial.println("HTTP GET finalizado correctamente.");
    return true;
}

bool HttpClientModule::sendHttpPostTo(const String &url, const String &body, const String &contentType)
{
    if (!celularRef.sendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", 2000))
    {
        Serial.println("Error POST estado 0");
        return false;
    }

    if (!celularRef.sendCommand("AT+HTTPINIT", "OK", 2000))
    {
        Serial.println("Error POST estado 1");
        return false;
    }

    {
        String cmd = "AT+HTTPPARA=\"URL\",\"" + url + "\"";
        if (!celularRef.sendCommand(cmd.c_str(), "OK", 2000))
        {
            Serial.println("Error POST estado 2");
            return false;
        }
    }

    {
        String cmd = "AT+HTTPPARA=\"CONTENT\",\"" + contentType + "\"";
        if (!celularRef.sendCommand(cmd.c_str(), "OK", 2000))
        {
            Serial.println("Error POST estado 3");
            return false;
        }
    }

    {
        String cmd = "AT+HTTPDATA=" + String(body.length()) + ",10000";
        if (!celularRef.sendCommand(cmd.c_str(), "DOWNLOAD", 3000))
        {
            Serial.println("Error POST estado 4 (DOWNLOAD)");
            return false;
        }

        if (!celularRef.sendCommand(body.c_str(), "OK", 2000))
        {
            Serial.println("Error al enviar body POST");
            return false;
        }
    }

    if (!celularRef.sendCommand("AT+HTTPACTION=1", "+HTTPACTION: 1,200", 15000))
    {
        Serial.println("Error POST estado 5 (HTTPACTION)");
        return false;
    }
  
    if (!celularRef.sendCommand("AT+HTTPTERM", "OK", 2000))
    {
        Serial.println("Error POST estado 7 (HTTPTERM)");
        return false;
    }

    return true;
}

char *HttpClientModule::getResponse()
{
    return mResponse;
}
