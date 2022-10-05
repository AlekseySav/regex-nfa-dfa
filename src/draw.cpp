#include <ugraphviz/ugraphviz.hpp>
#include "automata.h"

#include <iostream>

using namespace Ubpa::UGraphviz;

int main() {
    Automata a;
    a.deserialize();

    Graph graph("1", true);
    std::vector<size_t> nodes;
    std::vector<size_t> edges;

    nodes.resize(a.size() + 1);
    auto& reg = graph.GetRegistry();

    for (int i = 0; i < a.size(); i++)
        nodes[i] = reg.RegisterNode(std::to_string(i + 1));
    nodes[a.size()] = reg.RegisterNode("");
    reg.RegisterNodeAttr(nodes[a.size()], Attrs_shape, "point");
    edges.push_back(reg.RegisterEdge(nodes[a.size()], nodes[a.q0]));

    for (int i : a.qfinal)
        reg.RegisterNodeAttr(nodes[i], Attrs_shape, "doublecircle");

    for (int from = 0; from < a.size(); from++) {
        for (int to = 0; to < a.size(); to++) {
            auto e = a.get_edges(from, to);
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
