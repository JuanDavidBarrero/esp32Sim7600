#include "gps.h"
#include "core.h"

GPSModule::GPSModule(CelularModule &celular): celularRef(celular){}

 bool GPSModule::activeGPS()
{
    bool reponse = celularRef.sendCommand("AT+CGPS=1","OK",1000);
    Serial.println(celularRef.getResponse());
    return reponse;
}

bool GPSModule::deactivateGPS()
{
    bool reponse = celularRef.sendCommand("AT+CGPS=0","OK",1000);
    Serial.println(celularRef.getResponse());
    return reponse;
}

 bool GPSModule::isActiveGPS()
{
    bool reponse = celularRef.sendCommand("AT+CGPS?","+CGPS: 1,1",2000);
    Serial.println(celularRef.getResponse());
    return reponse;
}

 String GPSModule::getGPS()
{
    bool reponse = celularRef.sendCommand("AT+CGPSINFO","+CGPSINFO: ",3000);
    return String();
}
