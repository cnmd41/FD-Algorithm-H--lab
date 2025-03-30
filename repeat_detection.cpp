#include "repeat_detection.h"
#include "utils.h"
#include <unordered_map>
#include <string>

using namespace std;

vector<tiiib> detect_repeats(const vector<tiib>& segments) {
    unordered_map<string, int> count_map;
    for (const auto& seg : segments) {
        string key = to_string(get<0>(seg)) + "_" + to_string(get<1>(seg)) + "_" + (get<2>(seg) ? "1" : "0");
        count_map[key]++;
    }

    vector<tiiib> result;
    unordered_map<string, tiiib> stats;

    for (const auto& small : segments) {
        bool contained = false;
        for (const auto& large : segments) {
            if (get<0>(large) <= get<0>(small) && get<1>(small) <= get<1>(large) && 
                !(get<0>(large) == get<0>(small) && get<1>(large) == get<1>(small) && get<2>(large) == get<2>(small))) {
                contained = true;
                break;
            }
        }

        if (contained) {
            int len = get<1>(small) - get<0>(small) + 1;
            string key = to_string(get<0>(small)) + "_" + to_string(get<1>(small)) + "_" + (get<2>(small) ? "1" : "0");
            int count = count_map[key];
            string stat_key = to_string(get<1>(small)) + "_" + to_string(len) + "_" + (get<2>(small) ? "1" : "0");
            if (!stats.count(stat_key)) stats[stat_key] = {get<1>(small), len, count, get<2>(small)};
        }
    }

    for (const auto& res : stats) result.push_back(res.second);

    merge_sort(result, 0, result.size() - 1);

    return result;
}