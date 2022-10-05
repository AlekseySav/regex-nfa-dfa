#include "automata.h"

static Automata input;
static std::vector<bool> used;

void chain_dfs(int p, int node) {
    used[node] = true;
    for (int i : input.nodes[node][char_id('\e')]) {
        if (!used[i]) {
            chain_dfs(p, i);
        }
        if (input.final_states.contains(i)) {
            input.final_states.emplace(node);
        }
    }
    for (auto&[symbol, to] : input.nodes[node]) {
        if (symbol != char_id('\e')) {
            for (int i : to) {
                input.edge(p, i, symbol);
            }
        }
    }
}

int main(int argc, char** argv) {
    input.deserialize();

    used.resize(input.size());
    for (int node = 0; node < input.size(); node++) {
        chain_dfs(node, node);
        used.assign(input.size(), 0);
    }

    for (int node = 0; node < input.size(); node++) {
        input.nodes[node][char_id('\e')].clear();
    }

    used = input.get_reachable_nodes();
    for (int node = input.size() - 1; node >= 0; node--) {
        if (!used[node]) {
            input.remove_node(node);
        }
    }

    input.serialize();
}
