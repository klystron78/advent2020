#include <iostream>
#include <sstream>
#include <algorithm>
#include <assert.h>
#include <limits>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace {
    namespace mi = boost::multi_index;
    using dim_t = int;
    struct cube {
        dim_t w, z, y, x;
        bool active; /* an active cube is a fun cube */

        cube(dim_t w, dim_t z, dim_t y, dim_t x, bool active):w(w), z(z), y(y), x(x), active(active) {}
    };

    using space_t = boost::multi_index_container<
        cube,
        mi::indexed_by<
            mi::hashed_unique<
                mi::composite_key<
                    cube,
                    mi::member<cube, dim_t, &cube::w>,
                    mi::member<cube, dim_t, &cube::z>,
                    mi::member<cube, dim_t, &cube::y>,
                    mi::member<cube, dim_t, &cube::x>
                >
            >
        >
    >;

    int count_neighbors(const space_t &space, dim_t w, dim_t z, dim_t y, dim_t x)
    {
        int n=0;

        for(dim_t dw=(w-1); dw<=(w+1); ++dw)
            for(dim_t dz=(z-1); dz<=(z+1); ++dz)
                for(dim_t dy=(y-1); dy<=(y+1); ++dy)
                    for(dim_t dx=(x-1); dx<=(x+1); ++dx) {
                        if((dw==w)&&(dz==z)&&(dy==y)&&(dx==x)) continue; /* don't check self */
                        auto it = space.find(std::make_tuple(dw, dz, dy, dx));
                        if(it==space.end()) continue; /* doesn't exist, so can't possibly be active */
                        ++n;
                    }

        return n;
    }

    void check_and_emplace(const space_t &space, space_t &output, const cube &c)
    {
        const int n = count_neighbors(space, c.w, c.z, c.y, c.x);
        bool na=false;

        if(n==3) na=true;
        else if(c.active&&n==2) na=true;

        if(na) output.emplace(c.w, c.z, c.y, c.x, na);
    }

    void check_and_emplace(const space_t &space, space_t &output, dim_t w, dim_t z, dim_t y, dim_t x)
    {
        check_and_emplace(space, output, {w, z, y, x, false});
    }

    space_t cycle(const space_t &space)
    {
        space_t output;
        dim_t min_w = std::numeric_limits<dim_t>::max();
        dim_t max_w = std::numeric_limits<dim_t>::min();
        dim_t min_z = std::numeric_limits<dim_t>::max();
        dim_t max_z = std::numeric_limits<dim_t>::min();
        dim_t min_x = std::numeric_limits<dim_t>::max();
        dim_t max_x = std::numeric_limits<dim_t>::min();
        dim_t min_y = std::numeric_limits<dim_t>::max();
        dim_t max_y = std::numeric_limits<dim_t>::min();

        /* go around the edges, and add another set of neighbors */
        for(const auto &c : space) {
            check_and_emplace(space, output, c);

            if(c.w<min_w) min_w = c.w;
            if(c.w>max_w) max_w = c.w;
            if(c.z<min_z) min_z = c.z;
            if(c.z>max_z) max_z = c.z;
            if(c.x<min_x) min_x = c.x;
            if(c.x>max_x) max_x = c.x;
            if(c.y<min_y) min_y = c.y;
            if(c.y>max_y) max_y = c.y;
        }

        /* now do the outer bounds for new z slices */
        for(dim_t w=(min_w-1); w<=(max_w+1); ++w) {
            for(dim_t z=(min_z-1); z<=(max_z+1); ++z) {
                for(dim_t y=(min_y-1); y<=(max_y+1); ++y) {
                    for(dim_t x=(min_x-1); x<=(max_x+1); ++x) {
                        check_and_emplace(space, output, w, z, y, x);
                    }
                }
            }
        }
        return output;
    }

#if 0
    void print_z(const space_t &space, dim_t z)
    {
        auto pi = space.equal_range(std::make_tuple(z));

        assert(pi.first!=pi.second);

        auto old_y = pi.first->y;

        std::for_each(pi.first, pi.second, [&old_y](const cube &c) {
            const char symbol[2] = { '.', '#' };
            if(c.y!=old_y) {
                std::cout << std::endl;
                old_y = c.y;
            }

            std::cout << symbol[c.active];
        });

        std::cout << std::endl;
    }
#endif
}

int main()
{
    std::string line;
    space_t space;

    {
        dim_t x=0, y=0;
        while(std::getline(std::cin, line)) {
            for(const auto &c : line) {
                if(c=='#') space.emplace(0, 0, y, x, true);
                ++x;
            }
            ++y;
            x=0;
        }
    }

    //print_z(space, 0);
    //std::cout << std::endl;

    for(int i=0; i<6; ++i) space=cycle(space);

    std::cout << "total is " << space.size() << std::endl;
    return 0;
}

