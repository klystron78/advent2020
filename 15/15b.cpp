#include <iostream>
#include <sstream>
#include <array>
#include <string.h>
#include <algorithm>

namespace {
    using num_t = int;
    typedef struct t_t {
        int gone;
        int not_forgotten;
    } t_t;

    const int iterations = 30000000;
    using a_t = std::array<t_t, iterations>;

    a_t v = {0,0};

    /* find how many turns since number was last spoken, or 0 if never */
    inline int find_last(const t_t &t)
    {
        if(!t.not_forgotten) return 0;
        return t.gone - t.not_forgotten;
    }

    inline void update(t_t &t, int turn)
    {
        t.not_forgotten = t.gone;
        t.gone = turn;
    }
}

int main()
{
    std::string line;
    num_t val;
    int turn_number=0;

    std::getline(std::cin, line);
    std::istringstream istr(line);

    while(istr.good()) {
        istr >> val;
        istr.ignore();
        update(v[val], ++turn_number);
    }

    while(turn_number<iterations) {
        val = find_last(v[val]);
        update(v[val], ++turn_number);
    }

    std::cout << val << std::endl;
    return 0;
}

