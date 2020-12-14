#include <iostream>
#include <sstream>
#include <map>
#include <iomanip>

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

    mask_t ones, xs;

    int num_xs;
    while(std::getline(std::cin, line)) {
        if(line[1]=='a') {
            /* mask */
            ones=xs=0;
            num_xs = 0;

            for(int i=7, j=35; i<line.size(); ++i,--j) {
                if(line[i]=='1') ones |= (1ULL<<j);
                else if(line[i]=='X') {
                    xs |= (1ULL<<j);
                    ++num_xs;
                }
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

            a |= ones;
            a &= ~xs; /* start with all X bits set to 0 */
            /* write all possible addresses with v */

            const mask_t stop = (1ULL<<num_xs);
            const mask_t bits = (stop)-1;

            for(mask_t cur_bits=0; cur_bits<stop; ++cur_bits) {
                addr_t at = a;
                /* modify the address by setting any set bits in cur_bits at the bit positions
                 * in-order in xs */

                int shift=0;
                /* find the i'th set bit in mask */
                for(int j=0; j<36; ++j) {
                    if(xs&(1ULL<<j)) {
                        /* found a bit in xs, so check if it's a set bit in i */
                        if(cur_bits&(1ULL<<shift)) {
                            at |= (1ULL<<j);
                        }
                        ++shift;
                    }
                }

                mm[at] = v;
            }
        }
    }

    val_t sum = 0;
    for(const auto &m : mm) {
        sum += m.second;
    }

    std::cout << "sum is " << sum << std::endl;
    return 0;
}

