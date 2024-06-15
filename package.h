#ifndef PACKAGE_H
#define PACKAGE_H

class Package {
 private:
    int2 destination;
    int steps = 0;
    int body;

 public:
    const int2 get_destination() const {return destination;}
    const int get_age() const {return steps;}

    Package() = default;

    Package(int2 destination, int body): destination{destination}, body{body} {};

    void age() {this->steps++;}
};

#endif