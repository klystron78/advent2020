#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

static int comp(const void *a, const void *b)
{
    if((*(int32_t*)a)>(*(int32_t*)b)) return 1;
    if((*(int32_t*)a)<(*(int32_t*)b)) return -1;
    if((*(int32_t*)a)==(*(int32_t*)b)) return 0;
}

int main()
{
    int32_t a[256]; /* more than i need heh */
    uint64_t counts[256];

    size_t size=0;

    while(scanf("%u\n", a+size)==1) ++size;

    qsort(a, size, sizeof(int32_t), comp);
    a[size] = (a[size-1]+3);
    ++size;

    memset(counts, 0, sizeof(counts));

    /* prime the first 3 */
    if(a[0]<=3) ++counts[0];
    if(a[1]<=3) ++counts[1];
    if(a[2]<=3) ++counts[2];

    if((a[1]-a[0])<=3) counts[1]+=counts[0];
    
    if((a[2]-a[1])<=3) counts[2]+=counts[1];
    if((a[2]-a[0])<=3) counts[2]+=counts[0];

    for(size_t i=3; i<size; ++i) {
        if(a[i]-a[i-1]<=3) counts[i]+=counts[i-1];
        if(a[i]-a[i-2]<=3) counts[i]+=counts[i-2];
        if(a[i]-a[i-3]<=3) counts[i]+=counts[i-3];
    }

    printf("counter is %ld\n", counts[size-1]);

    return 0;
}

