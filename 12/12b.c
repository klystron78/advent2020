#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int x=0;
static int y=0;
static int wx=10;
static int wy=1;

static inline void l(int deg)
{
    const int old_wx = wx;
    const int old_wy = wy;

    switch(deg) {
        case 90:
            wx = -old_wy;
            wy = old_wx;
        break;
        case 180:
            wx = -old_wx;
            wy = -old_wy;
        break;
        case 270:
            wx = old_wy;
            wy = -old_wx;
        break;
    }
}

static inline void r(int deg)
{
    const int old_wx = wx;
    const int old_wy = wy;

    switch(deg) {
        case 90:
            wx = old_wy;
            wy = -old_wx;
        break;
        case 180:
            wx = -old_wx;
            wy = -old_wy;
        break;
        case 270:
            wx = -old_wy;
            wy = old_wx;
        break;
    }
}

static inline void n(int distance)
{
    wy += distance;
}

static inline void e(int distance)
{
    wx += distance;
}

static inline void w(int distance)
{
    wx -= distance;
}

static inline void s(int distance)
{
    wy -= distance;
}

static inline void f(int distance)
{
    y += (wy*distance);
    x += (wx*distance);
}

int main()
{
    int val;
    char command;

    static void (* const fns['Z'])(int) = { ['L'] = l, ['R'] = r, ['N'] = n, ['E'] = e, ['S'] = s, ['W'] = w, ['F'] = f};

    while(scanf("%c%u\n", &command, &val)==2) {
        fns[command](val);
    }

    printf("at the end, x=%d, y=%d,  wx=%d, wy=%d\n", x, y, wx, wy);
    printf("distance is %d\n", (abs(x)+abs(y)));
    return 0;
}

