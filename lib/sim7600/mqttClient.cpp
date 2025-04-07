#include "mqttClient.h"

MqttClient::MqttClient(CelularModule &celular) : celularRef(celular) {}

void MqttClient::setBrokerUrl(const char *url)
{
    strncpy(brokerUrl, url, sizeof(brokerUrl) - 1);
    brokerUrl[sizeof(brokerUrl) - 1] = '\0';
}

void MqttClient::setPort(int port)
{
    brokerPort = port;
}

bool MqttClient::connectMQTT()
{

    char fullUrl[128];
    snprintf(fullUrl, sizeof(fullUrl), "tcp://%s:%d", brokerUrl, brokerPort);

    char connectCmd[160];
    snprintf(connectCmd, sizeof(connectCmd), "AT+CMQTTCONNECT=0,\"%s\",60,1", fullUrl);

    return celularRef.sendCommand(connectCmd, "+CMQTTCONNECT: 0,0", 10000);
}

void MqttClient::startMqtt()
{
    bool startResponse = celularRef.sendCommand("AT+CMQTTSTART", "+CMQTTSTART: 0", 5000);
}

void MqttClient::setClient(const char* id, bool mqttWithTls)
{
    strncpy(clientId, id, sizeof(clientId) - 1);
    clientId[sizeof(clientId) - 1] = '\0';
    clientIdLen = strlen(clientId);

    char cmd[160];
    if (mqttWithTls) {
        snprintf(cmd, sizeof(cmd), "AT+CMQTTACCQ=0,\"%s\",1", clientId);  // 1 activa SSL
    } else {
        snprintf(cmd, sizeof(cmd), "AT+CMQTTACCQ=0,\"%s\"", clientId);
    }

    if (!celularRef.sendCommand(cmd, "OK", 2000)) {
        return;
    }

    if (mqttWithTls) {
        if (!celularRef.sendCommand("AT+CMQTTSSLCFG=0,0", "OK", 1000)) {
            return;
        }
    }
}


void MqttClient::setLastWillTopic(const char *topic)
{
    strncpy(lastWillTopic, topic, sizeof(lastWillTopic) - 1);
    lastWillTopic[sizeof(lastWillTopic) - 1] = '\0';
    lastWillTopicLen = strlen(lastWillTopic);
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMQTTWILLTOPIC=0,%u", lastWillTopicLen);
    bool response = celularRef.sendCommand(cmd, ">", 2000);
    delay(50);
    if (response)
    {
        celularRef.sendCommand(lastWillTopic, "OK");
    }
}

void MqttClient::setLastWillMsg(const char *msg)
{
    strncpy(lastWillMsg, msg, sizeof(lastWillMsg) - 1);
    lastWillMsg[sizeof(lastWillMsg) - 1] = '\0';
    lastWillMsgLen = strlen(lastWillMsg);
    char command[64];
    snprintf(command, sizeof(command), "AT+CMQTTWILLMSG=0,%d,1", lastWillMsgLen);
    bool response = celularRef.sendCommand(command, ">", 2000);
    delay(50);
    if (response)
    {
        celularRef.sendCommand(lastWillMsg, "OK");
    }
}

void MqttClient::publish(const char *topic, const char *msg)
{
    strncpy(publishTopic, topic, sizeof(publishTopic) - 1);
    publishTopic[sizeof(publishTopic) - 1] = '\0';
    publishTopicLen = strlen(publishTopic);

    strncpy(publishMsg, msg, sizeof(publishMsg) - 1);
    publishMsg[sizeof(publishMsg) - 1] = '\0';
    publishMsgLen = strlen(publishMsg);

    char topicCmd[64];
    snprintf(topicCmd, sizeof(topicCmd), "AT+CMQTTTOPIC=0,%d", publishTopicLen);
    if (!celularRef.sendCommand(topicCmd, ">", 2000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTTOPIC");
        return;
    }

    delay(100);

    if (!celularRef.sendCommand(publishTopic, "OK", 2000))
    {
        Serial.println("[MQTT] Falló envío del topic");
        return;
    }

    delay(10);

    char payloadCmd[64];
    snprintf(payloadCmd, sizeof(payloadCmd), "AT+CMQTTPAYLOAD=0,%d", publishMsgLen);
    if (!celularRef.sendCommand(payloadCmd, ">", 2000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTPAYLOAD");
        return;
    }

    delay(100);

    if (!celularRef.sendCommand(publishMsg, "OK", 2000))
    {
        Serial.println("[MQTT] Falló envío del mensaje");
        return;
    }

    // Enviar publicación final
    if (!celularRef.sendCommand("AT+CMQTTPUB=0,1,60", "+CMQTTPUB: 0,0", 5000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTPUB");
        return;
    }

}

bool MqttClient::disconnect()
{

    if (!celularRef.sendCommand("AT+CMQTTDISC=0,120", "+CMQTTDISC: 0,0", 5000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTDISC");
        return false;
    }
    delay(10);

    if (!celularRef.sendCommand("AT+CMQTTREL=0", "OK", 2000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTREL");
        return false;
    }
    delay(10);

    if (!celularRef.sendCommand("AT+CMQTTSTOP", "+CMQTTSTOP: 0", 5000))
    {
        Serial.println("[MQTT] Falló AT+CMQTTSTOP");
        return false;
    }

    return true;
}

bool MqttClient::subscribe(const char *topic)
{
    int topicLen = strlen(topic);

    char cmd[64];
    snprintf(cmd, sizeof(cmd), "AT+CMQTTSUB=0,%d,1", topicLen);

    if (!celularRef.sendCommand(cmd, ">", 2000))
    {
        return false;
    }

    delay(50);

    if (!celularRef.sendCommand(topic, "+CMQTTSUB: 0,0", 2000))
    {
        return false;
    }

    return true;
}

void MqttClient::readMsg() {

    bool ok = celularRef.sendCommand("AT", "OK", 2000); // Esperás la respuesta del módem

}
