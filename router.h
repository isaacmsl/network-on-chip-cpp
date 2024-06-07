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
    int get_id() {return id;}

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

    void set(bool active) {
        this->active = active;
    }

    const void ask(int neighbour) const {
        int neighbour_queue = this->n_neighbours - neighbour - 1;
        this->neighbours[neighbour]->ack(neighbour_queue);
    }

    const bool ack(int n) const {
        return queues[n][this->queue_size-1] == NULL;
    }

    const char state() const {
        if (!active) return 'x';
        if (has_package()) return '0' + package_count();
        return 'o';
    }

    // add a package to some queue
    void spawn_package(int2 destination, int body) {
        add_package_to_queue(0, new Package(destination, body));
    }

    void add_package_to_queue(int queue, Package * pack) {
        for (int i{0};i < queue_size;i ++) {
            if (queues[queue][i] == NULL) {
                this->queues[queue][i] = pack;
                return;
            }
        }

        std::cout << "Vixe! O queue " << id << " tava cheio o_o\n";
    }

    const bool has_package() const {
        return package_count() != 0;
    }

    // number of packages in queue 'n'
    const int package_count(int n) const {
        int k = 0;
        for (int j{0};j < this->queue_size;j ++) {
            k += (this->queues[n][j] != NULL) ? 1 : 0;
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

    ~Router() {
        delete neighbours;

        for (int i{0};i < n_neighbours;i ++) {
        for (int j{0};j < queue_size;j ++) {
            delete queues[i][j]; // deleting packages
        } 
            delete this->queues[i]; // deleting queues
        }

        delete this->queues; // deleting queue list
    }
};

#endif