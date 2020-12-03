#include <iostream>
#include <sstream>

namespace {
    size_t count_chars(std::string &str, char c)
    {
        std::string sc(&c, 1);
        size_t ret = 0;
        size_t total = 0;

        for(;;) {
            ret = str.find(sc, ret);
            if(ret==std::string::npos) break;
            ++total;
            ++ret; /* start at next char */
        }

        return total;
    }
}

int main()
{
    std::string line;
    int valid=0;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        int min, max;
        size_t count;
        char letter;
        std::string pw;

        if(istr.good()) {
            istr >> min;
            istr.ignore(1);
            istr >> max >> letter;
            istr.ignore(2);
            istr >> pw;

            count = count_chars(pw, letter);
            if((count>=min)&&(count<=max)) ++valid;
        }
    }

    std::cout << "valid: " << valid << std::endl;
    return 0;
}

