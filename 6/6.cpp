#include <iostream>
#include <sstream>
#include <set>
#include <vector>

using s_t = std::set<char>;
using v_t = std::vector<s_t>;

int main()
{
    std::string line;
    int i=0;
    v_t v(1);

    while(std::getline(std::cin, line)) {
        if(!line.size()) {
            v.emplace_back();
            ++i;
            continue;
        }

        for(auto &c : line) {
            v[i].insert(c);
        }
    }

    i=0;
    for(auto &s : v) {
        i += s.size();
    }

    std::cout << "i is " << i << std::endl;
    return 0;
}

