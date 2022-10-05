#include "automata.h"
#include <fstream>
#include <sstream>

bool read_from_file;
Automata input;

bool run(int state, std::string_view line) {
    for (auto it = line.begin(); it != line.end(); ++it) {
        auto& to = input.nodes[state][char_id(*it)];
        if (!to.size()) return false;
        for (int to : input.nodes[state][0]) {
            if (run(to, it)) {
                return true;
            }
        }
        for (int to : to) {
            if (run(to, it + 1)) {
                return true;
            }
        }
        return false;
    }
    return input.final_states.contains(state);
}

int main(int argc, char** argv) {
    input.deserialize();

    char** arg = &argv[1];
    while ((*arg)[0] == '-') {
        for (char* symbol = *arg + 1; *symbol; symbol++) {
            switch (*symbol) {
                case 'f': read_from_file = true; break;
                default: std::cerr << "bad parameter\n"; break;
            }
        }
        arg++;
    }

    std::ifstream ifs;
    std::stringstream iss;
    std::string line;
    if (read_from_file) {
        ifs = std::ifstream(*arg);
    }
    else {
        iss = std::stringstream(*arg);
    }
    std::istream& is = read_from_file ? (std::istream&)ifs : (std::istream&)iss;

    while (std::getline(is, line)) {
        if (line[0] == '/') {
            continue;
        }
        if (run(input.entry_state, line)) {
            std::cout << line << ';';
        }
    }
    std::cout << '\n';
}
