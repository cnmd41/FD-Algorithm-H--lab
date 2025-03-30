#include "utils.h"

using namespace std;

// 补充碱基互补
char complement(char base) {
    return base == 'A' ? 'T' : base == 'T' ? 'A' : base == 'G' ? 'C' : 'G';
}

// 反向互补
string reverse_complement(const string& seq) {
    string rev_comp;
    for (int i = seq.size() - 1; i >= 0; --i) rev_comp += complement(seq[i]);
    return rev_comp;
}

// 构建哈希表
unordered_map<string, pib> build_hash(const string& seq) {
    unordered_map<string, pib> hash_table;
    int len = seq.size();
    
    // 正向处理
    for (int i = 0; i < len; ++i) {
        string part;
        for (int j = i; j < len; ++j) {
            part += seq[j];
            hash_table[part] = {i, false};
        }
    }
    
    // 反向互补处理
    string rev_comp = reverse_complement(seq);
    for (int i = 0; i < len; ++i) {
        string part;
        for (int j = i; j < len; ++j) {
            part += rev_comp[j];
            if (!hash_table.count(part)) {
                hash_table[part] = {len - 1 - j, true};
            }
        }
    }
    
    return hash_table;
}

// 比较两个元组
bool compare_tuples(const tiiib& a, const tiiib& b) {
    return get<0>(a) != get<0>(b) ? get<0>(a) < get<0>(b) : get<1>(a) != get<1>(b) ? get<1>(a) < get<1>(b) : get<3>(a) < get<3>(b);
}

// 归并排序
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
        if (compare_tuples(left_arr[i], right_arr[j])) {
            arr[k] = left_arr[i];
            i++;
        } else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}

// 查找最优区段
vector<tiib> find_segments(const string& query, const unordered_map<string, pib>& hash_table) {
    int len = query.size();
    vector<int> dp(len + 1, len + 1);
    dp[len] = 0;
    vector<pair<int, pib>> record(len + 1, {-1, {-1, false}});

    unordered_map<string, int> segment_count;

    for (int i = len - 1; i >= 0; --i) {
        string part;
        int min_segments = -1;
        pair<int, pib> best_trace = {-1, {-1, false}}; 
        for (int j = i; j < len; ++j) {
            part += query[j];
            if (hash_table.find(part) != hash_table.end()) {
                int next_pos = j + 1;
                int segments = 1 + dp[next_pos];
                pib val = hash_table.at(part);
                int start = val.first;
                bool rev = val.second;
                int sub_len = next_pos - i;
                string key = to_string(start) + "_" + to_string(start + sub_len - 1);

                if (segments < min_segments || min_segments == -1) {
                    min_segments = segments;
                    best_trace = {next_pos, {start, rev}};
                } else if (segments == min_segments) {
                    string best_key = to_string(best_trace.second.first) + "_" + to_string(best_trace.second.first + (next_pos - i) - 1);
                    if (segment_count[key] > segment_count[best_key]) best_trace = {next_pos, {start, rev}};
                }
            }
        }
        if (min_segments != -1) {
            dp[i] = min_segments;
            record[i] = best_trace;
            string key = to_string(best_trace.second.first) + "_" + to_string(best_trace.second.first + (best_trace.first - i) - 1);
            segment_count[key]++;
        }
    }

    vector<tiib> segments;
    int pos = 0;
    while (pos < len) {
        auto next_pos = record[pos].first;
        auto info = record[pos].second;
        int start = info.first;
        bool rev = info.second;
        int sub_len = next_pos - pos;
        segments.emplace_back(start + 1, start + sub_len, rev);
        pos = next_pos;
    }
    return segments;
}


void print(const vector<tiiib>& results) {
    for (const auto& ans : results) {
        cout << "Position: " << get<0>(ans)
             << ", Length: " << get<1>(ans)
             << ", Count: " << get<2>(ans)
             << ", Reverse: " << (get<3>(ans) ? "True" : "False") << endl;
    }
}