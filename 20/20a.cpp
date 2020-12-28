#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <iomanip>

#include <vector>
#include <map>
#include <array>

namespace {
    const int R_N = 0;
    const int R_E = 1;
    const int R_S = 2;
    const int R_W = 3;
    const int NUM_ROTATION = 4;

    const int F_NORMAL = 0;
    const int F_X = 1;
    const int F_Y = 2;
    const int NUM_FLIP = 3;

    const int E_TOP = 0;
    const int E_RIGHT = 1;
    const int E_BOTTOM = 2;
    const int E_LEFT = 3;
    const int NUM_EDGE = 4;

    const int M = 10;
    using raw_tile_t = bool[M][M];
    using tile_number_t = std::uint16_t;

    using tile_row_t = std::uint16_t;
    using tile_t = std::array<tile_row_t, M>;

    struct tile {
        const tile_number_t id;

        tile_t tile_data[NUM_ROTATION][NUM_FLIP];

        tile_row_t edges[8]; 
        tile(tile_number_t id):id(id) {}

        int board_r; //the rotation of this tile in the board
        int board_f; //the flip of this tile in the board
    };

    using tile_vector_t = std::vector<tile>;
    using edge_map_t = std::map<tile_number_t, int>;

    static void flip_about_x(const raw_tile_t m, raw_tile_t n)
    {
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                n[y][x] = m[y][M-x-1];
            }
        }
    }
    
    static void flip_about_y(const raw_tile_t m, raw_tile_t n)
    {
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                n[y][x] = m[M-y-1][x];
            }
        }
    }
    
    static void rot90(const raw_tile_t m, raw_tile_t n)
    {
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                n[y][x] = m[M-x-1][y];
            }
        }
    }

    void make_tile_data(const raw_tile_t rt, tile_t &t)
    {
        /* first, build the normal case */

        for(int y=0; y<M; ++y) {
            tile_row_t row=0;

            t[y] = 0;
            for(int x=0; x<M; ++x) {
                t[y] |= (rt[y][x]<<x);
            }
        }
    }

    tile_row_t make_edge_from_col(const tile_t &t, int col)
    {
        tile_row_t ret = 0;

        for(int y=0; y<M; ++y) {
            ret |= ((!!(t[y]&(1<<col)))<<y);
        }

        return ret;
    }

    void make_tile(tile &t, const raw_tile_t rt, edge_map_t &em)
    {
        static void (* const flip_fns[])(const raw_tile_t, raw_tile_t) = { NULL, flip_about_x, flip_about_y };
        /* first, do the R_N case (no rotation needed) */
        make_tile_data(rt, t.tile_data[R_N][F_NORMAL]);
        for(int f = F_X; f<NUM_FLIP; ++f) {
            raw_tile_t frt;
            flip_fns[f](rt, frt);
            make_tile_data(frt, t.tile_data[R_N][f]);
        }

#if 0
        /* handle all rotations and flips */
        {
            raw_tile_t rrt[2];
            int index=0;
            rot90(rt, rrt[index]);
            for(int r=R_E; r<NUM_ROTATION; ++r) {
                make_tile_data(rrt[index], t.tile_data[r][F_NORMAL]);
                for(int f = F_X; f<NUM_FLIP; ++f) {
                    raw_tile_t frt;
                    flip_fns[f](rrt[index], frt);
                    make_tile_data(frt, t.tile_data[r][f]);
                }
                rot90(rrt[index], rrt[index^1]); /* each rotation just rotates 90 degrees CW */
                index ^= 1;
            }
        }
#endif

        /* build edge data */
        t.edges[0] = t.tile_data[R_N][F_NORMAL][0];
        t.edges[1] = t.tile_data[R_N][F_NORMAL][M-1];
        t.edges[2] = make_edge_from_col(t.tile_data[R_N][F_NORMAL], 0);
        t.edges[3] = make_edge_from_col(t.tile_data[R_N][F_NORMAL], M-1);
        t.edges[4] = t.tile_data[R_N][F_X][0];
        t.edges[5] = t.tile_data[R_N][F_X][M-1];
        t.edges[6] = make_edge_from_col(t.tile_data[R_N][F_Y], 0);
        t.edges[7] = make_edge_from_col(t.tile_data[R_N][F_Y], M-1);

        for(int i=0; i<8; ++i) ++em[t.edges[i]];
    }

    void print_tile(const tile_t t)
    {
        static const char p[] = { '.', '#' };
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                std::cout << p[!!(t[y]&(1<<x))];
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void print_tile(const tile &t, int rot, int flip)
    {
        std::cout << "Tile " << std::setw(4) << std::setfill('0') << t.id;
        print_tile(t.tile_data[rot][flip]);
    }
}

int main()
{
    std::string line;
    tile_vector_t tv;
    edge_map_t em;

    while(std::getline(std::cin, line)) {
        std::istringstream istr(line);
        istr.ignore(5);
        tile_number_t tn;
        istr >> tn;

        int m = 0; /* should be M after reading */
        raw_tile_t rt;

        while(std::getline(std::cin, line)&&line.size()) {
            assert(line.size()==M);           
            int j=0;
            for(const auto &c : line) {
                rt[m][j++] = (c=='#');
            }
            ++m;
        }

        assert(m==M);
        auto &t = tv.emplace_back(tn);

        make_tile(t, rt, em); /* set up the tile set with all permutations of flip and rotation */
    }

    std::uint64_t prod=1;

    for(auto &t : tv) {
        int sum=0;
        for(int i=0; i<8; ++i) {
            const auto &it = em.find(t.edges[i]);
            assert(it!=em.end()); //sanity check
            if(it->second==2) ++sum;
            //sum += (em[t.edges[i]]>1);
        }

        if(sum==4) prod *= t.id;
    }

    std::cout << "prod is " << prod << std::endl;

    return 0;
}

