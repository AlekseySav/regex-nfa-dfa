#include "automata.h"
#include <queue>

static Automata input, output;
static std::vector<bool> used;

Automata::edges_t invert_edges(const Automata& a) {
    Automata::edges_t nodes(a.size());
    for (int from = 0; from < a.size(); from++)
        for (auto&[c, set] : a.nodes[from])
            for (int to : set)
                nodes[to][c].emplace(from);
    return nodes;
}

std::vector<std::vector<bool>> build_table() {
    std::queue<std::pair<int, int>> queue;
    std::vector<std::vector<bool>> marked(input.size(), std::vector<bool>(input.size()));
    auto inverted = invert_edges(input);
    for (int i : input.qfinal) {
        for (int j = 0; j < input.size(); j++) {
            if (!input.qfinal.contains(j) && !marked[i][j]) {
                queue.push({i, j});
                marked[i][j] = marked[j][i] = true;
            }
        }
    }
    while (!queue.empty()) {
        auto[u, v] = queue.front();
        queue.pop();
        for (int c = 0; c < W; c++) {
            for (int r : inverted[u][c]) {
                for (int s : inverted[v][c]) {
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

void access_dfs(int n) {
    if (used[n]) return;
    used[n] = true;
    for (auto&[c, to] : input.nodes[n])
        for (int i : to)
            access_dfs(i);
}

int main() {
    input.deserialize();
    auto marked = build_table();
    std::vector<int> component(input.size(), -1);
    for (int i = 0; i < input.size(); i++)
        if (!marked[input.size() - 1][i])
            component[i] = 0;

    used.resize(input.size());
    access_dfs(input.q0);

    int n_components = 0;
    for (int i = 0; i < input.size(); i++) {
        if (!used[i]) continue;
        if (component[i] != -1) continue;
        component[i] = n_components++;
        for (int j = 0; j < input.size(); j++)
            if (!marked[i][j])
                component[j] = component[i];
    }

    output.q0 = component[input.q0];
    for (int i : input.qfinal)
        output.qfinal.emplace(component[i]);
    output.nodes.resize(n_components);

    for (int from = 0; from < input.size(); from++)
        for (auto& [c, set] : input.nodes[from])
            for (int to : set)
                output.nodes[component[from]][c].emplace(component[to]);

    output.serialize();
}
