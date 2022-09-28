#include "automata.h"
#include <stack>

/* sub-automata */
struct Slice {
    int begin, end;
};

static Automata a;

static struct Slice s1(char c) {
    int b = a.node();
    int e = a.node(b, chrid(c));
    return { b, e };
}

static void apply(char c, Slice* m, Slice ex) {
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
            case '*': apply('*', &sub.top(), {}); break;
            case '+':
                while (--n_dots) {
                    Slice m = sub.top(); sub.pop();
                    apply('.', &sub.top(), m);
                }
                break;
            default:
                sub.push(c == '(' ? compile(')') : s1(c));
                n_dots++; /* implicit dot */
                break;
        }
    }
    n_dots--;
    while (sub.size() > 1) {
        Slice m = sub.top(); sub.pop();
        apply(n_dots ? (n_dots--, '.') : '+', &sub.top(), m);
    }
    return sub.top();
}

int main(int argc, char** argv) {
    Slice s = compile();
    a.q0 = s.begin;
    a.F.emplace(s.end);
    a.serialize();
}
