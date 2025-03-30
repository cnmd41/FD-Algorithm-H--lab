#include "repeat_detection.h"
#include "utils.h"
#include <unordered_map>
#include <string>

using namespace std;

vector<tiiib> detect_repeats(vector<tiib> segments) {
    unordered_map<tuple<int, int, int>, int, tuple_hash> count_map;
    vector<tiiib> result;
    unordered_map<tuple<int, int, int>, tiiib, tuple_hash> map;

    for (const auto& seg : segments) {
        auto key = make_tuple(get<0>(seg), get<1>(seg), get<2>(seg));
        count_map[key]++;
    }
    for (const auto& temp1 : segments) {
        int flag = 0;
        for (const auto& temp2 : segments) {
            if (g0(temp2) <= g0(temp1) && g1(temp1) <= g1(temp2) && !(g0(temp2) == g0(temp1) && g1(temp2) == g1(temp1) && g2(temp2) == g2(temp1))) {
                flag = 1;
                break;
            }
        }
        if (flag) {
            auto key1 = make_tuple(g0(temp1), g1(temp1), g2(temp1));
            auto key2 = make_tuple(g1(temp1), g1(temp1) - g0(temp1) + 1, g2(temp1));
            if (!map.count(key2)) map[key2] = {g1(temp1), g1(temp1) - g0(temp1) + 1, count_map[key1], g2(temp1)};
        }
    }
    for (const auto& res : map) result.push_back(res.second);

    merge_sort(result, 0, result.size() - 1);

    return result;
}