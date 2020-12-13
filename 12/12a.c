#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int x=0;
static int y=0;

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

static int heading=EAST; /* start by facing E */
static inline void l(int deg)
{
    static const int tr[4][4] = {{NORTH, WEST, SOUTH, EAST}, {EAST, NORTH, WEST, SOUTH}, {SOUTH, EAST, NORTH, WEST}, {WEST, SOUTH, EAST, NORTH}};
    heading = tr[heading][deg/90];
}

static inline void r(int deg)
{
    static const int tr[4][4] = {{NORTH, EAST, SOUTH, WEST}, {EAST, SOUTH, WEST, NORTH}, {SOUTH, WEST, NORTH, EAST}, {WEST, NORTH, EAST, SOUTH}};
    heading = tr[heading][deg/90];
}

static inline void n(int distance)
{
    y += distance;
}

static inline void e(int distance)
{
    x += distance;
}

static inline void w(int distance)
{
    x -= distance;
}

static inline void s(int distance)
{
    y -= distance;
}

static inline void f(int distance)
{
    static void (* const fns[4])(int) = { n, e, s, w };
    fns[heading](distance);
}

int main()
{
    int val;
    char command;

    static void (* const fns['Z'])(int) = { ['L'] = l, ['R'] = r, ['N'] = n, ['E'] = e, ['S'] = s, ['W'] = w, ['F'] = f};

    while(scanf("%c%u\n", &command, &val)==2) {
        fns[command](val);
    }

    printf("at the end, x=%d, y=%d, heading=%d\n", x, y, heading);
    printf("distance is %d\n", (abs(x)+abs(y)));
    return 0;
}

