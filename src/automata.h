#pragma once

/*
 * Automata is mutual structure for all utilities
 * escape '\e' character used as epsilon (because they are simmilar ahaha) 
 */

#include <cstddef>
#include <vector>
#include <set>
#include <array>
#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "../alphabet.h"

template<int W>
struct Automata_w {
public:
    void serialize();
    void deserialize();

    void rm_node(int n) {
        int x = size() - 1;
        std::swap(nodes[x], nodes[n]);
        for (int i = 0; i < size(); i++) {
            for (int c = 0; c < W; c++) {
                auto& v = nodes[i][c];
                if (v.find(x) != v.end()) v.erase(x), v.emplace(n);
            }
        }
        nodes.pop_back();
        if (q0 == x) q0 = n;
        if (F.contains(n)) F.erase(n);
        if (F.contains(x)) F.erase(x), F.emplace(n);
    }

    constexpr void edge(int from, int to, int c) { nodes[from][c].emplace(to); }
    constexpr void edge(int from, int to) { edge(from, to, chrid('\e')); }


    constexpr int node() {
        int n = nodes.size();
        nodes.emplace_back();
        return n;
    }

    constexpr int node(int n, int c) {
        int x = node();
        edge(n, x, c);
        return x;
    }

    constexpr int node(int n) { return node(n, chrid('\e')); }

    constexpr int size() const { return nodes.size(); }
public:
    std::vector<std::array<std::unordered_set<int>, W>> nodes;
    std::unordered_set<int> F;
    int q0;
};

using Automata = Automata_w<W>;

template<int W>
void Automata_w<W>::serialize() {
    std::cout << nodes.size() << '\n' << q0 << '\n' << F.size() << '\n';
    for (auto i : F)
        std::cout << i << ' ';
    std::cout << '\n';
    for (int i = 0; i < nodes.size(); i++)
        for (int c = 0; c < nodes[i].size(); c++)
            for (int j : nodes[i][c])
                std::cout << i << ' ' << j << ' ' << c << '\n';
    std::cout << "0 0 0\n";
}

template<int W>
void Automata_w<W>::deserialize() {
    int n_nodes, n_finals, i, j, c;
    std::cin >> n_nodes >> q0 >> n_finals;
    nodes.resize(n_nodes);
    while (n_finals--) {
        std::cin >> i;
        F.emplace(i);
    }
    for (;;) {
        std::cin >> i >> j >> c;
        if (!i && !j && !c) break;
        nodes[i][c].emplace(j);
    }
}
