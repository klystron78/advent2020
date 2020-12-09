#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int comp(const void *a, const void *b)
{
    if((*(int64_t*)a)>(*(int64_t*)b)) return 1;
    if((*(int64_t*)a)<(*(int64_t*)b)) return -1;
    if((*(int64_t*)a)==(*(int64_t*)b)) return 0;
}

/* return 0 if it exists */
static int bsearch_sum(const int64_t a[25], int64_t val)
{
    int64_t s[25];

    memcpy(s, a, sizeof(s));
    qsort(s, 25, sizeof(int64_t), comp);

    for(int i=0; i<25; ++i) {
        int64_t expected = (val-s[i]);

        if(expected>0) {
            if(bsearch(&expected, s, 25, sizeof(int64_t), comp)) return 0; /* found it */
        }
    }

    return 1;
}

int main()
{
    int64_t val;
    int64_t q[25];
    int i=0;

    while(scanf("%lu\n", &val)==1) {
        q[i] = val;
        if(++i==25) break;
    }

    while(scanf("%lu\n", &val)==1) {
        /* first, see if sum exists in array */
        if((bsearch_sum(q, val))) {
            /* didn't find it! */
            printf("val is %lu\n", val); 
            return 0;
        }

        /* wasn't in there, so recalc */
        memmove(q, q+1, sizeof(int64_t)*24);
        q[24] = val;
    }

    return 1;
}

