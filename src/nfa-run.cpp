#include "automata.h"
#include <fstream>
#include <sstream>

bool run(Automata& a, int state, std::string_view s) {
    for (auto it = s.begin(); it != s.end(); ++it) {
        char c = *it;
        auto& v = a.nodes[state][chrid(c)];
        if (!v.size()) return false;
        for (int to : v)
            if (run(a, to, it + 1))
                return true;
        return false;
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
            if (run(a, a.q0, line))
                std::cout << line << ';';
        }
    }
    else {
        std::stringstream ss(argv[1]);
        std::string line;
        while (std::getline(ss, line)) {
            if (run(a, a.q0, line))
                std::cout << line << ';';
        }
    }
    std::cout << '\n';
}
