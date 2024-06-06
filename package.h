#ifndef PACKAGE_H
#define PACKAGE_H
#define DEFAULT_QUEUE_SIZE 4

#include <vector>

class Package {
 private:
    int destination;
    int steps;
    int body;

 public:
    int get_destination() {return destination;}
    int get_age() {return steps;}

    Package() = default;

    Package(int destination, int body): destination{destination}, body{body}  {
    }


    void age() {
        this->steps++;
    }
};

#endif