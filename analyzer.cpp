#include "analyzer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>

using namespace std;

static unordered_map<string, long long> zoneCount;
static unordered_map<string, long long> zoneHourCount;

void TripAnalyzer::ingestStdin() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;

        string col;
        stringstream ss(line);
        vector<string> cols;

        while (getline(ss, col, ',')) {
            cols.push_back(col);
        }

        if (cols.size() != 6) continue;
        if (cols[3].size() < 13) continue;

        int hour;
        try {
            hour = stoi(cols[3].substr(11, 2));
        }
        catch (...) {
            continue;
        }

        if (hour < 0 || hour > 23) continue;

        const string& zone = cols[1];

        ++zoneCount[zone];
        ++zoneHourCount[zone + "#" + to_string(hour)];
    }
}

void TripAnalyzer::ingestFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        string col;
        stringstream ss(line);
        vector<string> cols;

        while (getline(ss, col, ',')) {
            cols.push_back(col);
        }

        if (cols.size() != 6) continue;
        if (cols[3].size() < 13) continue;

        int hour;
        try {
            hour = stoi(cols[3].substr(11, 2));
        }
        catch (...) {
            continue;
        }

        if (hour < 0 || hour > 23) continue;

        const string& zone = cols[1];

        ++zoneCount[zone];
        ++zoneHourCount[zone + "#" + to_string(hour)];
    }
}

vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    vector<ZoneCount> result;
    result.reserve(zoneCount.size());

    for (const auto& p : zoneCount) {
        result.push_back({ p.first, p.second });
    }

    sort(result.begin(), result.end(),
        [](const ZoneCount& a, const ZoneCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            return a.zone < b.zone;
        });

    if ((int)result.size() > k)
        result.resize(k);

    return result;
}

vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    vector<SlotCount> result;
    result.reserve(zoneHourCount.size());

    for (const auto& p : zoneHourCount) {
        size_t pos = p.first.find('#');
        string zone = p.first.substr(0, pos);
        int hour = stoi(p.first.substr(pos + 1));

        result.push_back({ zone, hour, p.second });
    }

    sort(result.begin(), result.end(),
        [](const SlotCount& a, const SlotCount& b) {
            if (a.count != b.count)
                return a.count > b.count;
            if (a.zone != b.zone)
                return a.zone < b.zone;
            return a.hour < b.hour;
        });

    if ((int)result.size() > k)
        result.resize(k);

    return result;
}
