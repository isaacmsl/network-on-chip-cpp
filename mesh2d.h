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

        for (int i{0};i < h;i ++) {
        for (int j{0};j < w;j ++) {

            int N = i > 0 ? j + (i - 1)*w : -1,
                E = j < w - 1 ? j + 1 + i*w : -1,
                S = i < h - 1 ? j + (i + 1)*w : -1,
                W = j > 0 ? j - 1 + i*w : -1;
            
            // Clock-wise
            Router * neighbours[4] = {
                N >= 0 ? get(i - 1, j + 0) : NULL,
                E >= 0 ? get(i + 0, j + 1) : NULL,
                S >= 0 ? get(i + 1, j + 0) : NULL,
                W >= 0 ? get(i + 0, j - 1) : NULL
            };

            this->mesh[i][j] = new Router(j + i*w, neighbours);
        }
        }
    }

    Router * get(int y, int x){
        return this->mesh[y][x];
    }

    void set(Router * r, int h, int w) {
        delete this->mesh[h][w];
        this->mesh[h][w] = r;
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