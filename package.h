#ifndef PACKAGE_H
#define PACKAGE_H

class Package {
 private:
    int destination;
    int steps;
    int body;

 public:
    const int get_destination() const {return destination;}
    const int get_age() const {return steps;}

    Package() = default;

    Package(int destination, int body): destination{destination}, body{body} {};

    void age() {
        this->steps++;
    }
};

#endif