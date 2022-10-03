#include "automata.h"
#include <queue>
#include <unordered_set>
#include <map>

static Automata input, output;

using state = std::unordered_set<int>;
static std::vector<state> o_list;

int node(const state& q) {
    int n = output.node();
    o_list.push_back(q);
    for (int i : q)
        if (input.qfinal.contains(i))
            output.qfinal.emplace(n);
    return n;
}

void tompson_bfs() {
    std::queue<int> queue; /* index within o_list */
    output.q0 = node({input.q0});
    queue.push(output.q0);

    while (queue.size()) {
        int id_from = queue.front();
        queue.pop();
        for (int c = 0; c < W; c++) {
            const state& from = o_list[id_from];
            state to;
            for (int i : from) to.merge(state(input.nodes[i][c]));
            if (!to.size()) continue;

            int id_to = std::find(o_list.begin(), o_list.end(), to) - o_list.begin();
            if (id_to == o_list.size()) id_to = node(to), queue.push(id_to);
            output.edge(id_from, id_to, c);
        }
    }
}

int main() {
    input.deserialize();
    tompson_bfs();
    output.serialize();
}
