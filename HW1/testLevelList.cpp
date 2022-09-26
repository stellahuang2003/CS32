//FINAL

#include "LevelList.h"
#include <iostream>
#include <cassert>

int main()
{
    LevelList levels;
    levels.add(3);
    assert(levels.size() == 0);
    levels.add(45);
    assert(levels.size() == 1);
    assert(levels.remove(45) == true);
    assert(levels.size() == 0);
    for (int i = 0; i < 5; i++) {
        levels.add(i + 40);
    }
    assert(levels.size() == 5);
    assert(levels.minimum() == 40);
    assert(levels.maximum() == 44);
    std::cerr << "all passed" << std::endl;
}
