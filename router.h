#ifndef ROUTER_H
#define ROUTER_H

#include "package.h"

#define DEFAULT_QUEUE_SIZE 4

class Router {
 private:
    int id;
    int2 pos;
    int queue_size;
    int n_neighbours;
    bool active;
    Router ** neighbours;
    Package *** queues;

 public:

    // Acessed by other Routers; index is wich neighbour...
    bool * questions;//...asked
    bool * answers;//...answered

    /// === Constructors ===

    Router() = default;

    Router(int id, int2 pos, int queue_size = DEFAULT_QUEUE_SIZE) {
        this->id = id;
        this->pos = pos;
        this->n_neighbours = 4;
        this->active = true;
        this->queue_size = queue_size;
        this->queues = new Package**[n_neighbours];
        this->questions = new bool[n_neighbours];
        this->answers = new bool[n_neighbours];

        for (int i{0};i < n_neighbours;++i) {
            // Loading queues with null packages
            this->queues[i] = new Package*[this->queue_size];
            this->questions[i] = false;
            this->answers[i] = false;
            for (int j{0};j < this->queue_size;j ++) {
                this->queues[i][j] = NULL;
            }
        }
    }

    /// === Getters & Setters ===

    int get_id() {return id;}

    void set_active(bool active) {this->active = active;}

    void set_neighbours(Router ** neighbours) {
        this->neighbours = new Router*[this->n_neighbours];

        for (int i{0};i < this->n_neighbours;++i) {
            this->neighbours[i] = neighbours[i];
        }
    }

    /// === Interaction with other Routers ===

    const int fx(int neighbour) const {return (neighbour + 2) % 4;}

    const void ask(int neighbour) const{
        int neighbour_queue = fx(neighbour);
        neighbours[neighbour]->questions[neighbour_queue] = true;
    }

    // fills answer array 
    const void ack() {
    for (int i{0};i < this->n_neighbours;++i) {
    if (this->questions[i] == true) { /// has to be == true ?!??
        bool is_gate_available = (queues[i][queue_size-1] == NULL);
        neighbours[i]->answers[fx(i)] = is_gate_available;
        questions[i] = false;
    }}}

    void send_package(int neighbour, Package * package) {
        int neighbour_queue = fx(neighbour);
        if (neighbours[neighbour] != NULL)
            neighbours[neighbour]->add_to_queue(neighbour_queue, package);
    }

    void step() {
    for (int i{0}; i < n_neighbours; ++i) {
        if (package_count(i)) {judge(i);}
        answers[fx(i)] = false;
    }}

    void judge(int gate) {
        auto package = get_senior_package_in_queue(gate);
        auto dest = package->get_destination();

        const auto dy = dest[0] - pos[0];
        const auto dx = dest[1] - pos[1];

        // Package arrived
        if (dy == 0 && dx == 0) {
            std::cout << "Gatão\n";
            remove_from_queue(gate, package);
            delete package;
            return;
        }

        // Deciding where to send

        dir send_dir = get_send_dir(dy, dx);

        if (answers[send_dir] == true) {
            send_package(send_dir, package);
            remove_from_queue(gate, package);
            return;
        }
        // TODO: smarter logic

        ask(send_dir);
    }

    const dir get_send_dir(int dy, int dx) const {
        dir return_dir;

        if (dy != 0) {
            return_dir = dy > 0 ? S : N;
        }
        else if (dx != 0) {
            return_dir = dx > 0 ? E : W;
        }

        std::cout << dy << " "<< dx << " " << return_dir << '\n';

        return return_dir;
    }

    /// === Queue & Packages ===

    Package * get_senior_package_in_queue(int queue) {
        auto senior = queues[queue][0];
        for (int j{0}; j < package_count(queue); ++j) {
            auto package = queues[queue][j];
            if (package->get_age() > senior->get_age())
                senior = package;
        }
        return senior;
    }

    void add_to_queue(int queue, Package * pack) {
        for (int i{0};i < queue_size;i ++) {
            if (queues[queue][i] == NULL) {
                queues[queue][i] = pack;
                return;
            }
        }

        std::cout << "Vixe! O queue " << id << " tava cheio o_o\n";
    }

    void remove_from_queue(int queue, int pos) {
        queues[queue][pos] = NULL;
        update_queue(queue);
    }

    void remove_from_queue(int queue, Package * pack) {
        for (int i = 0;i < queue_size;i ++) {
            if (queues[queue][i] == pack) {
                remove_from_queue(queue, i);
                break;
            }
        }
    }

    void update_queue(int queue) {
        for (int i{0};i < queue_size - 1;i ++) {
            // removing empty spaces between packages in queue
            if (queues[queue][i] == NULL && queues[queue][i + 1] != NULL) {
                queues[queue][i] = queues[queue][i + 1];
                queues[queue][i + 1] = NULL;
            }
        }
    }

    void update_queue() {
        for (int i{0};i < n_neighbours;i ++) {update_queue(i);}
    }

    const bool has_package() const {return package_count() != 0;}

    // number of packages in queue 'n'
    const int package_count(int n) const {
        int k = 0;
        for (int j{0};j < queue_size;j ++) {
            k += (queues[n][j] != NULL) ? 1 : 0;
        }
        return k;
    }

    // number of packages in total
    const int package_count() const {
        int k = 0;
        for (int i{0};i < n_neighbours;i ++) {
            k += package_count(i);
        }
        return k;
    }

    // add a package to some queue
    void spawn_package(int2 destination, int body) {
        add_to_queue(0, new Package(destination, body));}

    /// === Destructor ===

    ~Router() {
        delete neighbours;

        for (int i{0};i < n_neighbours;i ++) {
        for (int j{0};j < queue_size;j ++) {
            delete queues[i][j]; // deleting packages
        } 
            delete queues[i]; // deleting queues
        }

        delete queues; // deleting queue list
    }

    /// === Cosmetics ===

    const char state() const {
        if (!active) return 'x';
        if (has_package()) return '0' + package_count();
        return 'o';
    }
};

#endif