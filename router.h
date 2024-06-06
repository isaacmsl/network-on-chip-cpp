#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include "package.h"
#include "mesh2d.h"

#define DEFAULT_QUEUE_SIZE 4

class Router {
 private:
    int id;
    int queue_size;
    int n_neighbours;
    int * neighbours;
    bool active;
    Package ** queues;
    Mesh2D * mesh;

 public:
    int get_id() {return id;}

    Router() = default;

    Router(int id, int * neighbours, Mesh2D * mesh, int queue_size = DEFAULT_QUEUE_SIZE) {
        this->id = id;
        this->n_neighbours = 4;
        this->active = true;
        this->queue_size = queue_size;
        this->neighbours = new int[n_neighbours];
        this->mesh = mesh;
        this->queues = new Package*[n_neighbours];

        for (int i{0};i < n_neighbours;++i) {
            this->neighbours[i] = neighbours[i];
            // TODO: Destructor
            this->queues[i] = new Package[this->queue_size];
        }
    }

    void ask(int neighbour){
        int neighbour_queue = this->n_neighbours - neighbour - 1;
        this->mesh.ask(neighbours[neighbour], neighbour_queue);
    }

    bool ack(int n){return queues[n][this->queue_size-1] == NULL;}

    void set(bool active) {
        this->active = active;
    }
};

#endif