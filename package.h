#ifndef PACKAGE_H
#define PACKAGE_H
#define HISTORY_SIZE 8
#define MAX_SIZE_IDS 64

namespace noc {

class Package {
 private:
    int2 destination;
    int steps = 0;
    int body;
    int * history;

 public:
    const int2 get_destination() const {return destination;}
    const int get_age() const {return steps;}
    const int get_body() const {return body;}

    const void add_to_history(int router_id) {
      for (int i{HISTORY_SIZE - 2}; i >= 0; --i) {
         this->history[i + 1] = this->history[i];
      }

      this->history[0] = router_id;
    }

    const int check_loop(int router_id) {
      int count[MAX_SIZE_IDS];
      for (int i{0}; i < MAX_SIZE_IDS; ++i)
         count[i] = 0;

      for (int i{0}; i < HISTORY_SIZE; ++i) {
         ++count[history[i]];
         if (count[history[i]] > 1) {
            for (int j{i}; i >= 0; --i) {
               if (history[j] == router_id)
                  return history[j + 1];
            }
         }
      }

      return -1;
    }

    Package() = default;

    Package(int2 destination, int body): destination{destination}, body{body} {
      this->history = new int[HISTORY_SIZE];
      for (int i{0}; i < HISTORY_SIZE; ++i)
         this->history[i] = -1;
    };

    void age() {this->steps++;}
};

}

#endif