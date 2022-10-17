#include "../automata.h"
#include <queue>
#include <unordered_set>
#include <map>

using state = std::unordered_set<int>;

int node(Automata& input, Automata& output, std::vector<state>& output_vertices, const state& origin_nodes) {
    int node = output.node();
    output_vertices.push_back(origin_nodes);
    for (int origin_node : origin_nodes) {
        if (input.final_states.contains(origin_node)) {
            output.final_states.emplace(node);
        }
    }
    return node;
}

void tompson_bfs(Automata& input, Automata& output, std::vector<state>& output_vertices) {
    std::queue<int> node_queue; /* index within output_vertices */
    output.entry_state = node(input, output, output_vertices, {input.entry_state});
    node_queue.push(output.entry_state);

    while (node_queue.size()) {
        int id_from = node_queue.front();
        node_queue.pop();
        for (int symbol = 0; symbol < input.max_literal; symbol++) {
            const state& from_set = output_vertices[id_from];
            state to;
            for (int from : from_set) {
                to.merge(state(input.nodes[from][symbol]));
            }
            if (!to.size()) continue;

            int id_to = std::find(output_vertices.begin(), output_vertices.end(), to) - output_vertices.begin();
            if (id_to == output_vertices.size()) {
                id_to = node(input, output, output_vertices, to);
                node_queue.push(id_to);
            }
            output.edge(id_from, id_to, symbol);
        }
    }
}

void build_dfa(Automata& nfa) {
    std::vector<state> output_vertices;
    Automata output;
    remove_epsilons(nfa);
    tompson_bfs(nfa, output, output_vertices);
    nfa = output;
}
