#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <array>

using int2 = std::array<int, 2>;

enum dir {
    N, E, S, W, X
};

class Mesh2D;
class Router;
class Package;

static int pkgs_per_step = 0;

#endif