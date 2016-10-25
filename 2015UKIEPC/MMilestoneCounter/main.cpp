#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_set>
#include <algorithm>

using namespace std;

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);
    long long ms[1000];
    long long ns[1000];

    long long n_diffs[999];
    long long m_diffs[999];
    long long mindiff = 0;

    int i = 0;
    for(;i<m;i++){
        scanf("%lld", &(ms[i]));
        if(i>0) m_diffs[i-1] = ms[i] - ms[i-1];
    }
    i=0;
    for(;i<n;i++){
        scanf("%lld", &(ns[i]));
        if(i>0) n_diffs[i-1] = ns[i] - ns[i-1];
    }

    long long ans[1000];
    ans[0] = -1;
    int ansi = 0;
    unordered_set<long long> distinct;

    i=0;
    for(;i<n-1;i++){
        int m_greater = (m_diffs[0] > n_diffs[i]);
        long long numerator = m_greater ? m_diffs[0] : n_diffs[i];
        long long denominator = m_greater ? n_diffs[i] : m_diffs[0];
        for(int j = 0;j<m-1;j++){
            if(m_greater && numerator * n_diffs[i+j] != m_diffs[j] * denominator) break;
            if(!m_greater && numerator * m_diffs[j] != n_diffs[i+j] * denominator) break;
            if(j == m-2){
                long long answer = n_diffs[i];
                if(distinct.find(answer) != distinct.end()) break;
                ans[ansi] = answer;
                ansi++;
                ans[ansi] = -1;
                distinct.insert(answer);
            }
        }
    }

    if(ansi > 0) std::sort(ans, ans + ansi);
    printf("%d\n", ansi);
    i = 0;
    for(;i<ansi;i++){
        cout << ans[i] << " ";
    }
    return 0;
}
