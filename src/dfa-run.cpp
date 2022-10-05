#include "automata.h"
#include <fstream>
#include <sstream>

bool run(Automata& a, const std::string& s) {
    int state = a.q0;
    for (char c : s) {
        auto& v = a.nodes[state][chrid(c)];
        if (!v.size()) return false;
        assert(v.size() == 1);
        state = *v.begin();
    }
    return a.qfinal.contains(state);
}

int main(int argc, char** argv) {
    Automata a;
    a.deserialize();
    
    if (argc == 3) {
        std::ifstream ifs(argv[2]);
        std::string line;
        while (std::getline(ifs, line)) {
            if (line[0] == '/') continue;
            if (run(a, line))
                std::cout << line << ';';
        }
    }
    else {
        std::stringstream ss(argv[1]);
        std::string line;
        while (std::getline(ss, line)) {
            if (run(a, line))
                std::cout << line << ';';
        }
    }
    std::cout << '\n';
}
