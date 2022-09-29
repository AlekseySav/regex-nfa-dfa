#include "automata.h"
#include <fstream>

bool run(Automata& a, const std::string& s) {
    int state = a.q0;
    for (char c : s) {
        auto& v = a.nodes[state][chrid(c)];
        if (!v.size()) return false;
        state = *v.begin();
    }
    return a.F.contains(state);
}

int main(int argc, char** argv) {
    Automata a;
    a.deserialize();
    
    std::ifstream ifs(argv[1]);
    std::string line;
    while (std::getline(ifs, line)) {
        if (line[0] == '/') continue;
        if (run(a, line))
            std::cout << line << ';';
    }
    std::cout << '\n';
}
