#include "automata.h"
#include <cassert>

void run_tests();

int main(int argc, char** argv) {
    if (argc == 3) {
        std::cout << prac_task15(argv[1], argv[2]) << '\n';
    }
    else {
        run_tests();
    }
}

/*
 * NOTE: this only tests prac_test15()
 * checkout tests/unit/ and tests/run-regex/ for tests on other submodules
 */
void run_tests() {
    assert(prac_task15("0", "")                                         == 0);
    assert(prac_task15("0", "a")                                        == 0);
    assert(prac_task15("0", "axsazd")                                   == 0);
    assert(prac_task15("a", "a")                                        == 1);
    assert(prac_task15("a", "aaa")                                      == 1);
    assert(prac_task15("aaa", "a")                                      == 1);
    assert(prac_task15("aaa", "aa")                                     == 2);
    assert(prac_task15("a*", "")                                        == 0);
    assert(prac_task15("a*", "a")                                       == 1);
    assert(prac_task15("a*", "aa")                                      == 2);
    assert(prac_task15("a*", "aaaa")                                    == 4);
    assert(prac_task15("(a+b)*", "a")                                   == 1);
    assert(prac_task15("(a+b)*", "ab")                                  == 2);
    assert(prac_task15("(a+b)*", "abb")                                 == 3);
    assert(prac_task15("(a+b)*", "ba")                                  == 2);
    assert(prac_task15("(a+b)*", "bab")                                 == 3);
    assert(prac_task15("(a+b)*", "baa")                                 == 3);
    assert(prac_task15("(a+b)*", "bbba")                                == 4);
    assert(prac_task15("(a+b)*", "ac")                                  == 1);
    assert(prac_task15("(a+b)*", "abc")                                 == 2);
    assert(prac_task15("(a+b)*", "abbc")                                == 3);
    assert(prac_task15("(a+b)*", "bac")                                 == 2);
    assert(prac_task15("(a+b)*", "babc")                                == 3);
    assert(prac_task15("(a+b)*", "baac")                                == 3);
    assert(prac_task15("(a+b)*", "bbbac")                               == 4);
    assert(prac_task15("(a+b)*", "ca")                                  == 0);
    assert(prac_task15("(a+b)*", "cab")                                 == 0);
    assert(prac_task15("(a+b)*", "cabb")                                == 0);
    assert(prac_task15("(a+b)*", "cba")                                 == 0);
    assert(prac_task15("(a+b)*", "cbab")                                == 0);
    assert(prac_task15("(a+b)*", "cbaa")                                == 0);
    assert(prac_task15("(a+b)*", "cbbba")                               == 0);
    assert(prac_task15("c(a+b)*c", "cbbba")                             == 5);
    assert(prac_task15("c(a+b)*c", "cbbbac")                            == 6);
    assert(prac_task15("c(a+b)*c", "cbbbacd")                           == 6);
    assert(prac_task15("c(a+b)*c", "cbqbbac")                           == 2);
    assert(prac_task15("c*a+d", "cca")                                  == 3);
    assert(prac_task15("c*a+d", "a")                                    == 1);
    assert(prac_task15("c*a+d", "ca")                                   == 2);
    assert(prac_task15("c*a+d", "d")                                    == 1);
    assert(prac_task15("c*a+d", "cad")                                  == 2);
    assert(prac_task15("c*a+d", "cd")                                   == 1);
    assert(prac_task15("cdvb+xyz", "cdvb")                              == 4);
    assert(prac_task15("cdvb+xyz", "cdv")                               == 3);
    assert(prac_task15("cdvb+xyz", "cd")                                == 2);
    assert(prac_task15("cdvb+xyz", "c")                                 == 1);
    assert(prac_task15("cdvb+xyz", "")                                  == 0);
    assert(prac_task15("cdvb+xyz", "x")                                 == 1);
    assert(prac_task15("cdvb+xyz", "xy")                                == 2);
    assert(prac_task15("cdvb+xyz", "xyz")                               == 3);
    assert(prac_task15("cdvb+xyz", "cdvbx")                             == 4);
    assert(prac_task15("cdvb+xyz", "cdvx")                              == 3);
    assert(prac_task15("cdvb+xyz", "cdx")                               == 2);
    assert(prac_task15("cdvb+xyz", "cx")                                == 1);
    assert(prac_task15("cdvb+xyz", "xc")                                == 1);
    assert(prac_task15("cdvb+xyz", "xyc")                               == 2);
    assert(prac_task15("cdvb+xyz", "xyzc")                              == 3);
    assert(prac_task15("aa*", "aaa")                                    == 3);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "aaa")    == 3);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "bcx")    == 3);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "bcy")    == 3);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "bcc")    == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "xyz")    == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "xy")     == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "z")      == 1);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "zz")     == 1);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "bca")    == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "abb")    == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "abx")    == 2);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "ax")     == 1);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "x")      == 1);
    assert(prac_task15("(a*.bc.(x+y)).ea+b+cde.qwer+(x.y)+z", "b")      == 1);
    std::cerr << "tests passed? yeah, tests passed.\n";
}
