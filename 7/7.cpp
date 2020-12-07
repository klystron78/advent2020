#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <assert.h>

namespace {
using c_t = std::pair<int, std::string>;
using v_t = std::vector<c_t>;
using m_t = std::map<std::string, v_t>;

bool recursive_find(const m_t &m, const std::string entry)
{
    bool ret = false;
    auto iter = m.find(entry);

    if(iter==m.end()) return false;

    for(const auto &p : iter->second) {
        if(p.second=="shiny gold") return true;

        ret |= recursive_find(m, p.second);
    }

    return ret;
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

    int i=0;
    for(auto &p : m) {
        i += recursive_find(m, p.first);
    }

    std::cout << "i is " << i << std::endl;
    return 0;
}

