#include "automata.h"

static std::vector<std::unordered_map<int, std::string>> edges;
static std::vector<std::unordered_map<int, std::string>> inverted_edges;
static std::vector<bool> deleted_nodes;

static Automata input;

bool has_operator(const std::string& s, char op) {
    int depth = 0;
    for (char symbol : s) {
        if (symbol == '(') {
            depth++;
        }
        else if (symbol == ')') {
            depth--;
        }
        else if (symbol == op && !depth) {
            return true;
        }
    }
    return false;
}

std::string star(std::string input) {
    if (has_operator(input, '.') || has_operator(input, '+')) {
        input = '(' + input + ')'; 
    }
    return input.size() ? input + '*' : input;
}

std::string cat(std::string input, std::string append) {
    if (has_operator(input, '+') && append.size()) {
        input = '(' + input + ')';
    }
    if (has_operator(append, '+') && input.size()) {
        append = '(' + append + ')';
    }
    if (input.size() && append.size()) {
        return input + '.' + append;
    }
    return input + append;
}

void add(std::string& input, const std::string& append) {
    if (input.size() && append.size()) {
        input += "+";
    }
    input += append;
}

void delete_node(int node) {
    for (auto[prev, in_re] : inverted_edges[node]) {
        if (prev == node || deleted_nodes[prev]) {
            continue;
        }
        for (auto[next, out_re] : edges[node]) {
            if (next == node || deleted_nodes[next]) {
                continue;
            } 
            auto re = cat(cat(in_re, star(edges[node][node])), out_re);
            add(edges[prev][next], re);
            add(inverted_edges[next][prev], re);
        }
    }
    deleted_nodes[node] = true;
}

void add_temp_nodes() {
    int temp_node = input.node();
    input.edge(temp_node, input.entry_state);
    input.entry_state = temp_node;
    temp_node = input.node();
    for (int final : input.final_states) {
        input.edge(final, temp_node);
    }
    input.final_states = {temp_node};
}

int main() {
    input.deserialize();
    add_temp_nodes();

    edges.resize(input.size());
    inverted_edges.resize(input.size());
    deleted_nodes.resize(input.size());
    
    for (int node = 0; node < input.size(); node++) {
        for (auto&[symbol, set] : input.nodes[node]) {
            for (int to : set) {
                add(edges[node][to], symbol ? std::string(1, alpha(symbol)) : "0");
                add(inverted_edges[to][node], symbol ? std::string(1, alpha(symbol)) : "0");
            }
        }
        if (!edges[node].contains(node)) {
            inverted_edges[node].emplace(node, "");
            edges[node].emplace(node, "");
        }
    }

    for (int node = 0; node < input.size(); node++) {
        if (node != input.entry_state && !input.final_states.contains(node)) {
            delete_node(node);
        }
    }

    std::cout << edges[input.entry_state][*input.final_states.begin()] << '\n';
}
