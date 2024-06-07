#include <iostream>
#include <array>

using int2 = std::array<int, 2>;

#include "mesh2d.h"

int main() {

    Mesh2D m = Mesh2D(8, 8);
    bool quit = false;
    int step = 0;

    while (!quit) {

        std::cout << "---------------\n";
        m.print();

        bool next_step = false;

        while (!next_step && !quit) {

            std::cout << "step: " << step << '\n';
            std::cout << "0 - next step\n";
            std::cout << "1 - insert package\n";
            std::cout << "2 - quit\n";

            int cmd;
            std::cin >> cmd;

            switch(cmd) {
                case 0:
                    next_step = true;
                    step ++;
                    break;

                case 1:
                    int y, x, dy, dx, body;

                    std::cout << "type: y x dx dy body: \n";

                    std::cin >> y >> x >> dy >> dx >> body;

                    m.spawn_package(y, x, int2{dy, dx}, body);

                    break;

                case 2:
                    quit = true;
                    break;
            }

            std::cout << '\n';

        }
    }

    return 0;
}