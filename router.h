#ifndef ROUTER_H
#define ROUTER_H

class Router {
 private:
    int id;
    int n_neighbours;
    int * neighbours;
    bool active;

 public:
    int get_id() {return id;}

    Router() = default;

    Router(int id, int * neighbours) {

        this->id = id;
        this->n_neighbours = 4;
        this->active = true;
        this->neighbours = new int[n_neighbours];

        for (int i{0};i < n_neighbours;i ++) {
            this->neighbours[i] = neighbours[i];
        }
    }

    void set(bool active) {
        this->active = active;
    }
};

#endif