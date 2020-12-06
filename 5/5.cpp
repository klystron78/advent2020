#include <iostream>
#include <sstream>

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
    uint32_t max_id=0;
    std::string line;

    while(std::getline(std::cin, line)) {
        std::string col(line.substr(7));

        line.resize(7);
        std::uint32_t val = binconv(line, 'F', 'B');
        val *= 8;
        val += binconv(col, 'L', 'R');

        if(max_id<val) max_id = val;
    }

    std::cout << "max id is " << max_id << std::endl;
    return 0;
}

