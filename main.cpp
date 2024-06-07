#include <iostream>
#include <array>

using int2 = std::array<int, 2>;

#include "mesh2d.h"

int main() {

    Mesh2D m = Mesh2D(8, 8);

    m.spawn_package(1, 1, int2{9, 9});

    m.print();

    return 0;
}