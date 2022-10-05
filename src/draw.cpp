#include <ugraphviz/ugraphviz.hpp>
#include "automata.h"

#include <iostream>

using namespace Ubpa::UGraphviz;

int main() {
    Automata input;
    input.deserialize();

    Graph graph("1", true);
    std::vector<size_t> nodes;
    std::vector<size_t> edges;

    nodes.resize(input.size() + 1);
    auto& reg = graph.GetRegistry();

    for (int i = 0; i < input.size(); i++)
        nodes[i] = reg.RegisterNode(std::to_string(i + 1));
    nodes[input.size()] = reg.RegisterNode("");
    reg.RegisterNodeAttr(nodes[input.size()], Attrs_shape, "point");
    edges.push_back(reg.RegisterEdge(nodes[input.size()], nodes[input.q0]));

    for (int i : input.qfinal)
        reg.RegisterNodeAttr(nodes[i], Attrs_shape, "doublecircle");

    for (int from = 0; from < input.size(); from++) {
        for (int to = 0; to < input.size(); to++) {
            auto e = input.get_edges(from, to);
            if (!e.size()) continue;
            std::string label;
            for (int c : e)
                label += (label.size() ? "," : "") + (c ? std::string(1, alpha(c)) : "eps");
            edges.push_back(reg.RegisterEdge(nodes[from], nodes[to]));
            reg.RegisterEdgeAttr(edges.back(), Attrs_label, label);
        }
    }
    for (size_t i : nodes)
        graph.AddNode(i);
    for (size_t i : edges)
        graph.AddEdge(i);
    std::cout << graph.Dump() << '\n';
}
