#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct ZoneCount {
    std::string zone;
    long long count;
};

struct SlotCount {
    std::string zone;
    int hour;
    long long count;
};

class TripAnalyzer {
public:
    void ingestStdin();
    void ingestFile(std::string filename);

    std::vector<ZoneCount> topZones(int k = 10);
    std::vector<SlotCount> topBusySlots(int k = 10);

private:
    std::unordered_map<std::string, long long> zoneFreq;
    std::unordered_map<std::string, long long> slotFreq;

    bool extractHour(const std::string& s, int& h);
    void processLine(const std::string& line);
};
