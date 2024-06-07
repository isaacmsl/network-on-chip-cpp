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

    const void ask(int neighbour) const {
        int neighbour_queue = this->n_neighbours - neighbour - 1;
        this->neighbours[neighbour]->ack(neighbour_queue);
    }

    const bool ack(int n) const {return (queues[n][this->queue_size-1] == NULL);}

    void set(bool active) {
        this->active = active;
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