#include "automata.h"
#include <stack>

/* sub-automata */
struct Slice {
    int begin, end;
};

static Automata input;

static struct Slice slice_from_literal(char c) {
    int b = input.node();
    int e = input.node(b, c == '0' ? chrid('\e') : chrid(c));
    return { b, e };
}

static void slice_from_operator(char c, Slice* m, Slice ex) {
    int tmp;
    switch (c) {
        case '*':
            m->end = input.node(m->end);
            input.edge(m->end, m->begin);
            m->begin = m->end;
            break;
        case '.':
            input.edge(m->end, ex.begin);
            m->end = ex.end;
            break;
        case '+':
            tmp = input.node();
            input.edge(tmp, m->begin);
            input.edge(tmp, ex.begin);
            input.edge(ex.end, m->end = input.node(m->end));
            m->begin = tmp;
            break;
    }
}

static Slice compile(char e = '\n') {
    char c;
    int n_dots = 0;
    std::stack<Slice> sub;
    while ((c = std::cin.get()) != e) {
        switch (c) {
            case '.': break;
            case '*': slice_from_operator('*', &sub.top(), {}); break;
            case '+':
                while (--n_dots) {
                    Slice m = sub.top(); sub.pop();
                    slice_from_operator('.', &sub.top(), m);
                }
                break;
            default:
                sub.push(c == '(' ? compile(')') : slice_from_literal(c));
                n_dots++; /* implicit dot */
                break;
        }
    }
    n_dots--;
    while (sub.size() > 1) {
        Slice m = sub.top(); sub.pop();
        slice_from_operator(n_dots ? (n_dots--, '.') : '+', &sub.top(), m);
    }
    return sub.top();
}

int main(int argc, char** argv) {
    Slice s = compile();
    input.q0 = s.begin;
    input.qfinal.emplace(s.end);
    input.serialize();
}
