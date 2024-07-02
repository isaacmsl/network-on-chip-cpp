#ifndef MESH2D_H
#define MESH2D_H

#include <cstdlib> 
#include "router.h"

namespace noc {

class Mesh2D {
 private:
    Router *** mesh;
    int w, h;
    bool do_populate;
    int populate_intensity;
    double arrival_rate = 1;
    int steps = 1;

 public:
    Mesh2D(int w, int h) {

        // Initializing variables

        this->w = w;
        this->h = h;

        this->mesh = new Router**[h];
        for (int i{0};i < h;i ++) {
            this->mesh[i] = new Router*[w];
        }

        this->do_populate = false;

        // Creating Routers

        for (int i{0};i < h;++i) {
        for (int j{0};j < w;++j) {
            this->mesh[i][j] = new Router(j + i*w, int2{i,j});
        }}

        for (int i{0};i < h;++i) {
        for (int j{0};j < w;++j) {
            bool N = i > 0,
                E = j < w - 1,
                S = i < h - 1,
                W = j > 0;
            
            // Clock-wise
            Router * neighbours[4] = {
                N ? get(i - 1, j + 0) : NULL,
                E ? get(i + 0, j + 1) : NULL,
                S ? get(i + 1, j + 0) : NULL,
                W ? get(i + 0, j - 1) : NULL
            };

            get(i, j)->set_neighbours(neighbours);
        }}
    }

    Router * get(int y, int x) const {return this->mesh[y][x];}

    void set(Router * r, int h, int w) {
        delete this->mesh[h][w];
        this->mesh[h][w] = r;
    }

    void set_router_active(int y, int x, bool active) {
        get(y, x)->set_active(active);}

    void spawn_package(int y, int x, int2 destination, int body = 0) {
        get(y, x)->spawn_package(destination, body);}

    void populate(bool active, int intensity) {
        this->do_populate = active;
        this->populate_intensity = intensity;
    }

    const void print() const {
        for (int i{0};i < h;i ++) {
        for (int j{0};j < w;j ++) {
            std::cout << get(i, j)->state() << ' ';
        }
            std::cout << '\n';
        }
    }

    double update_arrival_rate() {
        arrival_rate = ((steps - 1) * arrival_rate + pkgs_per_step)/((double) steps);
        std::cout << arrival_rate << '\n';
        return arrival_rate;
    }

    void step() {

        pkgs_per_step = 0;
        steps ++;

        // step
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->step();
        }}

        // mid-step
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->send();
        }}

        // post-step
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->ack();
        }}

        if (do_populate) {
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {

        float chances[3] = {0.01f, 0.1f, 0.3f};

        if ((rand() % 1000)/1000.0f <= chances[populate_intensity - 1]) {

            int2 rnd_dest{
                (i + ((rand() + 1) % (h - 1))) % h,
                (j + ((rand() + 1) % (w - 1))) % w};

            spawn_package(i, j, rnd_dest, rand() % 1024);

        }}}}
    }

    void print_package_info() {
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->print_package_info();
        }}
    }

    ~Mesh2D() {
        for (int i{0};i < h;i ++) {
        for (int j{0};j < w;j ++) {
            set(0, i, j);
        }
        delete this->mesh[i];
        }
        delete this->mesh;
    }

};
}
#endif