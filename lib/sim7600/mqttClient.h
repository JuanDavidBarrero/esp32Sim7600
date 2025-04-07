#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include <Arduino.h>
#include <string.h>
#include "core.h"

class MqttClient
{
public:
    MqttClient(CelularModule &celular);

    bool connectMQTT();
    void startMqtt();
    void setBrokerUrl(const char *url);
    void setPort(int port);
    void setClient(const char *id,bool mqttWithTls);
    void setLastWillTopic(const char *topic);
    void setLastWillMsg(const char *msg);
    void publish(const char* topic, const char* msg);
    bool subscribe(const char* topic);
    bool disconnect();
    void readMsg();

private:
    CelularModule &celularRef;

    char brokerUrl[128] = {0};
    uint16_t brokerPort = 0;

    char clientId[128] = {0};
    uint8_t clientIdLen = 0;

    char lastWillTopic[128] = {0};
    uint8_t lastWillTopicLen = 0;

    char lastWillMsg[128] = {0};
    uint8_t lastWillMsgLen = 0;

    char publishTopic[128]; 
    int publishTopicLen;

    char publishMsg[256];
    uint16_t publishMsgLen;
};

#endif
