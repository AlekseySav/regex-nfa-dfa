#include "automata.h"
#include <fstream>
#include <sstream>

bool read_from_file;
Automata a;

bool run(int state, std::string_view s) {
    for (auto it = s.begin(); it != s.end(); ++it) {
        auto& v = a.nodes[state][chrid(*it)];
        if (!v.size()) return false;
        for (int to : a.nodes[state][0])
            if (run(to, it))
                return true;
        for (int to : v)
            if (run(to, it + 1))
                return true;
        return false;
    }
    return a.qfinal.contains(state);
}

int main(int argc, char** argv) {
    a.deserialize();

    char** arg = &argv[1];
    while ((*arg)[0] == '-') {
        for (char* c = *arg + 1; *c; c++) {
            switch (*c) {
                case 'f': read_from_file = true; break;
                default: std::cerr << "bad parameter\n"; break;
            }
        }
        arg++;
    }

    std::ifstream ifs;
    std::stringstream iss;
    std::string line;
    if (read_from_file) ifs = std::ifstream(*arg);
    else iss = std::stringstream(*arg);
    std::istream& is = read_from_file ? (std::istream&)ifs : (std::istream&)iss;

    while (std::getline(is, line)) {
        if (line[0] == '/') continue;
        if (run(a.q0, line))
            std::cout << line << ';';
    }
    std::cout << '\n';
}
