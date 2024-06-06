#ifndef ROUTER_H
#define ROUTER_H
#define DEFAULT_QUEUE_SIZE 4

#include <vector>
#include "package.h"

class Router {
 private:
    int id;
    int n_neighbours;
    int * neighbours;
    bool active;
    Package ** queues;  

 public:
    int get_id() {return id;}

    Router() = default;

    Router(int id, int * neighbours, int queue_size = DEFAULT_QUEUE_SIZE) {

        this->id = id;
        this->n_neighbours = 4;
        this->active = true;
        this->neighbours = new int[n_neighbours];
        this->queues = new Package*[n_neighbours];

        for (int i{0};i < n_neighbours;++i) {
            this->neighbours[i] = neighbours[i];
            // TODO: Destructor
            this->queues[i] = new Package[queue_size];
        }
    }



    void set(bool active) {
        this->active = active;
    }
};

#endif