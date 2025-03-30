#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;
#define g0(x) get<0>(x)
#define g1(x) get<1>(x)
#define g2(x) get<2>(x)
#define g3(x) get<3>(x)

typedef tuple<int, int, int, bool> tiiib;
typedef tuple<int, int, bool> tiib;
typedef pair<int, bool> pib;

struct pair_hash {
    template <typename T1, typename T2>
    std::size_t operator ()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

string reverse_complement(string seq);
unordered_map<string, pib> build_hash(string seq);
bool compare_tuples(const tiiib& a, const tiiib& b);
void merge_sort(vector<tiiib>& arr, int left, int right);
void merge(vector<tiiib>& arr, int left, int mid, int right);
vector<tiib> find_segments(const string& query, const unordered_map<string, pib>& hash_table);
void print(const vector<tiiib>& results);

#endif