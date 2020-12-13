#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int heading=90; /* start by facing E */
static int x=0;
static int y=0;

static inline void l(int deg)
{
    heading -= deg;
    heading %= 360;
    if(heading<0) heading = (360+heading);
}

static inline void r(int deg)
{
    heading += deg;
    heading %= 360;
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
    static void (* const fns[359])(int) = { [0] = n, [90] = e, [180] = s, [270] = w };
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

    printf("at the end, x=%d, y=%d, heading=%u\n", x, y, heading);
    printf("distance is %d\n", (abs(x)+abs(y)));
    return 0;
}

