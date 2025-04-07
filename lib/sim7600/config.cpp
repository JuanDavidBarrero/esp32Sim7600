#include "config.h"
#include "core.h"
#include "utils.h"

ConfigModule::ConfigModule(CelularModule &celular) : celularRef(celular) {}

bool ConfigModule::isReadySIM()
{
    return celularRef.sendCommand("AT+CPIN?", "+CPIN: READY", 1000);
}

bool ConfigModule::setAPN(const char *apn)
{
    String cmd = "AT+CGDCONT=1,\"IP\",\"";
    cmd += apn;
    cmd += "\"";

    bool success = celularRef.sendCommand(cmd.c_str(), "OK", 2000);

    return success;
}

void ConfigModule::configCelular()
{
    if (!celularRef.sendCommand("AT", "OK", 1000)) {
        return;
    }

    if (!celularRef.sendCommand("ATE0", "OK", 1000)) {
        return;
    }

    if (!celularRef.sendCommand("AT+CFUN=1", "OK", 1000)) {
        return;
    }

    if (!celularRef.sendCommand("AT&W", "OK", 1000)) {
        return;
    }

    Serial.println("Configurado exitosamente");
}

const char* ConfigModule::getIMEI()
{
    bool success = celularRef.sendCommand("AT+SIMEI?", "OK", 2000);
    if (success){
        String data = celularRef.getResponse();
        Utils::extractAfterColon(data.c_str(),mResponse,sizeof(mResponse));
        return mResponse;
    }
    return "";
}
