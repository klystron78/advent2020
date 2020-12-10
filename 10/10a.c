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
    size_t size=0;

    int one=0;
    int three=0;

    int device_voltage = 0;

    while(scanf("%u\n", a+size)==1) ++size;

    int cur=0,desired=1;
    size_t num_used=0;

    qsort(a, size, sizeof(int32_t), comp);
    device_voltage = (a[size-1]+3);

    while(num_used<size) {
        if(a[num_used]==desired) {
            /* found it */
            if((desired-cur)==1) ++one;
            else if((desired-cur)==3) ++three;
            cur = desired;
            desired=cur+1;
            ++num_used;
        }
        else {
            /* didn't find it, so try next in range */
            ++desired;
            if((desired-cur)>3) {
                fprintf(stderr, "failed to find contiguous adapter\n");
                return 1;
            }
        }
    }

    printf("processed %ld out of %ld\n", num_used, size);
    printf("found %d for %d (1=%d, 3=%d), product=%d\n", cur, device_voltage, one, three, (one*(three+1))); /* device always 3, so +1 */

    return 0;
}

