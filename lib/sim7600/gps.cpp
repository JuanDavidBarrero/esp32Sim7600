#include "gps.h"
#include "core.h"

GPSModule::GPSModule(CelularModule &celular): celularRef(celular){}

 bool GPSModule::activeGPS()
{
    return celularRef.sendCommand("AT+CGPS=1","OK",1000);
}

bool GPSModule::deactivateGPS()
{
    return celularRef.sendCommand("AT+CGPS=0","+CGPS: 0",5000);
   
}

 bool GPSModule::isActiveGPS()
{
    return celularRef.sendCommand("AT+CGPS?","+CGPS: 1,1",2000);
 
}

bool GPSModule::getGPS()
{
    if (!celularRef.sendCommand("AT+CGPSINFO", "+CGPSINFO: ", 3000))
        return false;

    const char* data = celularRef.getResponse();
    return Utils::parseGPGPSInfo(data, coords);
}


Coordinates GPSModule::getCoordinates()
{
    return coords;
}
