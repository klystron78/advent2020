#include <iostream>
#include <sstream>
#include <vector>

namespace {
    const int64_t magic = 29221323; /* looking for this sum */
}

int main()
{
    std::string line;
    std::vector<int64_t> v;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        int64_t val;
        istr >> val;
        v.emplace_back(val);
    }

    /* look for the range */
    for(int i=0; i<v.size(); ++i) {
        int64_t sum=0;
        int64_t min=INT64_MAX;
        int64_t max=0;
        for(int j=i; j<v.size(); ++j) {
            sum += v[j];
            if(v[j]<min) min=v[j];
            if(v[j]>max) max=v[j];
            if(sum==magic) {
                std::cout << "output is " << min << ", " << max << ": " << (min+max) << std::endl;
                return 0;
            }
            else if(sum>magic) break;
        }
    }

    return 1;
}

