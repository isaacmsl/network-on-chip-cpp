#ifndef ROUTER_H
#define ROUTER_H

#define DEFAULT_QUEUE_SIZE 4

#include "package.h"

namespace noc {

struct SendInfo {
    dir send_dir;
    Package * pkg;

    SendInfo(dir send_dir, Package * pkg) : send_dir{send_dir}, pkg{pkg} {}
    SendInfo() = default;
};

class Router {
 private:
    int id;
    int2 pos;
    int queue_size;
    int n_neighbours;
    bool active;
    Router ** neighbours;
    Package *** queues;
    SendInfo ** sends;
    bool * asked;
    int * attempts;

 public:

    // Accessed by other Routers.
    // array index is wich neighbour...
    bool * questions;// ...asked
    bool * answers;// ...answered

    /// === Constructors ===

    Router() = default;

    Router(int id, int2 pos, int queue_size = DEFAULT_QUEUE_SIZE) {
        this->id = id;
        this->pos = pos;
        this->n_neighbours = 4;
        this->active = true;
        this->queue_size = queue_size;
        this->queues = new Package**[n_neighbours];
        this->questions = new bool[n_neighbours];
        this->answers = new bool[n_neighbours];
        this->sends = new SendInfo*[n_neighbours];
        this->asked = new bool[n_neighbours];
        this->attempts = new int[n_neighbours];

        for (int i{0};i < n_neighbours;++i) {
            // Loading queues with null packages
            this->queues[i] = new Package*[this->queue_size];
            this->questions[i] = false;
            this->answers[i] = false;
            this->sends[i] = NULL;
            this->asked[i] = false;
            this->attempts[i] = 0;
            for (int j{0};j < this->queue_size;j ++) {
                this->queues[i][j] = NULL;
            }
        }
    }

    /// === Getters & Setters ===

    int get_id() {return id;}

    void set_active(bool active) {this->active = active;}

    void set_neighbours(Router ** neighbours) {
        this->neighbours = new Router*[this->n_neighbours];

        for (int i{0};i < this->n_neighbours;++i) {
            this->neighbours[i] = neighbours[i];
        }
    }

    /// === Interaction with other Routers ===

    // Gets index of the 'neighbour' gate that is facing
    // towards this router.
    const int fx(int neighbour) const {return (neighbour + 2) % 4;}

    const void ask(int neighbour) const {
        if (neighbours[neighbour] != NULL) {
            int neighbour_queue = fx(neighbour);
            neighbours[neighbour]->questions[neighbour_queue] = true;
        }
        else {
            std::cout << "Non-existent neighbour\n";
        }
    }

    // fills answer array 
    const void ack() const {
    for (int i{0};i < this->n_neighbours;++i) {
        if (active && this->questions[i]) {
            bool is_gate_available = (queues[i][queue_size-1] == NULL);
            neighbours[i]->answers[fx(i)] = is_gate_available;
            questions[i] = false;
        }
        if (!active) questions[i] = false;
    }}

    // sends 'package' to 'neighbour'
    void send_package(int neighbour, Package * package) {
        int neighbour_queue = fx(neighbour);
        if (neighbours[neighbour] != NULL) {
            package->add_to_history(this->id);
            package->age();
            neighbours[neighbour]->add_to_queue(neighbour_queue, package);
        }
    }

    // router logic
    void step() {
    for (int i{0}; i < n_neighbours; ++i) {
        if (package_count(i)) {
            process_pkg(i);
        }
    }}

    // sends pending packages in 'sends' array
    void send() {
    for (int i{0}; i < n_neighbours; ++i) {
        if (sends[i] != NULL) {
            //std::cout << "did it " << id << '\n';
            send_package(sends[i]->send_dir, sends[i]->pkg);
            remove_from_queue(i, sends[i]->pkg);
            sends[i] = NULL;
        }
    }}

    // Receives a package and decides where the others should go
    void process_pkg(int gate) {

        // Getting Package
        auto package = get_senior_package_in_queue(gate);
        auto dest = package->get_destination();

        const auto dy = dest[0] - pos[0];
        const auto dx = dest[1] - pos[1];

        // Package arrived at final destination
        if (dy == 0 && dx == 0) {
            std::cout << "Gatão pego! " << package->get_body() << "\n";
            remove_from_queue(gate, package);
            delete package;
            return;
        }

        // Passing package to next router
        judge(dy, dx, gate, package);
    }

    dir avoid_starvation(Package * package);

    void judge(int dy, int dx, int gate, Package * package);

    const dir get_send_dir(int dy, int dx) const;

    dir strategy(int gate, int dy, int dx);

    bool try_send(dir send_dir, int gate, Package * package);

    dir get_closest_gate(int dy, int dx);

    /// === Queue & Packages ===

    Package * get_senior_package_in_queue(int queue) {
        auto senior = queues[queue][0];
        for (int j{0}; j < package_count(queue); ++j) {
            auto package = queues[queue][j];
            if (package->get_age() > senior->get_age())
                senior = package;
        }
        return senior;
    }

    void add_to_queue(int queue, Package * pack) {
        for (int i{0};i < queue_size;i ++) {
            if (queues[queue][i] == NULL) {
                queues[queue][i] = pack;
                return;
            }
        }

        std::cout << "Vixe! O queue " << id << " tava cheio o_o\n";
    }

    void remove_from_queue(int queue, int pos) {
        queues[queue][pos] = NULL;
        update_queue(queue);
    }

    void remove_from_queue(int queue, Package * pack) {
        for (int i = 0;i < queue_size;i ++) {
            if (queues[queue][i] == pack) {
                remove_from_queue(queue, i);
                break;
            }
        }
    }

    void update_queue(int queue) {
        for (int i{0};i < queue_size - 1;i ++) {
            // removing empty spaces between packages in queue
            if (queues[queue][i] == NULL && queues[queue][i + 1] != NULL) {
                queues[queue][i] = queues[queue][i + 1];
                queues[queue][i + 1] = NULL;
            }
        }
    }

    void update_queue() {
        for (int i{0};i < n_neighbours;i ++) {update_queue(i);}
    }

    const bool has_package() const {return package_count() != 0;}

    // number of packages in queue 'n'
    const int package_count(int n) const {
        int k = 0;
        for (int j{0};j < queue_size;j ++) {
            k += (queues[n][j] != NULL) ? 1 : 0;
        }
        return k;
    }

    // number of packages in total
    const int package_count() const {
        int k = 0;
        for (int i{0};i < n_neighbours;i ++) {
            k += package_count(i);
        }
        return k;
    }

    // add a package to some queue
    void spawn_package(int2 destination, int body) {

        int queue = destination[0] > pos[0] ? 
                    0 : (destination[0] < pos[0] ? 
                    2 : (destination[1] > pos[1]) ? 3 : 1);

        add_to_queue(queue, new Package(destination, body));}

    /// === Destructor ===

    ~Router() {
        delete neighbours;

        for (int i{0};i < n_neighbours;i ++) {
        for (int j{0};j < queue_size;j ++) {
            delete queues[i][j]; // deleting packages
        } 
            delete queues[i]; // deleting queues
        }

        delete queues; // deleting queue list
    }

    /// === Cosmetics ===

    const char state() const {
        if (!active) return 'x';
        if (has_package()) return '0' + package_count();
        return 'o';
    }

    const void print_package_info() {
        for (int i = 0;i < n_neighbours;i ++) {
        for (int j{0};j < queue_size;j ++) {
            if (queues[i][j] != NULL)
                queues[i][j]->print_info(this->id, pos, i, j);
        }}
    }

};
}
#endif