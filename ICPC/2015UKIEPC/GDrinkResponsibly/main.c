#include <stdio.h>
#include <stdlib.h>

const size_t M = 1001;
const size_t U = 201;
const size_t D = 9;

const size_t accuracy = 100;

typedef struct {
    int u;
    int ds[9];
} comb;

int main()
{

    char za[3];
    int deviation = 50;

    float mt, ut;
    unsigned int m, u, d;
    int moneys[8], units[8];
    char names[8][20];

    scanf("%f %f %d\n", &mt, &ut, &d);
    if(mt > 10 || ut > 20 || d > 8){
        printf("Invalid\n");
        return 0;
    }
    m = (int)(mt*100 + 0.1);
    u = (int)(ut*10*accuracy + 0.1);
    int i = 0;
    for(; i < d; i++){
        int percentage;
        float price;
        scanf("%s %d %s %f", names[i], &percentage, za, &price);
        moneys[i] = (int)(price*100 + 0.1);
        units[i] = percentage*10*accuracy;
        if(strcmp(za,"1/2")==0){
            units[i]/=2;
        }else if(strcmp(za,"1/3")==0){
            units[i]/=3;
        }
    }
    size_t memory = accuracy * sizeof(comb) *  M*U;
    comb * dp = malloc(memory);
    int a = 0, b = 0, c = 0;
    for(a = 0;a <= m; a++){
        for(c = 0;c <= u; c++){
            dp[(U*accuracy*a) + c].u = -1;
        }
    }
    dp[0].u = 1; /* no money and no units is possible*/
    for(b = 0;b < d; b++){
        for(a = 0;a < m || (m == 0 && a ==0); a++){
            for(c = 0;c <= u || (c == 0 && u == 0); c++){
                comb p = dp[(U*accuracy*a) + c];
                if(p.u < 0) continue;
                if(a+moneys[b] <= m && c + units[b] <= u + deviation){
                    int q = 0;
                    for(;q<D;q++){
                        dp[(U*accuracy*(a+moneys[b])) + c + units[b]].ds[q] = p.ds[q];
                    }
                    dp[(U*accuracy*(a+moneys[b])) + c + units[b]].u = 1;
                    dp[(U*accuracy*(a+moneys[b])) + c + units[b]].ds[b]++;
                }
            }
        }
    }

    int one = m, three = u;
    i = -1 * deviation;
    int found = 0;
    for(; i <= deviation && three - i >= 0; i++){
        comb p = dp[(U*accuracy*one) + three-i];
        if(p.u>0){
            int o = 0;
            for(;o<D;o++){
                if(p.ds[o] > 0) printf("%s %d\n", names[o], p.ds[o]);
            }
            found = 1;
            break;
        }else if(i == deviation && !found){
            printf("IMPOSSIBLE\n");
        }
    }
    free(dp);
    return 0;
}
