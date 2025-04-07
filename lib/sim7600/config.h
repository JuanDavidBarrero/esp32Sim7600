#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "core.h"


class ConfigModule {
    public:
        ConfigModule(CelularModule& celular);
        bool isReadySIM();
        bool setAPN(const char *apn);
        void configCelular();
        const char* getIMEI();
    
    private:
        CelularModule& celularRef;
        int mState = 0;
        int mTry = 0;
        bool mSuccess = false;
        char mResponse[20];
    };
    

#endif


