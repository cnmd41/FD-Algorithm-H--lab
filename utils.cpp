#include "utils.h"

using namespace std;

string reverse_complement(string s) {
    string t;
    for (int i = s.size() - 1; i >= 0; i--) t += s[i] == 'A' ? 'T' : s[i] == 'T' ? 'A' : s[i] == 'G' ? 'C' : 'G';
    return t;
}

unordered_map<string, pib> build_hash(string s) {
    unordered_map<string, pib> myhash;
    int len = s.size();
    string reverse = reverse_complement(s);
    for (int i = 0; i < len; i++) {
        string part;
        for (int j = i; j < len; j++) {
            part += s[j];
            myhash[part] = {i, 0};
        }
    }
    for (int i = 0; i < len; i++) {
        string part;
        for (int j = i; j < len; j++) {
            part += reverse[j];
            auto x = myhash.count(part);
            if (!x) myhash[part] = {len - 1 - j, 1};
        }
    }
    
    return myhash;
}

bool compare_tuples(const tiiib& a, const tiiib& b) {
    return g0(a) != g0(b) ? g0(a) < g0(b) : g1(a) != g1(b) ? g1(a) < g1(b) : g3(a) < g3(b);
}
void merge_sort(vector<tiiib>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}
void merge(vector<tiiib>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<tiiib> left_arr(n1);
    vector<tiiib> right_arr(n2);
    for (int i = 0; i < n1; i++) left_arr[i] = arr[left + i];
    for (int i = 0; i < n2; i++) right_arr[i] = arr[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare_tuples(left_arr[i], right_arr[j])) arr[k++] = left_arr[i++];
        else arr[k++] = right_arr[j++];
    }
    while (i < n1) arr[k++] = left_arr[i++];
    while (j < n2) arr[k++] = right_arr[j++];
}

vector<tiib> find_segments(string query, unordered_map<string, pib> myhash) {
    int len = query.size();
    vector<int> dp(len + 1, len + 1);
    dp[len] = 0;
    vector<pair<int, pib>> record(len + 1, {-1, {-1, 0}});
    unordered_map<pair<int, int>, int, pair_hash> cnt;
    vector<tiib> segments;

    for (int i = len - 1; i >= 0; i--) {
        string part;
        int min = -1;
        pair<int, pib> trace = {-1, {-1, 0}}; 
        for (int j = i; j < len; j++) {
            part += query[j];
            if (myhash.find(part) != myhash.end()) {
                int nxt = j + 1;
                int size = 1 + dp[nxt];
                pib tem = myhash.at(part);
                auto key1 = make_pair(tem.first, tem.first + nxt - i - 1);
                if (size > min && min != -1) {
                    continue;
                } else if (size == min) {
                    auto key2 = make_pair(trace.second.first, trace.second.first + nxt - i - 1);
                    if (cnt[key1] > cnt[key2]) trace = {nxt, {tem.first, tem.second}};
                } else {
                    min = size;
                    trace = {nxt, {tem.first, tem.second}};
                }
            }
        }
        if (min == -1) {
            continue;
        } else {
            dp[i] = min;
            record[i] = trace;
            auto key = make_pair(trace.second.first, trace.second.first + (trace.first - i) - 1);
            cnt[key]++;
        }
    }

    for (int i = 0; i < len; i = record[i].first)
        segments.emplace_back(record[i].second.first + 1, record[i].second.first + record[i].first - i, record[i].second.second);
    return segments;
}

void print(vector<tiiib> results) {
    for (const auto& ans : results) cout << "Pos: " << g0(ans) << ", Len: " << g1(ans) << ", Cnt: " << g2(ans)  << ", Isreverse: " << (g3(ans) ? "True" : "False") << endl;
}