#include <iostream>
#include <sstream>

int main()
{
    std::string line;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
    }

    return 0;
}

