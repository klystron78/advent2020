#include <iostream>
#include <sstream>
#include <set>
#include <list>

int main()
{
    std::set<int> is;
    std::string line;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        int val;

        if(istr.good()) {
            istr >> val;
            is.insert(val);
        }
    }

    if(is.empty()) return 1;

    for(auto &i : is) {
        if(i) {
            auto iter = is.upper_bound(i);
            while(iter!=is.end()) {
                auto iter2 = iter;
                while(iter2!=is.end()) {
                    if((i+*iter+*iter2)==2020) {
                        std::cout << "answer is " << (i**iter**iter2) << " (" << i << "*" << *iter << "*" << *iter2 << ")" << std::endl;
                        return 0;
                    }
                    ++iter2;
                }
                ++iter;
            }
        }
    }

    std::cout << "no answer found" << std::endl;
    return 1;
}

