#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include "core.h"
#include "utils.h"

class GPSModule
{
public:
    GPSModule(CelularModule &celular);
    bool activeGPS();
    bool deactivateGPS();
    bool isActiveGPS();
    bool getGPS();
    Coordinates getCoordinates();

private:
    CelularModule &celularRef;
    Coordinates coords;
};

#endif
