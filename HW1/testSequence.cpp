//FINAL

#include "newSequence.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Sequence s;
    assert(s.empty());
    assert(s.find(42) == -1);
    assert(s.insert(42) == 0);
    assert(s.size() == 1  &&  s.find(42) == 0);

    Sequence hello;
    hello.insert(0, 1);
    hello.insert(1, 2);
    hello.insert(0, 0);
    hello.insert(3, 3);
    s.remove(42);
    hello.swap(s);
    assert(hello.size() == 0);
    assert(hello.find(42) == -1);
    assert(s.size() == 4 && s.find(1) == 1);
    cout << "Passed all tests" << endl;
}
