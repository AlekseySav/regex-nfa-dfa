#include "../automata.h"
#include <stack>

/* sub-automata */
struct Slice {
    int begin, end;
};

struct NFA_Builder {

    Automata input;

    struct Slice slice_from_literal(char symbol) {
        int begin = input.node();
        int end = input.node(begin, symbol == '0' ? EPSILON : char_id(symbol));
        return { .begin = begin, .end = end };
    }

    void slice_from_operator(char symbol, Slice* destination, Slice source) {
        int tmp;
        switch (symbol) {
            case '*':
                destination->end = input.node(destination->end);
                input.edge(destination->end, destination->begin);
                destination->begin = destination->end;
                break;
            case '.':
                input.edge(destination->end, source.begin);
                destination->end = source.end;
                break;
            case '+':
                tmp = input.node();
                input.edge(tmp, destination->begin);
                input.edge(tmp, source.begin);
                input.edge(source.end, destination->end = input.node(destination->end));
                destination->begin = tmp;
                break;
        }
    }

    /*
     * so
     * sort of wierd algorithm
     * '*' applied when read
     * '.' saved on stack and applied when next '+' read
     * '+' applied in the end
     * also dots are reduntand and applied implicitly
    */

    Slice compile(const char*& regex, char terminate_char = '\0') {
        char symbol;
        int n_dots = 0;
        std::stack<Slice> sub;
        while ((symbol = *regex++) != terminate_char) {
            switch (symbol) {
                case '.': break;
                case '*': slice_from_operator('*', &sub.top(), {}); break;
                case '+':
                    while (--n_dots) {
                        Slice destination = sub.top(); sub.pop();
                        slice_from_operator('.', &sub.top(), destination);
                    }
                    break;
                default:
                    sub.push(symbol == '(' ? compile(regex, ')') : slice_from_literal(symbol));
                    n_dots++; /* implicit dot */
                    break;
            }
        }
        n_dots--;
        while (sub.size() > 1) {
            Slice destination = sub.top();
            sub.pop();
            if (n_dots) {
                n_dots--;
                slice_from_operator('.', &sub.top(), destination);
            }
            else {
                slice_from_operator('+', &sub.top(), destination);
            }
        }
        return sub.top();
    }

};

Automata build_nfa(const char* regex) {
    NFA_Builder builder{};
    Slice s = builder.compile(regex);
    builder.input.entry_state = s.begin;
    builder.input.final_states.emplace(s.end);
    return builder.input;
}
