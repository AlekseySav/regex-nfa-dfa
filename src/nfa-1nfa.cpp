#include "automata.h"

static Automata input;
static std::vector<bool> used;

void chain_dfs(int p, int n) {
    used[n] = true;
    for (int i : input.nodes[n][chrid('\e')]) {
        if (!used[i]) chain_dfs(p, i);
        if (input.qfinal.contains(i)) input.qfinal.emplace(n);
    }
    for (auto&[c, to] : input.nodes[n])
        if (c != chrid('\e'))
            for (int i : to)
                input.edge(p, i, c);
}

int main(int argc, char** argv) {
    input.deserialize();

    used.resize(input.size());
    for (int i = 0; i < input.size(); i++) {
        chain_dfs(i, i);
        used.assign(input.size(), 0);
    }

    for (int i = 0; i < input.size(); i++)
        input.nodes[i][chrid('\e')].clear();

    used = input.get_reachable_nodes();
    for (int i = input.size() - 1; i >= 0; i--)
        if (!used[i])
            input.remove_node(i);

    input.serialize();
}
