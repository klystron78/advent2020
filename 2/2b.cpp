#include <iostream>
#include <sstream>

int main()
{
    std::string line;
    int valid=0;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        int min, max;
        char letter;
        std::string pw;

        if(istr.good()) {
            istr >> min;
            istr.ignore(1);
            istr >> max >> letter;
            istr.ignore(2);
            istr >> pw;

            if((pw[min-1]==letter)^(pw[max-1]==letter)) ++valid;
        }
    }

    std::cout << "valid: " << valid << std::endl;
    return 0;
}

