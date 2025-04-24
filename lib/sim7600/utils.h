#ifndef UTILS_H
#define UTILS_H

struct Coordinates {
    double latitude = 0.0;
    double longitude = 0.0;
    double speed = 0.0;     
    double altitude = 0.0;  
    char formattedDateTime[20] = {0}; 
};

class Utils {
public:
    static bool extractAfterColon(const char* buffer, char* result, int maxLen);
    static bool extractCSVValue(const char* buffer, int targetIndex, char* result, int maxLen);
    static bool extractHttpJsonPayload(const char* buffer, char* result, int maxLen);
    static bool extractMqttTopicAndPayload(const char* buffer, char* topic, int topicLen, char* payload, int payloadLen);
    static bool extractLineBetweenNewlines(const char* buffer, char* result, int maxLen);
    static bool extractDateTimeFromCCLK(const char* buffer, char* result, int maxLen);

    static bool parseGPGPSInfo(const char* buffer, Coordinates& result);
};

#endif
