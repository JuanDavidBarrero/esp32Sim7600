#ifndef SMSCLIENT_H
#define SMSCLIENT_H

#include <Arduino.h>
#include "core.h"


class SmsClient {
    public:
        SmsClient(CelularModule& celular);
        bool sendSMS(const char*, const char*);
        bool listSMS();
        bool deleteSMS(int index);
        bool deleteAllSMS();
        bool readSMS(int index);
    
    private:
        CelularModule& celularRef;
        char mResponse[20];
    };
    

#endif


