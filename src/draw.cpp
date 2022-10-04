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
        nodes[i] = reg.RegisterNode(std::to_string(i));
    nodes[a.size()] = reg.RegisterNode("");
    reg.RegisterNodeAttr(nodes[a.size()], Attrs_shape, "point");
    edges.push_back(reg.RegisterEdge(nodes[a.size()], nodes[a.q0]));

    for (int i : a.qfinal)
        reg.RegisterNodeAttr(nodes[i], Attrs_shape, "doublecircle");

    for (int from = 0; from < a.size(); from++) {
        for (auto&[c, set] : a.nodes[from]) {
            for (int to : set) {
                edges.push_back(reg.RegisterEdge(nodes[from], nodes[to]));
                reg.RegisterEdgeAttr(edges.back(), Attrs_label, std::string(1, c ? alpha(c) : 'e'));
                if (!c) reg.RegisterEdgeAttr(edges.back(), Attrs_fontcolor, "blue");
            }
        }
    }
    for (size_t i : nodes)
        graph.AddNode(i);
    for (size_t i : edges)
        graph.AddEdge(i);
    std::cout << graph.Dump() << '\n';
}
