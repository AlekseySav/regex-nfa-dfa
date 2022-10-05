#include "automata.h"
#include <stack>

/* sub-automata */
struct Slice {
    int begin, end;
};

static Automata a;

static struct Slice slice_from_literal(char c) {
    int b = a.node();
    int e = a.node(b, c == '0' ? chrid('\e') : chrid(c));
    return { b, e };
}

static void slice_from_operator(char c, Slice* m, Slice ex) {
    int tmp;
    switch (c) {
        case '*':
            m->end = a.node(m->end);
            a.edge(m->end, m->begin);
            m->begin = m->end;
            break;
        case '.':
            a.edge(m->end, ex.begin);
            m->end = ex.end;
            break;
        case '+':
            tmp = a.node();
            a.edge(tmp, m->begin);
            a.edge(tmp, ex.begin);
            a.edge(ex.end, m->end = a.node(m->end));
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
    a.q0 = s.begin;
    a.qfinal.emplace(s.end);
    a.serialize();
}
