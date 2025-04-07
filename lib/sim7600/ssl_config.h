#ifndef SSL_CONFIG_H
#define SSL_CONFIG_H

#include "core.h"

class SslConfig {
    public:
        SslConfig(CelularModule& celular);
        bool showList();
        bool deleteCert(const char* name);
        bool downloadCert(const char* name, const uint8_t* data, int length);  
        bool serverVerify(const char* serverCaName);
        bool activateServerandClient(const char* caCert, const char* clientCert, const char* clientKey);


    private:
        CelularModule& celularRef;
};

#endif
