#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>

using namespace std;
typedef unsigned short us;

struct point {
    us q_st, q_ed;
    us r_st, r_ed;
};

struct Node {
    us flag;
    us r_pos, q_pos;
    bool operator==(const Node& other) const {
        return flag == other.flag && r_pos == other.r_pos && q_pos == other.q_pos;
    }
};

namespace std {
template <>
struct hash<Node> {
    size_t operator()(const Node& node) const {
        return (static_cast<size_t>(node.flag) << 32) ^ (node.r_pos << 16) ^ node.q_pos;
    }
};
} 

struct edge {
    us len;
    Node pre_node;
};

char reverse(char c) {
    return c == 'A' ? 'T' : c == 'T' ? 'A' : c == 'C' ? 'G' : 'C';
}

unordered_map<Node, edge, hash<Node>> map;
unordered_set<Node, hash<Node>> visited;
deque<Node> queue;
vector<point> ans;

void update(Node& node, us& len, Node& pre_node) {
    if (map.find(node) == map.end() || map[node].len > len + 1) {
        map[node] = {static_cast<us>(len + 1), pre_node};
        queue.push_back(node);
    }
}

int main() {
    string reference, reverse_reference, query;
    reference += 'M';
    reverse_reference += 'N';
    query += 'M';
    fstream file1("query1.txt");
    fstream file2("reference1.txt");
    char c;
    while (file1.get(c)) if (c != '\n') query += c;
    while (file2.get(c)) {
        if (c != '\n') {
            reference += c;
            reverse_reference += reverse(c);
        }
    }

    us m = reference.size(), n = query.size();

    Node start = {3, 0, 0};
    map[start] = {0, start};
    queue.push_back(start);
    
    while (!queue.empty()) {
        Node cur_node = queue.front();
        Node next_node;
        if (cur_node.q_pos == n - 1) {
            cout << m << "  " << "distance: " << map[cur_node].len << endl;
            break;
        }
        queue.pop_front();
        if (visited.find(cur_node) != visited.end()) continue;
        visited.insert(cur_node);
        us& cur_len = map[cur_node].len;
        if (cur_node.flag == 3) {
            us begin = cur_node.q_pos - 2500 > 0 ? cur_node.q_pos - 2500 : 1;
            us end = cur_node.q_pos + 2500 > m ? m : cur_node.q_pos + 2500;
            for (us i = begin; i <= end; i++) {
                next_node = {1, i, cur_node.q_pos};
                update(next_node, cur_len, cur_node);
                next_node = {2, i, cur_node.q_pos};
                update(next_node, cur_len, cur_node);
            }
            cout << "Reach query:" << cur_node.q_pos << " distance:" << cur_len << endl;
        } else if (cur_node.flag == 1) {
            us i = cur_node.r_pos;
            us j = cur_node.q_pos;
            if (i < m - 1 && j < n - 1) {
                next_node = {1, static_cast<us>(i + 1), static_cast<us>(j + 1)};
                us cost = reference[i + 1] == query[j + 1] ? 0 : 1;
                if (map.find(next_node) == map.end() || map[next_node].len > cur_len + cost) {
                    map[next_node] = {static_cast<us>(cur_len + cost), cur_node};
                    if (cost == 0) queue.push_front(next_node);
                    else           queue.push_back(next_node);
                }
            }
            if (i < m - 1) {
                next_node = {1, static_cast<us>(i + 1), j};
                update(next_node, cur_len, cur_node);
            }
            if (j < n - 1) {
                next_node = {1, i, static_cast<us>(j + 1)};
                update(next_node, cur_len, cur_node);
            }
            next_node = {3, 0, j};
            update(next_node, cur_len, cur_node);
        } else if (cur_node.flag == 2) {
            us& i = cur_node.r_pos, j = cur_node.q_pos;
            if (i >= 1 && j < n - 1) {
                next_node = {2, static_cast<us>(i - 1), static_cast<us>(j + 1)};
                us cost = reverse_reference[i - 1] == query[j + 1] ? 0 : 1;
                if (map.find(next_node) == map.end() || map[next_node].len > cur_len + cost) {
                    map[next_node] = {static_cast<us>(cur_len + cost), cur_node};
                    if (cost == 1) queue.push_back(next_node);
                    else           queue.push_front(next_node);
                }
            }
            if (i >= 1) {
                next_node = {2, static_cast<us>(i - 1), j};
                update(next_node, cur_len, cur_node);
            }
            if (j < n - 1) {
                next_node = {2, i, static_cast<us>(j + 1)};
                update(next_node, cur_len, cur_node);
            }
            next_node = {3, 0, j};
            update(next_node, cur_len, cur_node);
        }
    }

    Node node = queue.front();
    cout << node.q_pos << " " << node.r_pos << endl;
    us seg_end_i = node.r_pos, seg_end_j = node.q_pos;
    while (node.flag != 3 || node.q_pos != 0 || node.r_pos != 0) {
        if (node.flag == 1 && node.flag != map[node].pre_node.flag)
            ans.push_back({node.q_pos, seg_end_j, node.r_pos, seg_end_i});
        if (node.flag == 2 && node.flag != map[node].pre_node.flag)
            ans.push_back({node.q_pos, seg_end_j, static_cast<us>(seg_end_i - 1), static_cast<us>(node.r_pos - 1)});
        if (node.flag == 3 && node.flag != map[node].pre_node.flag) {
            seg_end_i = map[node].pre_node.r_pos;
            seg_end_j = map[node].pre_node.q_pos;
        }
        node = map[node].pre_node;
    }

    for (int i = static_cast<int>(ans.size()) - 1; i >= 0; i--) {
        if (i == 0)
            cout << "(" << ans[i].q_st << "," << ans[i].q_ed << "," << ans[i].r_st << "," << ans[i].r_ed << ")]";
        else if (i == static_cast<int>(ans.size()) - 1)
            cout << "[(" << ans[i].q_st << "," << ans[i].q_ed << "," << ans[i].r_st << "," << ans[i].r_ed << "),";
        else
            cout << "(" << ans[i].q_st << "," << ans[i].q_ed << "," << ans[i].r_st << "," << ans[i].r_ed << "),";
    }

    return 0;
}