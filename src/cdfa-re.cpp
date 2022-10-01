#include "automata.h"

struct edge {
    int from, to;
    std::string re;
    bool deleted;
};

static std::vector<edge> edges;

static struct ReDFA {
    std::vector<std::unordered_map<int, std::string>> nodes;
} regex;

static Automata a;

static std::vector<bool> g_used;
static std::vector<std::unordered_map<int, bool>> r_used;
static std::vector<bool> garbage;

std::string star(std::string a) {
    if (a.contains('.') || a.contains('+'))
        a = '(' + a + ')'; 
    return a.size() ? a + '*' : a;
}

std::string cat(std::string a, std::string b) {
    if (a.contains('+') && b.size()) a = '(' + a + ')';
    if (b.contains('+') && a.size()) b = '(' + b + ')';
    if (a.size() && b.size()) return a + '.' + b;
    return a + b;
}

void add(std::string& a, const std::string& b) {
    if (a.size() && b.size()) a += "+";
    a += b;
}

void garbage_dfs(int n) {
    int res = 0;
    g_used[n] = true;
    for (auto&[v, re] : regex.nodes[n]) {
        if (v == n) continue;
        if (!g_used[v]) garbage_dfs(v);
        if (!garbage[v]) res++;
    }
    if (!res && n != a.size())
        garbage[n] = true;
}

/* aaaaa need not dfs but pass through all edges */
/*void re_dfs(int n, int prev = -1) {
    if (prev >= 0) r_used[prev][n] = true;
    if (garbage[n]) return;

    std::unordered_map<int, std::string> copy = regex.nodes[n];
    for (auto&[to, re] : copy)
        if (!r_used[n][to])
            re_dfs(to, n);
    for (auto&[to, re] : regex.nodes[n]) {
        if (to == n || prev < 0 || garbage[to]) continue;
        add(regex.nodes[prev][to], cat(cat(regex.nodes[prev][n], star(regex.nodes[n][n])), re));
        if (!a.F.contains(n)) regex.nodes[prev].erase(n);
    }
}*/

void build_regex() {
    for (int from = 0; from < regex.nodes.size(); from++)
        for (auto&[to, re] : regex.nodes[from])
            edges.emplace_back(from, to, re, false);
    for (int i = 0; i < edges.size(); i++) { /* can't use iterators */
        auto&[from, mid, re1, del] = edges[i];
        if (garbage[mid] || garbage[from] || from == mid) continue;
        for (auto&[to, re2] : regex.nodes[mid]) {
            if (garbage[to] || to == mid) continue;
            add(regex.nodes[from][to], cat(cat(re1, star(regex.nodes[mid][mid])), re2));
            edges.emplace_back(from, to, regex.nodes[from][to], false);
            del = true;
        }
    }
    /*std::cout << edges.size() << '\n';*/
}

int main() {
    a.deserialize();
    regex.nodes.resize(a.size() + 1);
    for (int n = 0; n < a.size(); n++) {
        auto& dest = regex.nodes[n];
        for (auto&[c, to] : a.nodes[n]) {
            assert(to.size() == 1);
            int v = *to.begin();
            add(dest[v], std::string(1, alpha(c)));
        }
        if (!dest.contains(n)) dest.emplace(n, "");
    }
    for (int i : a.F)
        regex.nodes[i][a.size()] = "";

    g_used.resize(a.size());
    r_used.resize(a.size());
    garbage.resize(a.size());
    garbage_dfs(a.q0);
    build_regex();
    std::string result = regex.nodes[a.q0][a.size()];
    std::cout << result << '\n';
}
