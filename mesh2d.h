#ifndef MESH2D_H
#define MESH2D_H

#include "router.h"

class Router;

class Mesh2D {
 private:
    Router *** mesh;
    int w, h;

 public:
    Mesh2D(int w, int h) {

        // Initializing variables

        this->w = w;
        this->h = h;

        this->mesh = new Router**[h];
        for (int i{0};i < h;i ++) {
            this->mesh[i] = new Router*[w];
        }

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

    const void print() const {
        for (int i{0};i < h;i ++) {
        for (int j{0};j < w;j ++) {
            std::cout << get(i, j)->state() << ' ';
        }
            std::cout << '\n';
        }
    }

    void step() {

        // step
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->step();
        }}

        // post-step
        for(int i{0}; i < h; ++i) {
        for (int j{0}; j < w; ++j) {
            get(i, j)->ack();
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

#endif