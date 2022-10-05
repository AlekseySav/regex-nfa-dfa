#include <ugraphviz/ugraphviz.hpp>
#include "automata.h"

#include <iostream>

using namespace Ubpa::UGraphviz;

std::vector<size_t> nodes;
std::vector<size_t> edges;

void register_edges(const Automata& input, Registry& reg) {
    for (int from = 0; from < input.size(); from++) {
        for (int to = 0; to < input.size(); to++) {
            auto edges = input.get_edges(from, to);
            if (!edges.size()) {
                continue;
            }
            std::string label;
            for (int symbol : edges) {
                label += (label.size() ? "," : "") + (symbol ? std::string(1, alpha(symbol)) : "eps");
            }
            edges.push_back(reg.RegisterEdge(nodes[from], nodes[to]));
            reg.RegisterEdgeAttr(edges.back(), Attrs_label, label);
        }
    }
}

int main() {
    Automata input;
    input.deserialize();

    Graph graph("1", true);

    nodes.resize(input.size() + 1);
    auto& reg = graph.GetRegistry();

    for (int node = 0; node < input.size(); node++) {
        nodes[node] = reg.RegisterNode(std::to_string(node + 1));
    }
    nodes[input.size()] = reg.RegisterNode("");
    reg.RegisterNodeAttr(nodes[input.size()], Attrs_shape, "point");
    edges.push_back(reg.RegisterEdge(nodes[input.size()], nodes[input.entry_state]));

    for (int final : input.final_states) {
        reg.RegisterNodeAttr(nodes[final], Attrs_shape, "doublecircle");
    }

    register_edges(input, reg);

    for (size_t node : nodes) {
        graph.AddNode(node);
    }
    for (size_t edge : edges) {
        graph.AddEdge(edge);
    }
    std::cout << graph.Dump() << '\n';
}
