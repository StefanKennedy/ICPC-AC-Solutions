#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
  int n;
  double g;
  scanf("%d %lf\n", &n, &g);
  long D[5];
  double t[5];
  int i;
  for(i = 0; i < n; i++){
    scanf("%ld %lf\n", D+i, t+i);
    t[i] = t[i] / 180 * 3.14159265359; // MAKE THIS MORE ACCURATE!!
  }
  for(i = n-1; i >= 0; i--){
    int j;
    double u = 0;
    for(j = 0; j <= i; j++) {
      double force = g*cos(t[n-i-1+j]);
      double blarg = sqrt((u*u + 2 * force * (D[n-i-1+j])));
      u = blarg;
    }
    printf("%.5lf\n", u);
  }
  return 0;
}

/* v^2 = u^2 + 2as */
