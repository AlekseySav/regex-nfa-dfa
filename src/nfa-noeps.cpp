#include "automata.h"

Automata a;
std::vector<bool> used;

void chain_dfs(int p, int n) {
    used[n] = true;
    for (int i : a.nodes[n][chrid('\e')]) {
        if (!used[i]) chain_dfs(p, i);
        if (a.F.contains(i)) a.F.emplace(n);
    }
    for (int c = 0; c < W; c++)
        if (c != chrid('\e'))
            for (int i : a.nodes[n][c])
                a.edge(p, i, c);
}

void access_dfs(int n) {
    if (used[n]) return;
    used[n] = true;
    for (int c = 0; c < W; c++)
        for (int i : a.nodes[n][c])
            access_dfs(i);
}

bool valid(const std::array<std::vector<int>, W>& node) {
    for (int c = 0; c < W; c++)
        if (node[c].size())
            return true;
    return false;
}

int main(int argc, char** argv) {
    a.deserialize();

    used.resize(a.size());
    for (int i = 0; i < a.size(); i++) {
        chain_dfs(i, i);
        used.assign(a.size(), 0);
    }

    for (int i = 0; i < a.size(); i++)
        a.nodes[i][chrid('\e')].clear();

    access_dfs(a.q0);
    for (int i = a.size() - 1; i >= 0; i--)
        if (!used[i])
            a.rm_node(i);

    a.serialize();
}
