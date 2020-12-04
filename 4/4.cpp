#include <iostream>
#include <sstream>

#include <vector>
#include <map>

using pm_t = std::map<std::string, std::string>;
using pv_t = std::vector<pm_t>;

int main()
{
    std::string line;
    int i=0, total=0;
    pv_t pv(1);

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        char buf[256];

        if(!istr.str().size()) {
            pv.emplace_back();
            ++i;
            continue; /* blank line */
        }

        while(istr.good()) {
            istr.get(buf, sizeof(buf), ':'); /* does n-1 chars max */
            std::string k(buf);
            istr.ignore(); /* ignore the : */

            istr.get(buf, sizeof(buf), ' ');
            std::string v(buf);
            istr.ignore(); /* ignore the space or the newline */
            
            pv[i][k] = v;
        }
    }

    for(auto &pm : pv) {
        const int NUM_VALIDS = 7;
        const std::string valids[NUM_VALIDS] = { "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid" };

        i=0;
        for(auto &s : valids) {
            if(pm.find(s)!=pm.end()) ++i;
        }

        if(i==NUM_VALIDS) ++total;
    }

    std::cout << "valid passports: " << total << std::endl;
    return 0;
}

