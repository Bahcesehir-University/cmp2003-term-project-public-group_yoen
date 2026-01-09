#pragma once
#include <string>
#include <vector>

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

    std::vector<ZoneCount> topZones(int k = 10);
    std::vector<SlotCount> topBusySlots(int k = 10);
};
