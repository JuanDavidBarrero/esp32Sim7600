    #ifndef GPS_H
    #define GPS_H

    #include <Arduino.h>
    #include "core.h"


    class GPSModule {
        public:
            GPSModule(CelularModule& celular);
            bool activeGPS();
            bool deactivateGPS();
            bool isActiveGPS();
            String getGPS();
        
        private:
            CelularModule& celularRef;
            int mState = 0;
            int mTry = 0;
            bool mSuccess = false;
        };
        

    #endif
