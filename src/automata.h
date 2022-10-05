#pragma once

/*
 * Automata is mutual structure for all utilities
 * escape '\e' character used as epsilon (because they are simmilar ahaha)
 */

#include <cassert>
#include <cstddef>
#include <vector>
#include <set>
#include <array>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <functional>

#include "../alphabet.h"

class Automata {
public:
    using edges_t = std::vector<std::unordered_map<int, std::unordered_set<int>>>;

    void serialize();
    void deserialize(std::istream& is = std::cin);

    /* move last node to n node */
    void remove_node(int node) {
        int last = size() - 1;
        std::swap(nodes[last], nodes[node]);
        for (int i = 0; i < size(); i++) {
            for (auto&[symbol, to] : nodes[i]) {
                if (to.find(last) != to.end()) {
                    to.erase(last);
                    to.emplace(node);
                }
            }
        }
        nodes.pop_back();
        if (entry_state == last) entry_state = node;
        if (final_states.contains(node)) final_states.erase(node);
        if (final_states.contains(last)) final_states.erase(last), final_states.emplace(node);
    }

    void edge(int from, int to, int symbol) { nodes[from][symbol].emplace(to); }
    void edge(int from, int to) { edge(from, to, char_id('\e')); }


    int node() {
        int node = nodes.size();
        nodes.emplace_back();
        return node;
    }

    int node(int node_number, int symbol) {
        int new_node = node();
        edge(node_number, new_node, symbol);
        return new_node;
    }

    int node(int node_number) { return node(node_number, char_id('\e')); }
    int size() const { return nodes.size(); }

    std::vector<int> get_edges(int from, int to) const {
        std::vector<int> res;
        for (auto&[symbol, outgoing_edges] : nodes[from]) {
            if (std::find(outgoing_edges.begin(), outgoing_edges.end(), to) != outgoing_edges.end()) {
                res.push_back(symbol);
            }
        }
        return res;
    }

    std::vector<bool> get_reachable_nodes() const {
        std::vector<bool> used(size());
        std::function<void(int)> dfs;
        dfs = [&used, &dfs, this](int node_number) {
            if (used[node_number]) return;
            used[node_number] = true;
            for (auto&[symbol, to] : nodes[node_number]) {
                for (int i : to) {
                    dfs(i);
                }
            }
        };
        dfs(entry_state);
        return used;
    }

public:
    edges_t nodes;
    std::unordered_set<int> final_states;
    int entry_state;
    int max_literal;
};

void Automata::serialize() {
    std::cout << nodes.size() << '\n' << entry_state << '\n' << final_states.size() << '\n';
    for (auto i : final_states)
        std::cout << i << ' ';
    std::cout << '\n';
    for (int from = 0; from < nodes.size(); from++) {
        for (auto&[symbol, outgoing_edges] : nodes[from]) {
            for (int to : outgoing_edges) {
                std::cout << from << ' ' << to << ' ' << symbol << '\n';
            }
        }
    }
    std::cout << "0 0 0\n";
}

void Automata::deserialize(std::istream& is) {
    int n_nodes, n_finals, from, to, symbol;
    is >> n_nodes >> entry_state >> n_finals;
    nodes.resize(n_nodes);
    while (n_finals--) {
        is >> to;
        final_states.emplace(to);
    }
    for (;;) {
        is >> from >> to >> symbol;
        if (!from && !to && !symbol) {
            break;
        }
        if (symbol == -1) {
            while (symbol != '\'') {
                symbol = is.get();
            }
            symbol = char_id(is.get());
        }
        max_literal = max_literal > symbol + 1 ? max_literal : symbol + 1;
        nodes[from][symbol].emplace(to);
    }
}
