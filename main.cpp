#include "global.h"
#include "mesh2d.h"

int main() {

    noc::Mesh2D m = noc::Mesh2D(8, 8);
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
            std::cout << "2 - de/activate router\n";
            std::cout << "3 - populate\n";
            std::cout << "4 - package info\n";
            std::cout << "5 - next step + package info\n";
            std::cout << "6 - quit\n";

            int cmd;
            std::cin >> cmd;

            switch(cmd) {

                // Next Step
                case 0:
                    next_step = true;
                    ++step;
                    m.step();
                    break;

                // Insert Package
                case 1:
                {
                    int y, x, dy, dx, body;

                    std::cout << "type: y x dx dy body \n";

                    std::cin >> y >> x >> dy >> dx >> body;

                    m.spawn_package(y, x, int2{dy, dx}, body);
                }

                    break;

                // De/activate Router
                case 2:
                {
                    int y, x;
                    bool active;

                    std::cout << "type: y x active \n";

                    std::cin >> y >> x >> active;

                    m.set_router_active(y, x, active);
                }

                    break;

                // Populate
                case 3:
                    bool active;
                    int intensity;

                    std::cout << "type: active intensity [1, 3] \n";

                    std::cin >> active >> intensity;

                    m.populate(active, intensity);

                    break;

                // Package Info
                case 4:

                    m.print_package_info();

                    break;

                // Next Step + Package Info
                case 5:

                    m.print_package_info();

                    next_step = true;
                    ++step;
                    m.step();

                    break;

                // Quit
                case 6:
                    quit = true;
                    break;
            }

            std::cout << '\n';

        }
    }

    return 0;
}