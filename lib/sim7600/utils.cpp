#include "Utils.h"
#include <cstring>
#include "iostream"
#include "cmath"

bool Utils::extractAfterColon(const char* buffer, char* result, int maxLen) {
    const char* start = strchr(buffer, ':');
    if (!start) return false;

    start++; 

    
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

bool Utils::extractHttpJsonPayload(const char* buffer, char* result, int maxLen) {
    const char* start = strstr(buffer, "+HTTPREAD:");
    if (!start) return false;

    
    start = strchr(start, '\n');
    if (!start) return false;
    start++; 

    
    const char* end = strstr(start, "+HTTPREAD:");
    if (!end) return false;

    int len = end - start;
    if (len <= 0 || len >= maxLen) return false;

    strncpy(result, start, len);
    result[len] = '\0';
    return true;
}

bool Utils::extractMqttTopicAndPayload(const char* buffer, char* topic, int topicLen, char* payload, int payloadLen) {
    const char* topicStart = strstr(buffer, "+CMQTTRXTOPIC:");
    if (!topicStart) return false;

    topicStart = strchr(topicStart, '\n');
    if (!topicStart) return false;
    topicStart++; 

    const char* topicEnd = strchr(topicStart, '\n');
    if (!topicEnd) return false;

    int tLen = topicEnd - topicStart;
    if (tLen <= 0 || tLen >= topicLen) return false;

    strncpy(topic, topicStart, tLen);
    topic[tLen] = '\0';

    
    const char* payloadStart = strstr(topicEnd, "+CMQTTRXPAYLOAD:");
    if (!payloadStart) return false;

    payloadStart = strchr(payloadStart, '\n');
    if (!payloadStart) return false;
    payloadStart++; 

    const char* payloadEnd = strstr(payloadStart, "+CMQTTRXEND:");
    if (!payloadEnd) return false;

    int pLen = payloadEnd - payloadStart;
    if (pLen <= 0 || pLen >= payloadLen) return false;

    strncpy(payload, payloadStart, pLen);
    payload[pLen] = '\0';

    return true;
}

bool Utils::extractLineBetweenNewlines(const char* buffer, char* result, int maxLen) {
    const char* firstNewline = strchr(buffer, '\n');
    if (!firstNewline) return false;

    const char* start = firstNewline + 1;

    
    if (*start == '\r') start++;

    const char* end = strchr(start, '\n');
    if (!end) return false;

    int len = end - start;
    if (len >= maxLen) len = maxLen - 1;

    strncpy(result, start, len);
    result[len] = '\0';
    return true;
}

static double convertToDecimalDegrees(const char* coord, const char* dir) {
    if (!coord || !dir) return 0.0;

    double raw = atof(coord);
    double degrees = floor(raw / 100.0);
    double minutes = raw - (degrees * 100.0);
    double decimalDegrees = degrees + (minutes / 60.0);

    if (dir[0] == 'S' || dir[0] == 'W') {
        decimalDegrees = -decimalDegrees;
    }

    return decimalDegrees;
}

static void formatDateTime(const char* date, const char* time, char* output, size_t len) {
    if (!date || !time || strlen(date) < 6 || strlen(time) < 6) {
        snprintf(output, len, "00:00:00 00/00/00");
        return;
    }

    char hour[3] = { time[0], time[1], '\0' };
    char minute[3] = { time[2], time[3], '\0' };
    char second[3] = { time[4], time[5], '\0' };

    char day[3] = { date[0], date[1], '\0' };
    char month[3] = { date[2], date[3], '\0' };
    char year[3] = { date[4], date[5], '\0' };

    snprintf(output, len, "%s:%s:%s %s/%s/%s", hour, minute, second, day, month, year);
}

bool Utils::extractDateTimeFromCCLK(const char* buffer, char* result, int maxLen) {
    const char* cclkStart = strstr(buffer, "+CCLK:");
    if (!cclkStart) return false;

    const char* quoteStart = strchr(cclkStart, '\"');
    if (!quoteStart) return false;
    quoteStart++; // Skip opening quote

    const char* plusSign = strchr(quoteStart, '+');
    if (!plusSign) return false;

    int len = plusSign - quoteStart; // up to but not including '+'
    if (len <= 0 || len >= maxLen) return false;

    char temp[64];
    strncpy(temp, quoteStart, len);
    temp[len] = '\0';

    // Replace comma with space
    char* comma = strchr(temp, ',');
    if (comma) *comma = ' ';

    strncpy(result, temp, maxLen);
    result[maxLen - 1] = '\0';
    return true;
}

bool Utils::parseGPGPSInfo(const char* buffer, Coordinates& result) {
    char temp[128];
    std::strncpy(temp, buffer, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';

    const char* start = std::strchr(temp, ':');
    if (!start) return false;

    start++; // Skip ':'
    while (*start == ' ') start++; // Skip spaces

    char* fields[10] = {nullptr};
    int fieldIndex = 0;
    char* token = std::strtok(const_cast<char*>(start), ",");

    while (token && fieldIndex < 10) {
        fields[fieldIndex++] = token;
        token = std::strtok(nullptr, ",");
    }

    if (fieldIndex < 8) return false;

    result.latitude = convertToDecimalDegrees(fields[0], fields[1]);
    result.longitude = convertToDecimalDegrees(fields[2], fields[3]);

    formatDateTime(fields[4], fields[5], result.formattedDateTime, sizeof(result.formattedDateTime));

    result.speed = std::atof(fields[7]);
    result.altitude = std::atof(fields[6]);

    return true;
}