#include <iostream>
#include <sstream>
#include <map>

namespace {
    using addr_t = std::uint64_t;
    using mask_t = std::uint64_t;
    using val_t = std::uint64_t;

    using mm_t = std::map<addr_t, val_t>;
}

int main()
{
    mm_t mm;
    std::string line;

    mask_t mask[2] = {0,0};

    while(std::getline(std::cin, line)) {
        if(line[1]=='a') {
            /* mask */
            mask[0]=mask[1]=0;

            for(int i=7, j=35; i<line.size(); ++i,--j) {
                if(line[i]!='X') mask[line[i]-'0'] |= (1ULL<<j);
            }
        }
        else {
            /* assume mem */
            std::istringstream istr(line);
            istr.ignore(4);
            addr_t a;
            istr >> a;

            val_t v;
            istr.ignore(4);
            istr >> v;

            v |= mask[1];
            v &= ~mask[0];

            mm[a] = v;
        }
    }

    val_t sum = 0;
    for(const auto &m : mm) {
        sum += m.second;
    }

    std::cout << "sum is " << sum << std::endl;
    return 0;
}

