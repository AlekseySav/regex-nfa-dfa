#include "automata.h"
#include <queue>
#include <unordered_set>
#include <map>

static Automata input, output;

using qtype = std::unordered_set<int>;
static std::vector<qtype> o_list;

int node(const qtype& q) {
    int n = output.node();
    o_list.push_back(q);
    for (int i : q)
        if (input.F.contains(i))
            output.F.emplace(n);
    return n;
}

void tompson_bfs() {
    std::queue<int> q; /* index within o_list */
    output.q0 = node({input.q0});
    q.push(output.q0);

    while (q.size()) {
        int id_from = q.front(); q.pop();
        for (int c = 0; c < W; c++) {
            const qtype& from = o_list[id_from];
            qtype to;
            for (int i : from) to.merge(qtype(input.nodes[i][c]));
            if (!to.size()) continue;

            int id_to = std::find(o_list.begin(), o_list.end(), to) - o_list.begin();
            if (id_to == o_list.size()) id_to = node(to), q.push(id_to);
            output.edge(id_from, id_to, c);
        }
    }
}

int main() {
    input.deserialize();
    tompson_bfs();
    output.serialize();
}
