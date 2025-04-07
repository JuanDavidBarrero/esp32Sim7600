#ifndef HTTP_CLIENT_MODULE_H
#define HTTP_CLIENT_MODULE_H

#include <Arduino.h>
#include "core.h"

class HttpClientModule {
public:
    HttpClientModule(CelularModule& celular);

    bool activate();
    String getIP();
    int getSignalQuality();
    bool sendHttpGetTo(const String& url);
    char* getResponse();
    bool sendHttpPostTo(const String &url, const String &body, const String &contentType);

private:
    CelularModule& celularRef;
    int state = 0;
    int tries = 0;
    const int maxTries = 3;
    bool mSuccess=false;
    char mResponse[2048];
    char mHttpLength[5];
};

#endif
