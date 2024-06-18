#include "global.h"
#include "router.h"
#include "package.h"

namespace noc {

void Router::judge(int dy, int dx, int gate, Package * package) {
    // Deciding where to send
    dir send_dir = get_send_dir(dy, dx);

    if (answers[send_dir]) {
        sends[gate] = new SendInfo(send_dir, package);
        answers[send_dir] = false;
        return;
    }
    // TODO: smarter logic

    ask(send_dir);
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

/*
send_package(send_dir, package);
        remove_from_queue(gate, package);
        */