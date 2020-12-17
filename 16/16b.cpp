#include <iostream>
#include <sstream>
#include <vector>
#include <assert.h>
#include <string.h>
#include <algorithm>

namespace {
    using n_t = int;
    using range_t = std::pair<n_t, n_t>;
    using vn_t = std::vector<n_t>;
    using vnt_t = std::vector<vn_t>;

    typedef struct r_t {
        int depart;
        range_t ranges[2];

        r_t(int depart, n_t min_a, n_t max_a, n_t min_b, n_t max_b):depart(depart), ranges({std::make_pair(min_a, max_a),
            std::make_pair(min_b, max_b)}) {}

        bool operator==(const r_t &r) {
            return !memcmp(ranges, r.ranges, sizeof(ranges));
        }
    } r_t;

    using vr_t = std::vector<r_t>;
    using vrr_t = std::vector<vr_t>;

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

    bool valid(n_t val, const r_t &r)
    {
        return (((val>=r.ranges[0].first)&&(val<=r.ranges[0].second)) || ((val>=r.ranges[1].first)&&(val<=r.ranges[1].second)));
    }

    void cleanup(vrr_t &vrr, r_t &r)
    {
        for(auto &vr : vrr) {
            if(vr.size()>1) {
                auto it = std::find(vr.begin(), vr.end(), r);
                if(it!=vr.end()) {
                    vr.erase(it);
                    if(vr.size()==1) cleanup(vrr, vr[0]);
                }
            }
        }
    }
}

int main()
{
    std::string line;
    std::string dummy;

    vr_t vr;

    int i=0;
    while(std::getline(std::cin, line)&&line.size()) {
        std::istringstream istr(line);
        int d=0;
        do {
            istr >> dummy;
            if(dummy=="departure") d=1;
        } while(dummy.size()>0&&dummy[dummy.size()-1]!=':');

        n_t min_a, max_a, min_b, max_b;
        istr >> min_a;
        istr.ignore();
        istr >> max_a;
        istr >> dummy;
        istr >> min_b;
        istr.ignore();
        istr >> max_b;
        vr.emplace_back(d, min_a, max_a, min_b, max_b);
        ++i;
    }

    //for(const auto &i : vr) std::cout << i.first << ", " << i.second << std::endl;

    vn_t your_ticket;

    std::getline(std::cin, line); /* your ticket: */
    std::getline(std::cin, line);
    your_ticket = read_ticket(line);

    vrr_t vrr;
    vrr.resize(your_ticket.size(), vr);

    //for(const auto &n : your_ticket) std::cout << n << ", ";
    //std::cout << std::endl;

    std::getline(std::cin, line); /* empty line after your ticket: */
    std::getline(std::cin, line); /* nearby tickets: */

    vnt_t nearby_tickets_all, nearby_tickets_good; /* have possible candidates for all fields in your ticket */
    while(std::getline(std::cin, line)) {
        auto t = read_ticket(line);

        auto const invalid = std::find_if(t.begin(), t.end(), [&vr](n_t value) {
            auto const vf = std::find_if(vr.begin(), vr.end(), [value](auto const &r) {
                return valid(value, r);
            });

            return vf==vr.end();
        });
        if(invalid==t.end()) {
            /* got a valid ticket */
            for(i=0; i<t.size(); ++i) {
                if(vrr[i].size()>1) {
                    auto const invalid = std::remove_if(vrr[i].begin(), vrr[i].end(), [value=t[i]](const auto &r) {
                        return !valid(value, r);
                    });

                    vrr[i].erase(invalid, vrr[i].end());
                    if(vrr[i].size()==1) {
                        cleanup(vrr, vrr[i][0]);
                    }
                }
            }
        }
    }

    std::uint64_t prod = 1;
    for(i=0; i<vrr.size(); ++i) {
        if(vrr[i][0].depart) prod *= your_ticket[i];
    }

    std::cout << "prod is " << prod << std::endl;

    return 0;
}

