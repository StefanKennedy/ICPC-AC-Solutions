#include <stdio.h>
#include <stdlib.h>

int main()
{
    long long n, r, p;
    scanf("%lld %lld %lld", &n, &r, &p);

    if(n == 1){
        printf("0");
        return 0;
    }

    long long int* dp = (long long int*)malloc(sizeof(long long int) * 1000000);
    dp[1] = 0;

    long gap = 1;
    long long m = 1;
    for(;m<=n;m++){
        dp[m] = (p*(m-1)) + r;

        long used = 1;
        long sections;
        gap = 1;
        for(;gap<=(m+1)/2;gap++){
            //printf("Gap size: %ld\n", gap);
            if(dp[(m+gap)/(gap+1)] == 0) printf("Accessing %ld was 0\n", (m+gap)/(gap+1));
            long long int suggest = dp[(m+gap)/(gap+1)] + (p*gap) + r;
            if(suggest < dp[m]){ used = gap; }
            dp[m] = (suggest < dp[m] ? suggest : dp[m]);

            sections = ((n+gap-1) / gap);
            --sections;
            if(sections == 0) continue;
            long new_gap = ((n+sections-1) / (sections));
            //printf("Suggesting %ld instead of %ld, when inserting %ld\n", new_gap, gap, sections);
            if(new_gap > gap) gap = new_gap-1; /* 1 will be added to gap on next iteration */
        }
        //printf("at %lld, the optimum is %lld\n", m, dp[m]);

        sections = ((n+m-1) / m);
        --sections;
        if(sections == 0) continue;
        long new_m = ((n+sections-1) / (sections));
        //printf("Suggesting %ld instead of %ld, when inserting %ld\n", new_gap, gap, sections);
        if(new_m > m) m = new_m-1; /* 1 will be added to m on next iteration */
    }

    printf("%lld", dp[n]);

    free(dp);
    return 0;
}
