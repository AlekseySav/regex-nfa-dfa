#include "automata.h"

static Automata a;
static std::vector<bool> used;

void chain_dfs(int p, int n) {
    used[n] = true;
    for (int i : a.nodes[n][chrid('\e')]) {
        if (!used[i]) chain_dfs(p, i);
        if (a.F.contains(i)) a.F.emplace(n);
    }
    for (auto&[c, to] : a.nodes[n])
        if (c != chrid('\e'))
            for (int i : to)
                a.edge(p, i, c);
}

void access_dfs(int n) {
    if (used[n]) return;
    used[n] = true;
    for (auto&[c, to] : a.nodes[n])
        for (int i : to)
            access_dfs(i);
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
