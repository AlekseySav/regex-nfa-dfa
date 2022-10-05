#include "automata.h"

static std::vector<std::unordered_map<int, std::string>> edges;
static std::vector<std::unordered_map<int, std::string>> inv_edges;
static std::vector<bool> deleted_nodes;

static Automata a;

bool has_operator(const std::string& s, char op) {
    int depht = 0;
    for (char c : s) {
        if (c == '(') depht++;
        else if (c == ')') depht--;
        else if (c == op && !depht) return true;
    }
    return false;
}

std::string star(std::string a) {
    if (has_operator(a, '.') || has_operator(a, '+'))
        a = '(' + a + ')'; 
    return a.size() ? a + '*' : a;
}

std::string cat(std::string a, std::string b) {
    if (has_operator(a, '+') && b.size()) a = '(' + a + ')';
    if (has_operator(b, '+') && a.size()) b = '(' + b + ')';
    if (a.size() && b.size()) return a + '.' + b;
    return a + b;
}

void add(std::string& a, const std::string& b) {
    if (a.size() && b.size()) a += "+";
    a += b;
}

void delete_node(int n) {
    for (auto[prev, in_re] : inv_edges[n]) {
        if (prev == n || deleted_nodes[prev]) continue;
        for (auto[next, out_re] : edges[n]) {
            if (next == n || deleted_nodes[next]) continue; 
            auto re = cat(cat(in_re, star(edges[n][n])), out_re);
            add(edges[prev][next], re);
            add(inv_edges[next][prev], re);
        }
    }
    deleted_nodes[n] = true;
}

int main() {
    a.deserialize();

    int t = a.node();
    a.edge(t, a.q0);
    a.q0 = t;
    t = a.node();
    for (int i : a.qfinal)
        a.edge(i, t);
    a.qfinal = {t};

    edges.resize(a.size());
    inv_edges.resize(a.size());
    deleted_nodes.resize(a.size());
    
    for (int n = 0; n < a.size(); n++) {
        for (auto&[c, set] : a.nodes[n]) {
            for (int v : set) {
                add(edges[n][v], c ? std::string(1, alpha(c)) : "0");
                add(inv_edges[v][n], c ? std::string(1, alpha(c)) : "0");
            }
        }
        if (!edges[n].contains(n)) {
            inv_edges[n].emplace(n, "");
            edges[n].emplace(n, "");
        }
    }

    for (int n = 0; n < a.size(); n++)
        if (n != a.q0 && !a.qfinal.contains(n))
            delete_node(n);

    std::cout << edges[a.q0][*a.qfinal.begin()] << '\n';
}
