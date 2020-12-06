#include <iostream>
#include <sstream>
#include <set>

namespace {
    std::uint32_t binconv(const std::string &str, char zero, char one)
    {
        std::uint32_t ret = 0;

        for(auto &c : str) {
            ret <<= 1;

            /* TODO: check for zero? meh, input seems ok */
            ret |= (c==one);
        }

        return ret;
    }
}

int main()
{
    std::set<std::uint32_t> s;
    std::string line;

    while(std::getline(std::cin, line)) {
        std::string col(line.substr(7));

        line.resize(7);
        std::uint32_t val = binconv(line, 'F', 'B');
        val *= 8;
        val += binconv(col, 'L', 'R');

        s.insert(val);
    }

    std::uint32_t old = 0;
    for(auto &i : s) {
        if((i-old)==2) {
            std::cout << "id is: " << (i-1) << std::endl;
            return 0;
        }

        old = i;
    }

    std::cout << "id not found" << std::endl;
    return 0;
}

