#include "automata.h"
#include <stack>

/* sub-automata */
struct Slice {
    int begin, end;
};

static Automata input;

static struct Slice slice_from_literal(char symbol) {
    int begin = input.node();
    int end = input.node(begin, symbol == '0' ? char_id('\e') : char_id(symbol));
    return { .begin = begin, .end = end };
}

static void slice_from_operator(char symbol, Slice* dest, Slice source) {
    int tmp;
    switch (symbol) {
        case '*':
            dest->end = input.node(dest->end);
            input.edge(dest->end, dest->begin);
            dest->begin = dest->end;
            break;
        case '.':
            input.edge(dest->end, source.begin);
            dest->end = source.end;
            break;
        case '+':
            tmp = input.node();
            input.edge(tmp, dest->begin);
            input.edge(tmp, source.begin);
            input.edge(source.end, dest->end = input.node(dest->end));
            dest->begin = tmp;
            break;
    }
}

static Slice compile(char terminate_char = '\n') {
    char symbol;
    int n_dots = 0;
    std::stack<Slice> sub;
    while ((symbol = std::cin.get()) != terminate_char) {
        switch (symbol) {
            case '.': break;
            case '*': slice_from_operator('*', &sub.top(), {}); break;
            case '+':
                while (--n_dots) {
                    Slice dest = sub.top(); sub.pop();
                    slice_from_operator('.', &sub.top(), dest);
                }
                break;
            default:
                sub.push(symbol == '(' ? compile(')') : slice_from_literal(symbol));
                n_dots++; /* implicit dot */
                break;
        }
    }
    n_dots--;
    while (sub.size() > 1) {
        Slice dest = sub.top();
        sub.pop();
        slice_from_operator(n_dots ? (n_dots--, '.') : '+', &sub.top(), dest);
    }
    return sub.top();
}

int main(int argc, char** argv) {
    Slice s = compile();
    input.entry_state = s.begin;
    input.final_states.emplace(s.end);
    input.serialize();
}
