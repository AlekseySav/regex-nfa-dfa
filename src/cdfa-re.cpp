#include "automata.h"

static struct ReDFA {
    std::vector<std::unordered_map<int, std::string>> nodes;
} regex;

static Automata a;

static std::vector<bool> used;
static std::vector<bool> garbage;

std::string star(std::string a) {
    if (a.contains('.') || a.contains('+'))
        a = '(' + a + ')'; 
    return a.size() ? a + '*' : a;
}

std::string cat(std::string a, std::string b) {
    if (a.contains('+')) a = '(' + a + ')';
    if (b.contains('+')) b = '(' + b + ')';
    if (a.size() && b.size()) return a + '.' + b;
    return a + b;
}

void add(std::string& a, const std::string& b) {
    if (a.size() && b.size()) a += "+";
    a += b;
}

void garbage_dfs(int n) {
    int res = 0;
    used[n] = true;
    for (auto&[v, re] : regex.nodes[n]) {
        if (v == n) continue;
        if (!used[v]) garbage_dfs(v);
        if (!garbage[v]) res++;
    }
    if (!res && !a.F.contains(n))
        garbage[n] = true;
}


void re_dfs(int n, int prev = -1) {
    used[n] = true;
    if (garbage[n]) return;

    auto copy = regex.nodes[n];
    for (auto&[to, re] : copy)
        if (!used[to])
            re_dfs(to, n);
    for (auto&[to, re] : regex.nodes[n]) {
        if (to == n || prev < 0 || garbage[to]) continue;
        add(regex.nodes[prev][to], cat(cat(regex.nodes[prev][n], star(regex.nodes[n][n])), re));
        regex.nodes[prev].erase(n);
    }
}

int main() {
    a.deserialize();
    regex.nodes.resize(a.size());
    for (int n = 0; n < a.size(); n++) {
        auto& dest = regex.nodes[n];
        for (auto&[c, to] : a.nodes[n]) {
            assert(to.size() == 1);
            int v = *to.begin();
            add(dest[v], std::string(1, alpha(c)));
        }
        if (!dest.contains(n)) dest.emplace(n, "");
    }
    used.resize(a.size());
    garbage.resize(a.size());
    garbage_dfs(a.q0);
    used.assign(a.size(), false);
    re_dfs(a.q0);
    std::string result;
    for (auto& v_id : a.F) {
        const std::string& re_loop = regex.nodes[v_id][v_id];
        const std::string& re = v_id == a.q0 ? "" : regex.nodes[a.q0][v_id];
        add(result, re + star(re_loop));
        if (!re_loop.size() && v_id == a.q0)
            add(result, "0");
    }
    std::cout << result << '\n';
}
