#include "global.h"
#include "router.h"
#include "package.h"

namespace noc {

void Router::try_send(dir send_dir, int gate, Package * package) {
    if (answers[send_dir]) {
        sends[gate] = new SendInfo(send_dir, package);
        answers[send_dir] = false;
        return;
    }
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

dir Router::get_closest_gate(int gate, int dy, int dx) {

    int shortest_dis = 0;
    int closest_gate = 0;

    for (int i{0};i < n_neighbours;i ++) {
        if (i == gate) continue;

        int dx_ = dx - (id + gx(i));
        int dy_ = dy - (id + gy(i));

        int new_dis = dx_*dx_ + dy_*dy_; 

        if (shortest_dis < new_dis) {
            // TODO: Everything
        }
    }
}

void Router::judge(int dy, int dx, int gate, Package * package) {
    // Deciding where to send
    dir send_dir = get_send_dir(dy, dx);

    try_send(send_dir, gate, package);
    // TODO: smarter logic

    if (!asked[gate]) {
        ask(send_dir);
    }
    else {
        asked[gate] = false;
        // Search new path

        switch(attempts[gate]) {
        //attempts #1 (Isaac's strategy TM)
            case 0:
                send_dir = get_closest_gate(gate, dy, dx);
                try_send(send_dir, gate, package);
                break;

        }
    }
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