#ifndef MESH2D_H
#define MESH2D_H

#include "router.h"

class Mesh2D {
 private:
    Router *** mesh;
    int w, h;

 public:
    Mesh2D(int w, int h) {
        this->w = w;
        this->h = h;

        this->mesh = new Router**[h];
        for (int i{0};i < h;i ++) {
            this->mesh[i] = new Router*[w];
        }

        for (int i{0};i < h;i ++) {
        for (int j{0};j < w;j ++) {

            int N = i > 0 ? j + (i - 1)*w : -1,
                E = j < w - 1 ? j + 1 + i*w : -1,
                S = i < h - 1 ? j + (i + 1)*w : -1,
                W = j > 0 ? j - 1 + i*w : -1;
            
            int neighbours[4] = {N, E, S, W};

            this->mesh[i][j] = new Router(j + i*w, neighbours);
        }
        }
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