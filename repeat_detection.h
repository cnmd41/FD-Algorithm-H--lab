#ifndef REPEAT_DETECTION_H
#define REPEAT_DETECTION_H

#include <vector>
#include <tuple>
#include "utils.h"

using namespace std;

struct tuple_hash {
    template <typename T1, typename T2, typename T3>
    std::size_t operator ()(const std::tuple<T1, T2, T3>& t) const {
        auto h1 = std::hash<T1>{}(std::get<0>(t));
        auto h2 = std::hash<T2>{}(std::get<1>(t));
        auto h3 = std::hash<T3>{}(std::get<2>(t));
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }
};

vector<tiiib> detect_repeats(vector<tiib> segments);

#endif