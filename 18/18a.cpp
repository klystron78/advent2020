#include <iostream>
#include <sstream>
#include <array>

namespace {
    using n_t = std::uint64_t;

    n_t calc(const char **str)
    {
        n_t acc=0;
        enum {
            NONE,
            PLUS,
            MULT
        } op=NONE;
        while(**str!=0) {
            if(**str=='(') {
                /* starting new group */
                ++*str;
                switch(op) {
                    case NONE:
                        acc = calc(str);
                    break;
                    case PLUS:
                        op = NONE;
                        acc += calc(str);
                    break;
                    case MULT:
                        op = NONE;
                        acc *= calc(str);
                    break;
                }
            }
            else if(**str==')') {
                /* end of group */
                ++*str;
                return acc;
            }
            else if(**str=='+') {
                op = PLUS;
                *str += 2;
            }
            else if(**str=='*') {
                op = MULT;
                *str += 2;
            }
            else if(**str==' ') {
                ++*str;
            }
            else {
                /* must be a number */
                std::uint8_t v = (**str-'0');
                switch(op) {
                    case NONE:
                        acc = v;
                    break;
                    case PLUS:
                        op = NONE;
                        acc += v;
                    break;
                    case MULT:
                        op = NONE;
                        acc *= v;
                    break;
                }
                ++*str;
            }
        }

        return acc;
    }
}

int main()
{
    char buf[256]; /* big enough */
    n_t sum=0;

    while(std::cin.getline(buf, sizeof(buf))) {
        const char *b = buf;
        sum += calc(&b);
    }

    std::cout << "sum of all values " << sum << std::endl;
    return 0;
}

