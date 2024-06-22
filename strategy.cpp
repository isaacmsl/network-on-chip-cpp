#include "global.h"
#include "router.h"
#include "package.h"

namespace noc {

bool Router::try_send(dir send_dir, int gate, Package * package) {
    if (answers[send_dir]) {
        sends[gate] = new SendInfo(send_dir, package);
        answers[send_dir] = false;
        return true;
    }
    return false;
}

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

static dir last_dir;

dir Router::strategy(int gate, int dy, int dx) {

    dir send_dir = last_dir;

    switch(attempts[gate]) {
        case 0:
            send_dir = get_send_dir(dy, dx);
            break;
        //attempts #1 (Isaac's strategy™)
        case 1:
            send_dir = get_closest_gate(dy, dx);
            break;
        //attempts #2 (Carlos's strategy™)
        case 2:
            for (int i{0};i < 4;i ++) {
            if (i != gate && !asked[i]) {
                send_dir = (dir) i;
            }}
            break;
    }

    return send_dir;
}

void Router::judge(int dy, int dx, int gate, Package * package) {
    // Deciding where to send
    dir send_dir;
    send_dir = strategy(gate, dy, dx);

    if (try_send(send_dir, gate, package)) {
        for (int i{0};i < 4;i ++) {
            asked[i] = false;
        }
        attempts[gate] = 0;
        return;
    }

    if (asked[send_dir]) ++ attempts[gate];
    send_dir = strategy(gate, dy, dx);
    ask(send_dir);
    asked[send_dir] = true;
    last_dir = send_dir;
}

const dir Router::get_send_dir(int dy, int dx) const {
    dir return_dir;

    if (dy != 0) {
        return_dir = dy > 0 ? S : N;
    }
    else if (dx != 0) {
        return_dir = dx > 0 ? E : W;
    }

    std::cout << dy << " "<< dx << " " << return_dir << '\n';

    return return_dir;
}

}