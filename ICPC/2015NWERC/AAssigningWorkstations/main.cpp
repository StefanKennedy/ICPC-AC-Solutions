#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

typedef struct {
    long long a;
    long long s;
} ws;

int cmp(const void* a, const void* b){
    long long diff = ((ws*)a)->a - ((ws*)b)->a;
    if(diff > 0) return 1;
    if(diff < 0) return -1;
    return 0;
}

int main()
{
    long n;
    long long m;
    scanf("%ld %lld", &n, &m);

    ws * r = new ws[300000];

    int i;
    for(i = 0; i< n; i++){
        scanf("%lld %lld", &((r+i)->a), &((r+i)->s));
    }

    qsort(r, n, sizeof(ws), cmp);

    priority_queue<long long, std::vector<long long>, std::greater<long long>> p;

    long blarg = 0;
    for(i = 0; i < n; i++){
        long long end = (r[i].a + r[i].s)+m;
        if(p.empty()){
            p.push(end);
            continue;
        }
        long long top = p.top();
        while(top < r[i].a && !p.empty()){
            p.pop();
            top = p.top();
        }
        if(!p.empty() && top >= r[i].a && r[i].a >= top - m){
            p.pop();
            p.push(end);
            blarg++;
            continue;
        }
        p.push(end);
    }

    printf("%ld\n", blarg);

    return 0;
}

