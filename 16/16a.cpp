#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>

namespace {
    using n_t = int;
    using r_t = std::pair<n_t, n_t>;
    using vr_t = std::vector<r_t>;
    using vn_t = std::vector<n_t>;
    using vnt_t = std::vector<vn_t>;

    vn_t read_ticket(const std::string &line)
    {
        vn_t ret;
        std::istringstream istr(line);
        while(istr.good()) {
            n_t n;
            istr >> n;
            istr.ignore();
            ret.emplace_back(n);
        }

        return ret;
    }
}

int main()
{
    std::string line;
    std::string dummy;

    vr_t vr;

    while(std::getline(std::cin, line)&&line.size()) {
        std::istringstream istr(line);
        do {
            istr >> dummy;
        } while(dummy.size()>0&&dummy[dummy.size()-1]!=':');

        n_t n, m;
        istr >> n;
        istr.ignore();
        istr >> m;
        vr.emplace_back(n, m);
        istr >> dummy;
        istr >> n;
        istr.ignore();
        istr >> m;
        vr.emplace_back(n, m);
    }

    //for(const auto &i : vr) std::cout << i.first << ", " << i.second << std::endl;

    vn_t your_ticket;

    std::getline(std::cin, line); /* your ticket: */
    std::getline(std::cin, line);
    your_ticket = read_ticket(line);

    //for(const auto &n : your_ticket) std::cout << n << ", ";
    //std::cout << std::endl;

    std::getline(std::cin, line); /* empty line after your ticket: */
    std::getline(std::cin, line); /* nearby tickets: */

    vnt_t nearby_tickets;
    while(std::getline(std::cin, line)) {
        nearby_tickets.emplace_back(read_ticket(line));
    }

    //for(const auto &t : nearby_tickets) {
    //    for(const auto &n : t) std::cout << n << ", ";
    //    std::cout << std::endl;
    //}

    int sum = 0;
    for(const auto &t : nearby_tickets) {
        /* for each ticket */
        for(const auto &n : t) {
            int bad=0;
            /* for each number in the ticket */
            for(const auto &r : vr) {
                if((n<r.first)||(n>r.second)) ++bad;
            }
            if(bad==vr.size()) sum += n;
        }
    }

    std::cout << "sum is " << sum << std::endl;
    return 0;
}

