#include "Utils.h"
#include <cstring>

bool Utils::extractAfterColon(const char* buffer, char* result, int maxLen) {
    const char* start = strchr(buffer, ':');
    if (!start) return false;

    start++; // avanzar después de ':'

    // Omitir espacios en blanco
    while (*start == ' ') start++;

    int i = 0;
    while (*start && *start != '\r' && *start != '\n' && i < maxLen - 1) {
        result[i++] = *start++;
    }

    result[i] = '\0';
    return i > 0;
}

bool Utils::extractCSVValue(const char* buffer, int targetIndex, char* result, int maxLen) {
    int currentIndex = 0;
    const char* start = buffer;
    const char* end = nullptr;

    while (*start && currentIndex < targetIndex) {
        if (*start == ',') {
            currentIndex++;
        }
        start++;
    }

    if (currentIndex != targetIndex) return false;

    end = start;
    while (*end && *end != ',' && *end != '\r' && *end != '\n') {
        end++;
    }

    int len = end - start;
    if (len <= 0 || len >= maxLen) return false;

    strncpy(result, start, len);
    result[len] = '\0';
    return true;
}

// Extrae la parte entre las dos líneas +HTTPREAD:
bool Utils::extractHttpJsonPayload(const char* buffer, char* result, int maxLen) {
    const char* start = strstr(buffer, "+HTTPREAD:");
    if (!start) return false;

    // Buscar el salto de línea después del primer +HTTPREAD:
    start = strchr(start, '\n');
    if (!start) return false;
    start++; // saltar al comienzo de la línea del JSON

    // Buscar la segunda ocurrencia de +HTTPREAD:
    const char* end = strstr(start, "+HTTPREAD:");
    if (!end) return false;

    int len = end - start;
    if (len <= 0 || len >= maxLen) return false;

    strncpy(result, start, len);
    result[len] = '\0';
    return true;
}

// Extrae el tópico y el payload JSON de la respuesta CMQTTRX
bool Utils::extractMqttTopicAndPayload(const char* buffer, char* topic, int topicLen, char* payload, int payloadLen) {
    const char* topicStart = strstr(buffer, "+CMQTTRXTOPIC:");
    if (!topicStart) return false;

    topicStart = strchr(topicStart, '\n');
    if (!topicStart) return false;
    topicStart++; // saltar a la línea con el tópico

    const char* topicEnd = strchr(topicStart, '\n');
    if (!topicEnd) return false;

    int tLen = topicEnd - topicStart;
    if (tLen <= 0 || tLen >= topicLen) return false;

    strncpy(topic, topicStart, tLen);
    topic[tLen] = '\0';

    // Buscar payload
    const char* payloadStart = strstr(topicEnd, "+CMQTTRXPAYLOAD:");
    if (!payloadStart) return false;

    payloadStart = strchr(payloadStart, '\n');
    if (!payloadStart) return false;
    payloadStart++; // Saltar a la línea con el JSON

    const char* payloadEnd = strstr(payloadStart, "+CMQTTRXEND:");
    if (!payloadEnd) return false;

    int pLen = payloadEnd - payloadStart;
    if (pLen <= 0 || pLen >= payloadLen) return false;

    strncpy(payload, payloadStart, pLen);
    payload[pLen] = '\0';

    return true;
}
