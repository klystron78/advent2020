#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <assert.h>

namespace {
using c_t = std::pair<int, std::string>;
using v_t = std::vector<c_t>;
using m_t = std::map<std::string, v_t>;

int recurse(const m_t &m, std::string entry)
{
    auto iter = m.find(entry);

    if(iter==m.end()) return 0;

    int i=0;
    for(auto &p : iter->second) {
        i += (p.first + (p.first*recurse(m, p.second)));
    }

    return i;
}

}

int main()
{
    m_t m;
    std::string line;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);

        std::string one, two, dummy;
        int val;

        istr >> one >> two >> dummy >> dummy;
        auto p = m.emplace(one + " " + two, 0);
        assert(p.second);
        auto &v = p.first->second;

        do {
            istr >> val >> one >> two >> dummy;
            if(!val) break; /* 0 means no number here */
            v.emplace_back(val, one + " " + two);
        } while(dummy[dummy.size()-1]!='.');
    }

    std::cout << "result is : " << recurse(m, "shiny gold") << std::endl;

    return 0;
}

