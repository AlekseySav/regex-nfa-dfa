#include "automata.h"
#include <queue>

static Automata input, output;
static std::vector<bool> used;

Automata::edges_t invert_edges(const Automata& a) {
    Automata::edges_t nodes(a.size());
    for (int from = 0; from < a.size(); from++) {
        for (auto&[symbol, set] : a.nodes[from]) {
            for (int to : set) {
                nodes[to][symbol].emplace(from);
            }
        }
    }
    return nodes;
}

std::vector<std::vector<bool>> build_table() {
    std::queue<std::pair<int, int>> queue;
    std::vector<std::vector<bool>> marked(input.size(), std::vector<bool>(input.size()));
    auto inverted = invert_edges(input);
    for (int from : input.final_states) {
        for (int to = 0; to < input.size(); to++) {
            if (!input.final_states.contains(to) && !marked[from][to]) {
                queue.push({from, to});
                marked[from][to] = marked[to][from] = true;
            }
        }
    }
    while (!queue.empty()) {
        auto[from, to] = queue.front();
        queue.pop();
        for (int symbol = 0; symbol < input.max_literal; symbol++) {
            for (int r : inverted[from][symbol]) {
                for (int s : inverted[to][symbol]) {
                    if (!marked[r][s]) {
                        marked[r][s] = marked[s][r] = true;
                        queue.push({r, s});
                    }
                }
            }
        }
    }
    return marked;
}

int make_components(const std::vector<std::vector<bool>>& marked, std::vector<int>& component) {
    int n_components = 0;
    for (int from = 0; from < input.size(); from++) {
        if (!used[from]) {
            continue;
        }
        if (component[from] != -1) {
            continue;
        }
        component[from] = n_components;
        n_components++;
        for (int to = 0; to < input.size(); to++) {
            if (!marked[from][to]) {
                component[to] = component[from];
            }
        }
    }
    return n_components;
}

int main() {
    input.deserialize();
    auto marked = build_table();
    std::vector<int> component(input.size(), -1);
    used = input.get_reachable_nodes();

    int n_components = make_components(marked, component);

    output.entry_state = component[input.entry_state];
    for (int final_state : input.final_states) {
        output.final_states.emplace(component[final_state]);
    }
    output.nodes.resize(n_components);

    for (int from = 0; from < input.size(); from++) {
        for (auto& [symbol, set] : input.nodes[from]) {
            for (int to : set) {
                output.nodes[component[from]][symbol].emplace(component[to]);
            }
        }
    }

    output.serialize();
}
