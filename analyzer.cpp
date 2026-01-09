#include "analyzer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

void TripAnalyzer::processLine(const string& line) {
    if (line.empty()) return;

    vector<string> cols;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        cols.push_back(token);
    }

    string zone;
    string timeStr;

    if (cols.size() == 3) {
        zone = cols[1];
        timeStr = cols[2];
    } 
    else if (cols.size() >= 6) {
        zone = cols[1];
        timeStr = cols[3];
    } 
    else {
        return;
    }

    if (timeStr.size() < 13) return;

    int hour = -1;
    if (isdigit(timeStr[11]) && isdigit(timeStr[12])) {
        hour = (timeStr[11] - '0') * 10 + (timeStr[12] - '0');
    }

    if (hour < 0 || hour > 23) return;

    zoneFreq[zone]++;
    slotFreq[zone + "#" + to_string(hour)]++;
}

void TripAnalyzer::ingestStdin() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        processLine(line);
    }
}

void TripAnalyzer::ingestFile(const std::string& csvPath) {
    zoneFreq.clear();
    slotFreq.clear();

    ifstream file(csvPath);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.find("TripID") != string::npos) continue;
        processLine(line);
    }
    file.close();
}

vector<ZoneCount> TripAnalyzer::topZones(int k) const {
    vector<ZoneCount> out;
    out.reserve(zoneFreq.size());

    for (const auto& pair : zoneFreq)
        out.push_back({pair.first, pair.second});

    sort(out.begin(), out.end(), [](const ZoneCount& a, const ZoneCount& b) {
        if (a.count != b.count) return a.count > b.count;
        return a.zone < b.zone;
    });

    if ((int)out.size() > k) out.resize(k);
    return out;
}

vector<SlotCount> TripAnalyzer::topBusySlots(int k) const {
    vector<SlotCount> out;
    out.reserve(slotFreq.size());

    for (const auto& pair : slotFreq) {
        size_t pos = pair.first.find('#');
        out.push_back({
            pair.first.substr(0, pos),
            stoi(pair.first.substr(pos + 1)),
            pair.second
        });
    }

    sort(out.begin(), out.end(), [](const SlotCount& a, const SlotCount& b) {
        if (a.count != b.count) return a.count > b.count;
        if (a.zone != b.zone) return a.zone < b.zone;
        return a.hour < b.hour;
    });

    if ((int)out.size() > k) out.resize(k);
    return out;
}
