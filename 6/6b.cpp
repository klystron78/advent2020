#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <tuple>

using s_t = std::set<char>;
using vs_t = std::vector<s_t>;
using ve_t = std::vector<std::tuple<s_t, vs_t>>;

int main()
{
    std::string line;
    int i=0, j=0;
    ve_t v(1);

    while(std::getline(std::cin, line)) {
        if(!line.size()) {
            v.emplace_back();
            ++i;
            j=0;
            continue;
        }

        std::get<1>(v[i]).emplace_back();
        for(auto &c : line) {
            std::get<0>(v[i]).insert(c);
            std::get<1>(v[i])[j].insert(c);
        }
        ++j;
    }

    i=0;
    for(auto &s : v) {
        for(auto &c : std::get<0>(s)) {
            /* for each character */
            j=0;
            for(auto &ss : std::get<1>(s)) {
                /* for each person in the set, which did they answer? */
                j+=ss.count(c);
            }
            if(j==std::get<1>(s).size()) ++i;
        }
    }

    std::cout << "i is " << i << std::endl;
    return 0;
}

