#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <unordered_map>
#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

typedef tuple<int, int, int, bool> tiiib;
typedef tuple<int, int, bool> tiib;
typedef pair<int, bool> pib;

// 补充碱基互补函数
char complement(char base);

// 反向互补
string reverse_complement(const string& seq);

// 构建哈希表
unordered_map<string, pib> build_hash(const string& seq);

// 比较两个元组
bool compare_tuples(const tiiib& a, const tiiib& b);

// 归并排序
void merge_sort(vector<tiiib>& arr, int left, int right);
void merge(vector<tiiib>& arr, int left, int mid, int right);

// 查找最优区段
vector<tiib> find_segments(const string& query, const unordered_map<string, pib>& hash_table);

void print(const vector<tiiib>& results);

#endif // UTILS_H