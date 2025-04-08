#include "ssl_config.h"

SslConfig::SslConfig(CelularModule& celular)
    : celularRef(celular) {}

bool SslConfig::showList() {
    celularRef.sendCommand("AT+CCERTLIST", 2000);
    Serial.println("");
    Serial.println(celularRef.getResponse());
    return 1;
}

bool SslConfig::deleteCert(const char* name) {
    char command[64];
    snprintf(command, sizeof(command), "AT+CCERTDELE=\"%s\"", name);
    return celularRef.sendCommand(command, "OK", 2000);
}

bool SslConfig::downloadCert(const char* name, const uint8_t* data, int length) {
    char command[64];
    snprintf(command, sizeof(command), "AT+CCERTDOWN=\"%s\",%d", name, length);

    if (!celularRef.sendCommand(command, ">", 2000)) {
        return false;
    }

    if (!celularRef.sendCommand((const char*)data, "OK", 3000, false)) {
        return false;
    }
    
    return true;
}

bool SslConfig::activateServerandClient(const char* caCert, const char* clientCert, const char* clientKey) {
   
    if (!celularRef.sendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", 1000)) {
        return false;
    }

    if (!celularRef.sendCommand("AT+CSSLCFG=\"authmode\",0,2", "OK", 1000)) {
        return false;
    }

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CSSLCFG=\"cacert\",0,\"%s\"", caCert);
    if (!celularRef.sendCommand(cmd, "OK", 1000)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd), "AT+CSSLCFG=\"clientcert\",0,\"%s\"", clientCert);
    if (!celularRef.sendCommand(cmd, "OK", 1000)) {
        return false;
    }

    snprintf(cmd, sizeof(cmd), "AT+CSSLCFG=\"clientkey\",0,\"%s\"", clientKey);
    if (!celularRef.sendCommand(cmd, "OK", 1000)) {
        return false;
    }

    return true;
}

bool SslConfig::serverVerify(const char* serverCaName) {
    if (!celularRef.sendCommand("AT+CSSLCFG=\"sslversion\",0,4", "OK", 1000)) {
        return false;
    }

    if (!celularRef.sendCommand("AT+CSSLCFG=\"authmode\",0,1", "OK", 1000)) {
        return false;
    }

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CSSLCFG=\"cacert\",0,\"%s\"", serverCaName);
    if (!celularRef.sendCommand(cmd, "OK", 1000)) {
        return false;
    }

    return true;
}
