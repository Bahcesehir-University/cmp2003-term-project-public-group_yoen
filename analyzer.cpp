#include "analyzer.h"
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cctype>

using namespace std;

static unordered_map<string, long long> zoneFreq;
static unordered_map<string, long long> slotFreq;

static bool extractHour(const string& s, int& h) {
    if (s.size() < 13) return false;
    if (!isdigit(s[11]) || !isdigit(s[12])) return false;
    h = (s[11] - '0') * 10 + (s[12] - '0');
    return h >= 0 && h <= 23;
}

void TripAnalyzer::ingestStdin() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;

        vector<string> cols;
        string token;
        stringstream ss(line);

        while (getline(ss, token, ',')) {
            cols.push_back(token);
        }

        if (cols.size() != 6) continue;

        int hour;
        if (!extractHour(cols[3], hour)) continue;

        const string& zone = cols[1];

        zoneFreq[zone]++;
        slotFreq[zone + "#" + to_string(hour)]++;
    }
}

vector<ZoneCount> TripAnalyzer::topZones(int k) {
    vector<ZoneCount> out;
    out.reserve(zoneFreq.size());

    for (auto& it : zoneFreq)
        out.push_back({it.first, it.second});

    sort(out.begin(), out.end(), [](auto& a, auto& b) {
        if (a.count != b.count) return a.count > b.count;
        return a.zone < b.zone;
    });

    if ((int)out.size() > k) out.resize(k);
    return out;
}

vector<SlotCount> TripAnalyzer::topBusySlots(int k) {
    vector<SlotCount> out;
    out.reserve(slotFreq.size());

    for (auto& it : slotFreq) {
        size_t pos = it.first.find('#');
        out.push_back({
            it.first.substr(0, pos),
            stoi(it.first.substr(pos + 1)),
            it.second
        });
    }

    sort(out.begin(), out.end(), [](auto& a, auto& b) {
        if (a.count != b.count) return a.count > b.count;
        if (a.zone != b.zone) return a.zone < b.zone;
        return a.hour < b.hour;
    });

    if ((int)out.size() > k) out.resize(k);
    return out;
}
