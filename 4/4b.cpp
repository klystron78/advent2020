#include <iostream>
#include <sstream>

#include <vector>
#include <map>

#include <regex>

using pm_t = std::vector<std::pair<std::string, std::string>>;
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

            pv[i].emplace_back(k, v);
        }
    }

    for(auto &pm : pv) {
        const int NUM_VALIDS = 7;

        i=0;

        for(auto &iter : pm) {
            const std::regex n4("[0-9]{4}");

            auto range_check = [&i](const std::string &str, int min, int max) -> bool {
                std::istringstream istr(str);
                int x;
                istr >> x;
                return ((x>=min)&&(x<=max));
            };

            if(iter.first=="byr") {
                if(std::regex_match(iter.second, n4)) i += range_check(iter.second, 1920, 2002);
            }
            else if(iter.first=="iyr") {
                if(std::regex_match(iter.second, n4)) i += range_check(iter.second, 2010, 2020);
            }
            else if(iter.first=="eyr") {
                if(std::regex_match(iter.second, n4)) i += range_check(iter.second, 2020, 2030);
            }
            else if(iter.first=="hgt") {
                std::istringstream istr(iter.second);
                std::string units;
                int x;
                istr >> x; /* ignore */
                istr >> units;

                if(units=="in") {
                    if(std::regex_match(iter.second, std::regex("[0-9]{2}in"))) i += range_check(iter.second, 59, 76);
                }
                else if(units=="cm") {
                    if(std::regex_match(iter.second, std::regex("[0-9]{3}cm"))) i += range_check(iter.second, 150, 193);
                }
            }
            else if(iter.first=="hcl") {
                i += std::regex_match(iter.second, std::regex("#([[:xdigit:]]){6}"));
            }
            else if(iter.first=="ecl") {
                const int num_colors = 7;
                const std::string colors[num_colors] = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
                for(int j=0; j<num_colors; ++j) {
                    if(iter.second==colors[j]) {
                        ++i;
                        break;
                    }
                }
            }
            else if(iter.first=="pid") {
                i += std::regex_match(iter.second, std::regex("[0-9]{9}"));
            }
            else if(iter.first=="cid") {
                /* ignore */
            }
            else {
                std::cout << "invalid field " << iter.first << std::endl;
                break;
            }
        }

        if(i==NUM_VALIDS) ++total;
    }

    std::cout << "valid passports: " << total << std::endl;
    return 0;
}

