#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>
#include <iomanip>
#include <algorithm>

#include <vector>
#include <set>
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
    const char pch[] = { '.', '#' };

    using raw_tile_t = bool[M][M];
    using tile_number_t = std::uint16_t;

    using tile_row_t = std::uint16_t;
    using tile_t = std::array<tile_row_t, M>;

    struct tile {
        const tile_number_t id;

        tile_t tile_data[NUM_ROTATION][NUM_FLIP];

        tile_row_t edge_data[NUM_ROTATION][NUM_FLIP][NUM_EDGE];
        tile(tile_number_t id):id(id), board_r(-1), board_f(-1) {}

        int board_r; //the rotation of this tile in the board
        int board_f; //the flip of this tile in the board
    };

    using tile_vector_t = std::vector<tile>;
    using tile_p_vector_t = std::vector<tile*>;
    using tile_p_set_t = std::set<tile*>;
    using edge_map_t = std::map<tile_number_t, int>;

    static void flip_about_y(const raw_tile_t m, raw_tile_t n)
    {
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                n[y][x] = m[y][M-x-1];
            }
        }
    }
    
    static void flip_about_x(const raw_tile_t m, raw_tile_t n)
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

        /* build edge data */
        for(int r=R_N; r<NUM_ROTATION; ++r) {
            for(int f=F_NORMAL; f<NUM_FLIP; ++f) {
                t.edge_data[r][f][E_TOP] = t.tile_data[r][f][0]; /* TOP is always first */
                t.edge_data[r][f][E_BOTTOM] = t.tile_data[r][f][M-1]; /* BOTTOM is always last */
                t.edge_data[r][f][E_LEFT] = make_edge_from_col(t.tile_data[r][f], 0);
                t.edge_data[r][f][E_RIGHT] = make_edge_from_col(t.tile_data[r][f], M-1);
            }
        }

        /* map unique edge counts */
        ++em[t.edge_data[R_N][F_NORMAL][E_TOP]];
        ++em[t.edge_data[R_N][F_NORMAL][E_BOTTOM]];
        ++em[t.edge_data[R_N][F_NORMAL][E_LEFT]];
        ++em[t.edge_data[R_N][F_NORMAL][E_RIGHT]];
        ++em[t.edge_data[R_N][F_X][E_LEFT]];
        ++em[t.edge_data[R_N][F_X][E_RIGHT]];
        ++em[t.edge_data[R_N][F_Y][E_TOP]];
        ++em[t.edge_data[R_N][F_Y][E_BOTTOM]];
    }

    inline const tile *get_tile_on_board(const tile * const *board, int board_size, int y, int x)
    {
        return *(board+(y*board_size)+x);
    }

    inline void set_tile_on_board(const tile **board, int board_size, int y, int x, const tile *t)
    {
        *(board+(y*board_size)+x) = t;
    }

    void print_tile(const tile_t t)
    {
        for(int y=0; y<M; ++y) {
            for(int x=0; x<M; ++x) {
                std::cout << pch[!!(t[y]&(1<<x))];
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void print_tile(const tile &t, int rot, int flip)
    {
        std::cout << "Tile " << std::setw(4) << std::setfill('0') << t.id << std::endl;
        print_tile(t.tile_data[rot][flip]);
    }

    void print_board_numbers(const tile * const *board, int board_size)
    {
        for(int y=0; y<board_size; ++y) {
            for(int x=0; x<board_size; ++x) {
                auto *t = get_tile_on_board(board, board_size, y, x);
                if(t) std::cout << get_tile_on_board(board, board_size, y, x)->id << '\t';
                else std::cout << "NULL" << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void print_board(const tile * const *board, int board_size)
    {
        for(int i=0; i<board_size; ++i) {
            for(int y=0; y<M; ++y) {
                for(int j=0; j<board_size; ++j) {
                    auto p = get_tile_on_board(board, board_size, i, j);
                    for(int x=0; x<M; ++x) {
                        if(p) std::cout << pch[!!(p->tile_data[p->board_r][p->board_f][y]&(1<<x))];
                        else std::cout << 'N';
                    }
                    std::cout << ' ';
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    bool place_edges(const tile **board, int board_size, tile_p_set_t &edges)
    {
        auto fn_tb = [&board, board_size, &edges](int y_offset) -> bool {
            int found=0;
            int r, f;
            for(int x=1 /* always start off corner */; x<(board_size-1) /* stop off corner */; ++x) {
                auto *pl = get_tile_on_board(board, board_size, y_offset, x-1);
                auto *pr = get_tile_on_board(board, board_size, y_offset, x+1);
                tile *found_t=nullptr;

                assert(pl);
                for(auto &t : edges) {
                    for(r=R_N; r<NUM_ROTATION; ++r) {
                        for(f=F_NORMAL; f<NUM_FLIP; ++f) {
                            if((x==(board_size-2))&&(t->edge_data[r][f][E_RIGHT] != pr->edge_data[pr->board_r][pr->board_f][E_LEFT])) continue;
                            if(t->edge_data[r][f][E_LEFT] == pl->edge_data[pl->board_r][pl->board_f][E_RIGHT]) {
                                found_t = t;
                                goto bail;
                            }
                        }
                    }
                }

bail:
                if(!found_t) return false;
                found_t->board_r = r;
                found_t->board_f = f;
                set_tile_on_board(board, board_size, y_offset, x, found_t);
                edges.erase(found_t);
                ++found;
            }

            return (found==board_size-2);
        };

        auto fn_lr = [&board, board_size, &edges](int x_offset) -> bool {
            int found=0;
            int r, f;
            for(int y=1 /* always start off corner */; y<(board_size-1) /* stop off corner */; ++y) {
                auto *pt = get_tile_on_board(board, board_size, y-1, x_offset);
                auto *pb = get_tile_on_board(board, board_size, y+1, x_offset);
                tile *found_t=nullptr;

                assert(pt);
                for(auto &t : edges) {
                    for(r=R_N; r<NUM_ROTATION; ++r) {
                        for(f=F_NORMAL; f<NUM_FLIP; ++f) {
                            if((y==(board_size-2))&&(t->edge_data[r][f][E_BOTTOM] != pb->edge_data[pb->board_r][pb->board_f][E_TOP])) continue;
                            if(t->edge_data[r][f][E_TOP] == pt->edge_data[pt->board_r][pt->board_f][E_BOTTOM]) {
                                found_t = t;
                                goto bail;
                            }
                        }
                    }
                }

bail:
                if(!found_t) return false;
                found_t->board_r = r;
                found_t->board_f = f;
                set_tile_on_board(board, board_size, y, x_offset, found_t);
                edges.erase(found_t);
                ++found;
            }

            return (found==board_size-2);
        };

        return (fn_tb(0)&&fn_tb(board_size-1)&&fn_lr(0)&&fn_lr(board_size-1));
    }

    bool place_corners(const tile **board, const edge_map_t &em, int board_size, tile_p_set_t &corners, const tile_p_set_t &edges)
    {
        static const int desired_face[4][2] = {{E_RIGHT, E_BOTTOM}, {E_LEFT, E_BOTTOM}, {E_LEFT, E_TOP}, {E_RIGHT, E_TOP}};
        const int coords[4][2] = {{0, 0}, {0, board_size-1}, {board_size-1, board_size-1}, {board_size-1, 0}};
        const int index = (4-corners.size());

        for(auto &t : corners) {
            auto nh = corners.extract(t);
            for(int r=R_N; r<NUM_ROTATION; ++r) {
                for(int f=F_NORMAL; f<NUM_FLIP; ++f) {
                    int j;
                    for(j=0; j<2; ++j) {
                        auto it = em.find(t->edge_data[r][f][desired_face[index][j]]);
                        assert(it!=em.end());
                        if(it->second!=2) break;
                    }

                    if(j<2) continue; /* didn't find it */
                    t->board_r = r;
                    t->board_f = f;
                    set_tile_on_board(board, board_size, coords[index][0], coords[index][1], t);
                    if(!corners.size()) {
                        /* all corners placed, so try and place edges */
                        tile_p_set_t edges_copy(edges);
                        if(place_edges(board, board_size, edges_copy)) return true;
                    }
                    else if(place_corners(board, em, board_size, corners, edges)) return true;
                }
            }
            corners.insert(std::move(nh));
        }

        return false;
    }

    bool place_interiors(const tile **board, int board_size, tile_p_vector_t &interiors)
    {
        int x, y, r, f;

        for(y=1; y<(board_size-1); ++y) {
            for(x=1; x<(board_size-1); ++x) {
                auto *pt = get_tile_on_board(board, board_size, y-1, x);
                auto *pl = get_tile_on_board(board, board_size, y, x-1);
                auto *pr = get_tile_on_board(board, board_size, y, x+1);
                auto *pb = get_tile_on_board(board, board_size, y+1, x);
                tile *found_t=nullptr;

                for(auto &t : interiors) {
                    for(r=R_N; r<NUM_ROTATION; ++r) {
                        for(f=F_NORMAL; f<NUM_FLIP; ++f) {
                            if((x==(board_size-2))&&(t->edge_data[r][f][E_RIGHT] != pr->edge_data[pr->board_r][pr->board_f][E_LEFT])) continue;
                            if((y==(board_size-2))&&(t->edge_data[r][f][E_BOTTOM] != pb->edge_data[pb->board_r][pb->board_f][E_TOP])) continue;
                            if(t->edge_data[r][f][E_LEFT] != pl->edge_data[pl->board_r][pl->board_f][E_RIGHT]) continue;
                            if(t->edge_data[r][f][E_TOP] == pt->edge_data[pt->board_r][pt->board_f][E_BOTTOM]) {
                                found_t = t;
                                goto bail;
                            }
                        }
                    }
                }

bail:
                if(!found_t) return false;
                found_t->board_r = r;
                found_t->board_f = f;
                set_tile_on_board(board, board_size, y, x, found_t);
                interiors.erase(std::remove_if(interiors.begin(), interiors.end(), [found_t](const tile *t) { return found_t==t; }), interiors.end());
            }
        }

        return true;
    }

    void print_char_board(const char *board, int board_size)
    {
        const int size = (board_size*(M-2));
        for(int y=0; y<size; ++y) {
            for(int x=0; x<size; ++x) {
                std::cout << board[(y*size)+x];
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void char_rot90(const char *in, char *out, int board_size)
    {
        const int size = (board_size*(M-2));
        for(int y=0; y<size; ++y) {
            for(int x=0; x<size; ++x) {
                out[(y*size)+x] = in[((size-x-1)*size)+y];
            }
        }
    }

    void char_flip_about_x(const char *in, char *out, int board_size)
    {
        const int size = (board_size*(M-2));
        for(int y=0; y<size; ++y) {
            for(int x=0; x<size; ++x) {
                out[(y*size)+x] = in[((size-y-1)*size)+x];
            }
        }
    }
    
    void char_flip_about_y(const char *in, char *out, int board_size)
    {
        const int size = (board_size*(M-2));
        for(int y=0; y<size; ++y) {
            for(int x=0; x<size; ++x) {
                out[(y*size)+x] = in[(y*size)+(size-x-1)];
            }
        }
    }

    std::pair<int, int> char_find_monsters(char *board, int board_size)
    {
        const int size = (board_size*(M-2));
        const int y_size = size-2;
        const int x_size = size-19;

        int num_monsters=0, num_pound=0;

#if 0
        static const bool search[3][20] = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
            {1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1 },
            {0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
        };
#endif

        auto b = [&board, size](int y, int x) -> char* {
            return (board+(y*size)+x);
        };

        auto p = [b](int y, int x) -> bool {
            return (*b(y, x)=='#');
        };

        auto s = [b](int y, int x) {
            *b(y, x) = '@';
        };

        for(int y=0; y<y_size; ++y) {
            for(int x=0; x<x_size; ++x) {
                /* search for a pattern from this point that matches our desired search */
                if(p(y, x+18)&&p(y+1, x)&&p(y+1, x+5)&&p(y+1, x+6)&&p(y+1, x+11)&&p(y+1, x+12)&&
                        p(y+1, x+17)&&p(y+1, x+18)&&p(y+1, x+19)&&p(y+2, x+1)&&p(y+2, x+4)&&
                        p(y+2, x+7)&&p(y+2, x+10)&&p(y+2, x+13)&&p(y+2, x+16)) {
                    /* found one! */
                    ++num_monsters;
                    s(y, x+18);
                    s(y+1, x);
                    s(y+1, x+5);
                    s(y+1, x+6);
                    s(y+1, x+11);
                    s(y+1, x+12);
                    s(y+1, x+17);
                    s(y+1, x+18);
                    s(y+1, x+19);
                    s(y+2, x+1);
                    s(y+2, x+4);
                    s(y+2, x+7);
                    s(y+2, x+10);
                    s(y+2, x+13);
                    s(y+2, x+16);
                }
            }
        }

        if(num_monsters) {
            for(int yy=0; yy<size; ++yy) {
                for(int xx=0; xx<size; ++xx) {
                    num_pound += p(yy, xx);
                }
            }
        }
        return std::make_pair(num_monsters, num_pound);
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

    const double sq = sqrt(tv.size());
    assert(floor(sq)==sq);
    const int board_size = static_cast<int>(sq);
    const int num_edges = ((board_size*4)-8); /* subtract out the 4 corenrs, which are counted twice */
    const int num_interiors = ((board_size*board_size)-num_edges-4);

    tile_p_set_t corners, edges;

    tile_p_vector_t interiors;
    interiors.reserve(num_interiors);

    for(auto &t : tv) {
        int sum=0;
        for(int i=0; i<8; ++i) {
            static const int corners[8][2] = { {F_NORMAL, E_TOP}, {F_NORMAL, E_BOTTOM}, {F_NORMAL, E_LEFT}, {F_NORMAL, E_RIGHT}, {F_X, E_LEFT}, {F_X, E_RIGHT}, {F_Y, E_TOP}, {F_Y, E_BOTTOM} };

            const auto &it = em.find(t.edge_data[R_N][corners[i][0]][corners[i][1]]);
            assert(it!=em.end()); //sanity check
            if(it->second==2) ++sum;
        }

        if(sum==4) {
            /* found corner tile */
            assert(corners.size()<4);
            corners.emplace(&t);
        }
        else if(sum==6) {
            /* found edge tile */
            assert(edges.size()<num_edges);
            edges.emplace(&t);
        }
        else if(sum==8) {
            assert(interiors.size()<num_interiors);
            interiors.emplace_back(&t);
        }
    }

    assert(corners.size()==4);
    assert(edges.size()==num_edges);
    assert(interiors.size()==num_interiors);

    const tile *board[board_size*board_size] = {nullptr}; /* hold pointers to tiles on the board */

    int ret = place_corners(board, em, board_size, corners, edges); /* places corners and edges */
    assert(ret);

    ret = place_interiors(board, board_size, interiors);
    assert(ret);

    print_board_numbers(board, board_size);
    print_board(board, board_size);

    char blah[2][board_size*(M-2)][board_size*(M-2)];
    int index=0;
    /* now that all that mess is done, build an array of characters to sift through */
    for(int y=0; y<board_size; ++y) {
        for(int x=0; x<board_size; ++x) {
            auto *t = get_tile_on_board(board, board_size, y, x);
            for(int y_=1; y_<(M-1); ++y_) {
                for(int x_=1; x_<(M-1); ++x_) {
                    //std::cout << ((y*(M-2))+(y_-1)) << ' ' << ((x*(M-2))+(x_-1)) << std::endl;
                    blah[index][(y*(M-2))+(y_-1)][(x*(M-2))+(x_-1)] = pch[!!(t->tile_data[t->board_r][t->board_f][y_]&(1<<x_))];
                }
            }
        }
    }

    //print_char_board(reinterpret_cast<const char*>(blah[0]), board_size);

    std::pair<int, int> fret;

    static void (* const flip_fns[])(const char *, char*, int) = { NULL, char_flip_about_x, char_flip_about_y };
    for(int r=R_N; r<NUM_ROTATION; ++r) {
        //print_char_board(reinterpret_cast<const char*>(blah[index]), board_size);
        fret = char_find_monsters(reinterpret_cast<char*>(blah[index]), board_size);
        if(fret.first) goto done;

        for(int f=F_X; f<NUM_FLIP; ++f) {
            char work[board_size*(M-2)][board_size*(M-2)];
            flip_fns[f](reinterpret_cast<const char*>(blah[index]), reinterpret_cast<char*>(work), board_size);
            //print_char_board(reinterpret_cast<const char*>(work), board_size);
            fret = char_find_monsters(reinterpret_cast<char*>(work), board_size);
            if(fret.first) goto done;
            /* search in work */
        }

        char_rot90(reinterpret_cast<const char*>(blah[index]), reinterpret_cast<char*>(blah[index^1]), board_size);
        index ^= 1;
    }

done:
    if(!fret.first) std::cout << "nope" << std::endl;
    else {
        std::cout << fret.first << " sea monsters found" << std::endl;
        std::cout << fret.second << " water roughness" << std::endl;
    }

    return 0;
}

