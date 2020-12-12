#include <iostream>
#include <vector>

/* NOTE: everythign is col-major order! */
namespace {
    typedef enum pos_t : std::uint8_t {
        FLOOR,
        EMPTY,
        OCCUPIED,
    } pos_t;

    using pv_t = std::vector<pos_t>;
    using pvs_t = std::vector<pv_t>;

    void print_map(const pvs_t &pvs)
    {
        for(const auto &pv : pvs) {
            for(const auto &s : pv) {
                if(s==OCCUPIED) std::cout << '#';
                else if(s==EMPTY) std::cout << 'L';
                else if(s==FLOOR) std::cout << ".";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    std::uint32_t count_seats(const pvs_t &pvs, pos_t check)
    {
        std::uint32_t ret = 0;
        for(const auto &pv : pvs) {
            for(const auto &t : pv) {
                ret += (t==check);
            }
        }

        return ret;
    }

    inline bool check_x(const pvs_t &in, int start_y, int start_x, int delta)
    {
        auto &row = in[start_y];
        int remaining;

        if(delta>0) remaining = row.size()-start_x-1;
        else remaining = start_x;

        while(remaining--) {
            start_x += delta;
            if(row[start_x]==EMPTY) return false;
            if(row[start_x]==OCCUPIED) return true;
        }

        return false;
    }

    inline bool check_y(const pvs_t &in, int start_y, int start_x, int delta)
    {
        int remaining;

        if(delta>0) remaining = in.size()-start_y-1;
        else remaining = start_y;

        while(remaining--) {
            start_y += delta;
            if(in[start_y][start_x]==EMPTY) return false;
            if(in[start_y][start_x]==OCCUPIED) return true;
        }

        return false;
    }

    inline bool check_diag(const pvs_t &in, int start_y, int start_x, int delta_y, int delta_x)
    {
        int check_y, check_x;

        if(delta_y>0) check_y = in.size()-1;
        else check_y = 0;

        if(delta_x>0) check_x = in[start_y].size()-1;
        else check_x = 0;

        while((start_x!=check_x)&&(start_y!=check_y)) {
            start_x += delta_x;
            start_y += delta_y;
            if(in[start_y][start_x]==EMPTY) return false;
            if(in[start_y][start_x]==OCCUPIED) return true;
        }

        return false;
    }

    inline void left_top(const pvs_t &in, pvs_t &out)
    {
        /* left top only has 3 adjacent */
        /* left top can't become empty once occupied, so if occupied we're done */
        if((in[0][0]==OCCUPIED)||(in[0][0]==FLOOR)) {
            out[0][0] = in[0][0];
            return;
        }

        out[0][0] = (!check_x(in, 0, 0, 1)&&!check_y(in, 0, 0, 1)&&!check_diag(in, 0, 0, 1, 1))?OCCUPIED:in[0][0];
    }

    inline void right_top(const pvs_t &in, pvs_t &out)
    {
        /* right top has only 3 adjacent */
        /* right top can't become empty once occupied, so if occupied we're done */
        const size_t r = in[0].size()-1;
        if((in[0][r]==OCCUPIED)||(in[0][r]==FLOOR)) {
            out[0][r] = in[0][r];
            return;
        }

        out[0][r] = (!check_x(in, 0, r, -1)&&!check_y(in, 0, r, 1)&&!check_diag(in, 0, r, 1, -1))?OCCUPIED:in[0][r];
    }

    inline void row_top(const pvs_t &in, pvs_t &out)
    {
        /* for top row, just consider 5 possibilities */
        const size_t stop = (in[0].size()-1);

        for(size_t i = 1; i<stop; ++i) {
            const auto &s = in[0][i];
            if(s==FLOOR) continue; /* nothing to do for floor */

            int total = check_x(in, 0, i, -1);
            total += check_x(in, 0, i, 1);
            total += check_y(in, 0, i, 1);
            total += check_diag(in, 0, i, 1, -1);
            total += check_diag(in, 0, i, 1, 1);

            if(s==OCCUPIED) out[0][i] = (total>=5)?EMPTY:OCCUPIED;
            else out[0][i] = !total?OCCUPIED:EMPTY;
        }
    }

    inline void left_bottom(const pvs_t &in, pvs_t &out)
    {
        /* left bottom only has 3 adjacent */
        /* left bottom can't become empty once occupied, so if occupied we're done */
        size_t b = in.size()-1;

        if((in[b][0]==OCCUPIED)||(in[b][0]==FLOOR)) {
            out[b][0] = in[b][0];
            return;
        }

        out[b][0] = (!check_x(in, b, 0, 1)&&!check_y(in, b, 0, -1)&&!check_diag(in, b, 0, -1, 1))?OCCUPIED:in[0][0];
    }

    inline void right_bottom(const pvs_t &in, pvs_t &out)
    {
        /* right bottom has only 3 adjacent */
        /* right bottom can't become empty once occupied, so if occupied we're done */
        const size_t r = in[0].size()-1;
        const size_t b = in.size()-1;
        if((in[b][r]==OCCUPIED)||(in[b][r]==FLOOR)) {
            out[b][r] = in[b][r];
            return;
        }

        out[b][r] = (!check_x(in, b, r, -1)&&!check_y(in, b, r, -1)&&!check_diag(in, b, r, -1, -1))?OCCUPIED:in[b][r];
    }

    inline void row_bottom(const pvs_t &in, pvs_t &out)
    {
        /* for bottom row, just consider 5 possibilities */
        const size_t b = in.size()-1;
        const size_t stop = (in[b].size()-1);

        for(size_t i = 1; i<stop; ++i) {
            const auto &s = in[b][i];

            if(s==FLOOR) continue; /* nothing to do for floor */

            int total = check_x(in, b, i, -1);
            total += check_x(in, b, i, 1);
            total += check_y(in, b, i, -1);
            total += check_diag(in, b, i, -1, -1);
            total += check_diag(in, b, i, -1, 1);

            if(s==OCCUPIED) out[b][i] = (total>=5)?EMPTY:OCCUPIED;
            else out[b][i] = !total?OCCUPIED:EMPTY;
        }
    }

    inline void left(const pvs_t &in, pvs_t &out, int y)
    {
        /* any left has 5 adjacent */
        if(in[y][0]==FLOOR) return;

        int total = check_x(in, y, 0, 1);
        total += check_y(in, y, 0, -1);
        total += check_y(in, y, 0, 1);
        total += check_diag(in, y, 0, -1, 1);
        total += check_diag(in, y, 0, 1, 1);

        if(in[y][0]==OCCUPIED) out[y][0] = (total>=5)?EMPTY:OCCUPIED;
        else out[y][0] = !total?OCCUPIED:EMPTY;
    }

    inline void right(const pvs_t &in, pvs_t &out, int y)
    {
        /* any right has 5 adjacent */
        size_t r = in[0].size()-1;
        if(in[y][r]==FLOOR) return; /* nothing to do for floor */

        int total = check_x(in, y, r, -1);
        total += check_y(in, y, r, -1);
        total += check_y(in, y, r, 1);
        total += check_diag(in, y, r, -1, -1);
        total += check_diag(in, y, r, 1, -1);

        if(in[y][r]==OCCUPIED) out[y][r] = (total>=5)?EMPTY:OCCUPIED;
        else out[y][r] = !total?OCCUPIED:EMPTY;
    }

    inline void row(const pvs_t &in, pvs_t &out, int y)
    {
        /* the normal case has 8 adjacents */
        const size_t stop = (in[y].size()-1);

        for(size_t i = 1; i<stop; ++i) {
            const auto &s = in[y][i];
            if(s==FLOOR) continue; /* nothing to do for floor */

            int total = check_x(in, y, i, -1);
            total += check_x(in, y, i, 1);
            total += check_y(in, y, i, -1);
            total += check_y(in, y, i, 1);
            total += check_diag(in, y, i, -1, -1);
            total += check_diag(in, y, i, -1, 1);
            total += check_diag(in, y, i, 1, -1);
            total += check_diag(in, y, i, 1, 1);

            if(s==OCCUPIED) out[y][i] = (total>=5)?EMPTY:OCCUPIED;
            else out[y][i] = !total?OCCUPIED:EMPTY;
        }
    }
}

int main()
{
    std::string line;
    pvs_t pvs[2];
    int index=0;

    while(std::getline(std::cin, line)) {
        auto &pv = pvs[0].emplace_back();
        pv.reserve(line.size());
        for(const auto &c : line) {
            /* assume input is good */
            if(c=='L') pv.emplace_back(EMPTY);
            else pv.emplace_back(FLOOR);
        }
    }

    pvs[1] = pvs[0];
    int y=0;
    std::uint64_t runs=0;

    const size_t stop = pvs[0].size()-1;
    for(;;) {
        /* determine top row, which has no seats above */
        left_top(pvs[index], pvs[index^1]);
        row_top(pvs[index], pvs[index^1]);
        right_top(pvs[index], pvs[index^1]);

        /* do all rows except bottom */
        for(y=1; y<stop; ++y) {
            left(pvs[index], pvs[index^1], y);
            row(pvs[index], pvs[index^1], y);
            right(pvs[index], pvs[index^1], y);
        }
    
        /* determine bottom row, which has no seats below */
        left_bottom(pvs[index], pvs[index^1]);
        row_bottom(pvs[index], pvs[index^1]);
        right_bottom(pvs[index], pvs[index^1]);

        ++runs;

        if(pvs[0]==pvs[1]) break; /* no changes */

        //print_map(pvs[index^1]);
        //if(runs==3) break;
        index ^= 1;
    }

    std::cout << "seats occupied after " << runs << " runs: " << count_seats(pvs[index], OCCUPIED) << std::endl;
    return 0;
}

