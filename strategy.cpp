#include "global.h"
#include "router.h"
#include "package.h"

namespace noc {

// returns whether the g'th gate is to the left or right
int gx(int g) {
    if (g % 2 == 0) return 0;
    return g == 1 ? 1 : -1;
}

// returns whether the g'th gate is above or below
int gy(int g) {
    if (g % 2 == 1) return 0;
    return g == 0 ? -1 : 1;
}

// returns the direction of the closest gate from 'gate' to 'dy dx'
dir Router::get_closest_gate(int dy, int dx) {
    if (dy > 0) { // W, E or S

        if (dx > 0) {return dx > dy ? S : E;} // E or S
        else {return -dx > dy ? S : W;} // W or S

    } else  { // W, E or N

        if (dx > 0) {return dx > -dy ? N : E;} // E or N
        else {return -dx > -dy ? N : W;} // W or N
    }
}

static dir last_dir = X;

dir Router::strategy(int gate, int dy, int dx) {

    dir send_dir = last_dir;

    switch(attempts[gate]) {
        // optimal choice
        case 0:
            send_dir = get_send_dir(dy, dx);
            break;
        //attempt #1 (Isaac's strategy™)
        case 1:
            send_dir = get_closest_gate(dy, dx);
            break;
        //attempt #2 (Carlos's strategy™)
        case 2:
            for (int i{0};i < 4;i ++) {
            if (i != gate && !asked[i]) {
                send_dir = (dir) i;
            }}
            break;
    }

    return send_dir;
}

dir Router::avoid_starvation(Package * package) {
    int next_router_id_loop = package->check_loop(this->id);
    if (next_router_id_loop != -1) {
        for (int i{0}; i < this->n_neighbours; ++i) {
            if (this->neighbours[i] != NULL && this->neighbours[i]->id == next_router_id_loop) {
                return (dir) i;
            }
        }
    }
    return X;
}

void Router::judge(int dy, int dx, int gate, Package * package) {

    if (attempts[gate] > 3) attempts[gate] = 0;

    // Deciding where to send
    dir send_dir = strategy(gate, dy, dx);
    dir avoid_dir = avoid_starvation(package);

    //std::cout << "attempt: " << attempts[gate] << '\n';
    //std::cout << "i want to go: " << send_dir << " answer: " << answers[send_dir] <<  '\n';
    //std::cout << "i want to avoid: " << avoid_dir << '\n';

    if (send_dir == avoid_dir || send_dir == gate) {
        // send_dir will lead to a loop, trying another route
        asked[send_dir] = true;
        ++ attempts[gate];
        send_dir = strategy(gate, dy, dx);

        //std::cout << "new i want to go: " << send_dir << '\n';

    } else if (answers[send_dir]) {
        // Registering package for dispatch
        sends[gate] = new SendInfo(send_dir, package);

        // Reseting variables
        for (int i{0};i < 4;++ i) {asked[i] = false;}
        attempts[gate] = 0;
        answers[send_dir] = false;

        return;

    } else {
        // Failed to send, registering an attempt
        if (asked[send_dir]) {++ attempts[gate];}
        else {asked[send_dir] = true;}
    }

    ask(send_dir);
    last_dir = send_dir;
}

// Optimal sending direction
const dir Router::get_send_dir(int dy, int dx) const {
    dir return_dir;

    if (dy != 0) {
        return_dir = dy > 0 ? S : N;
    }
    else if (dx != 0) {
        return_dir = dx > 0 ? E : W;
    }

    return return_dir;
}

}