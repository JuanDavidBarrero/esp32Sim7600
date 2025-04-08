#ifndef UTILS_H
#define UTILS_H

class Utils {
public:
    static bool extractAfterColon(const char* buffer, char* result, int maxLen);
    static bool extractCSVValue(const char* buffer, int targetIndex, char* result, int maxLen);
    static bool extractHttpJsonPayload(const char* buffer, char* result, int maxLen);
    static bool extractMqttTopicAndPayload(const char* buffer, char* topic, int topicLen, char* payload, int payloadLen);
    static bool extractLineBetweenNewlines(const char* buffer, char* result, int maxLen);
};

#endif 
