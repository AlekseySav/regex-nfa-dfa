#include "automata.h"

static std::vector<std::unordered_map<int, std::string>> edges;
static std::vector<std::unordered_map<int, std::string>> inv_edges;
static std::vector<bool> deleted_nodes;

static Automata input;

bool has_operator(const std::string& s, char op) {
    int depht = 0;
    for (char c : s) {
        if (c == '(') depht++;
        else if (c == ')') depht--;
        else if (c == op && !depht) return true;
    }
    return false;
}

std::string star(std::string input) {
    if (has_operator(input, '.') || has_operator(input, '+'))
        input = '(' + input + ')'; 
    return input.size() ? input + '*' : input;
}

std::string cat(std::string input, std::string b) {
    if (has_operator(input, '+') && b.size()) input = '(' + input + ')';
    if (has_operator(b, '+') && input.size()) b = '(' + b + ')';
    if (input.size() && b.size()) return input + '.' + b;
    return input + b;
}

void add(std::string& input, const std::string& b) {
    if (input.size() && b.size()) input += "+";
    input += b;
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
    input.deserialize();

    int t = input.node();
    input.edge(t, input.q0);
    input.q0 = t;
    t = input.node();
    for (int i : input.qfinal)
        input.edge(i, t);
    input.qfinal = {t};

    edges.resize(input.size());
    inv_edges.resize(input.size());
    deleted_nodes.resize(input.size());
    
    for (int n = 0; n < input.size(); n++) {
        for (auto&[c, set] : input.nodes[n]) {
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

    for (int n = 0; n < input.size(); n++)
        if (n != input.q0 && !input.qfinal.contains(n))
            delete_node(n);

    std::cout << edges[input.q0][*input.qfinal.begin()] << '\n';
}
