#include "automata.h"
#include <queue>
#include <unordered_set>
#include <map>

static Automata input, output;

using state = std::unordered_set<int>;
static std::vector<state> o_list;

int node(const state& origin_nodes) {
    int node = output.node();
    o_list.push_back(origin_nodes);
    for (int origin_node : origin_nodes) {
        if (input.final_states.contains(origin_node)) {
            output.final_states.emplace(node);
        }
    }
    return node;
}

void tompson_bfs() {
    std::queue<int> node_queue; /* index within o_list */
    output.entry_state = node({input.entry_state});
    node_queue.push(output.entry_state);

    while (node_queue.size()) {
        int id_from = node_queue.front();
        node_queue.pop();
        for (int symbol = 0; symbol < input.max_literal; symbol++) {
            const state& from_set = o_list[id_from];
            state to;
            for (int from : from_set) {
                to.merge(state(input.nodes[from][symbol]));
            }
            if (!to.size()) continue;

            int id_to = std::find(o_list.begin(), o_list.end(), to) - o_list.begin();
            if (id_to == o_list.size()) {
                id_to = node(to);
                node_queue.push(id_to);
            }
            output.edge(id_from, id_to, symbol);
        }
    }
}

int main() {
    input.deserialize();
    tompson_bfs();
    output.serialize();
}
