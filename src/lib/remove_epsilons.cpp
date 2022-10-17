#include "../automata.h"

class EpsilonVisitor {
public:
    EpsilonVisitor(Automata& nfa, std::vector<bool>& used) : nfa(nfa), used(used) {
        for (int node = 0; node < nfa.size(); node++) {
            chain_dfs(node, node);
            used.assign(nfa.size(), 0);
        }
    }
private:
    void chain_dfs(int p, int node) {
        used[node] = true;
        for (int i : nfa.nodes[node][EPSILON]) {
            if (!used[i]) {
                chain_dfs(p, i);
            }
            if (nfa.final_states.contains(i)) {
                nfa.final_states.emplace(node);
            }
        }
        for (auto&[symbol, to] : nfa.nodes[node]) {
            if (symbol != EPSILON) {
                for (int i : to) {
                    nfa.edge(p, i, symbol);
                }
            }
        }
    }

    Automata& nfa;
    std::vector<bool>& used;
};

void remove_epsilons(Automata& nfa) {
    static std::vector<bool> used(nfa.size());
    EpsilonVisitor(nfa, used);
    for (int node = 0; node < nfa.size(); node++) {
        nfa.nodes[node][EPSILON].clear();
    }
    used = nfa.get_reachable_nodes();
    for (int node = nfa.size() - 1; node >= 0; node--) {
        if (!used[node]) {
            nfa.remove_node(node);
        }
    }
}
