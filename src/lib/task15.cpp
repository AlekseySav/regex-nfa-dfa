#include "../automata.h"

int prac_task15(const std::string& regex, const std::string& u) {
    Automata language = build_nfa(regex.c_str());
    remove_epsilons(language);
    build_dfa(language);
    /*
     * terminal state is reachable form all verticies, so we can mark all as terminal
     * so, language now defines all prefixes too
     */
    int state = language.entry_state;
    int length = 0;
    for (char symbol : u) {
        if (!language.nodes[state].contains(char_id(symbol))) {
            break;
        }
        state = *language.nodes[state][char_id(symbol)].begin();
        length++;
    }
    return length;
}
