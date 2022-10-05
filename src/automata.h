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

#include "../alphabet.h"

struct Automata {
public:
    using edges_t = std::vector<std::unordered_map<int, std::unordered_set<int>>>;

    void serialize();
    void deserialize(std::istream& is = std::cin);

    /* copy last node to n node */
    void remove_node(int n) {
        int last = size() - 1;
        std::swap(nodes[last], nodes[n]);
        for (int i = 0; i < size(); i++) {
            for (auto&[c, to] : nodes[i]) {
                if (to.find(last) != to.end()) {
                    to.erase(last);
                    to.emplace(n);
                }
            }
        }
        nodes.pop_back();
        if (q0 == last) q0 = n;
        if (qfinal.contains(n)) qfinal.erase(n);
        if (qfinal.contains(last)) qfinal.erase(last), qfinal.emplace(n);
    }

    void edge(int from, int to, int c) { nodes[from][c].emplace(to); }
    void edge(int from, int to) { edge(from, to, chrid('\e')); }


    int node() {
        int n = nodes.size();
        nodes.emplace_back();
        return n;
    }

    int node(int n, int c) {
        int x = node();
        edge(n, x, c);
        return x;
    }

    int node(int n) { return node(n, chrid('\e')); }
    int size() const { return nodes.size(); }
public:
    edges_t nodes;
    std::unordered_set<int> qfinal;
    int q0;
};

void Automata::serialize() {
    std::cout << nodes.size() << '\n' << q0 << '\n' << qfinal.size() << '\n';
    for (auto i : qfinal)
        std::cout << i << ' ';
    std::cout << '\n';
    for (int i = 0; i < nodes.size(); i++)
        for (auto&[c, to] : nodes[i])
            for (int j : to)
                std::cout << i << ' ' << j << ' ' << c << '\n';
    std::cout << "0 0 0\n";
}

void Automata::deserialize(std::istream& is) {
    int n_nodes, n_finals, i, j, c;
    is >> n_nodes >> q0 >> n_finals;
    nodes.resize(n_nodes);
    while (n_finals--) {
        is >> i;
        qfinal.emplace(i);
    }
    for (;;) {
        is >> i >> j >> c;
        if (!i && !j && !c) break;
        if (c == -1) {
            while ((c = is.get()) != '\'');
            c = chrid(is.get());
        }
        nodes[i][c].emplace(j);
    }
}
