#ifndef ROUTER_H
#define ROUTER_H

#include "package.h"

#define DEFAULT_QUEUE_SIZE 4

class Router {
 private:
    int id;
    int queue_size;
    int n_neighbours;
    bool active;
    Router ** neighbours;
    Package *** queues;

 public:

    /// === Constructors ===

    Router() = default;

    Router(int id, Router ** neighbours, int queue_size = DEFAULT_QUEUE_SIZE) {
        this->id = id;
        this->n_neighbours = 4;
        this->active = true;
        this->queue_size = queue_size;
        this->neighbours = new Router*[n_neighbours];
        this->queues = new Package**[n_neighbours];

        for (int i{0};i < n_neighbours;++i) {
            this->neighbours[i] = neighbours[i];

            // Loading queues with null packages
            this->queues[i] = new Package*[this->queue_size];
            for (int j{0};j < this->queue_size;j ++) {
                this->queues[i][j] = NULL;
            }
        }
    }

    /// === Getters & Setters ===

    int get_id() {return id;}

    void set_active(bool active) {
        this->active = active;
    }

    /// === Interaction with other Routers ===

    const void ask(int neighbour) const {
        int neighbour_queue = n_neighbours - neighbour - 1;
        neighbours[neighbour]->ack(neighbour_queue);
    }

    const bool ack(int q) const {
        return queues[q][queue_size-1] == NULL;
    }

    void send_package(int neighbour, Package * package) {
        int neighbour_queue = n_neighbours - neighbour - 1;

        neighbours[neighbour]->add_to_queue(neighbour_queue, package);
    }

    /// === Queue & Packages ===

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
        queues[queue][pos] == NULL;
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
        for (int i{0};i < n_neighbours;i ++) {
            update_queue(i);
        }
    }

    const bool has_package() const {
        return package_count() != 0;
    }

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
        add_to_queue(0, new Package(destination, body));
    }

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
};

#endif