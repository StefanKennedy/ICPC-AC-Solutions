#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  int x;
  int y;
} pt;

int cmp(const void* a, const void* b){
  pt pt_a = *(pt*)a;
  pt pt_b = *(pt*)b;
  if(pt_a.x == pt_b.x){
    return pt_a.y - pt_b.y;
  }
  return pt_a.x - pt_b.x;
}

long cross(pt o, pt a, pt b){
  return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

int main() {
  /*int q;
  for(q = -32; q < 32; q++){
    printf("%d %d\n", q*q, -1*(q*q));
  }
  return;*/
  long n;
  scanf("%d\n", &n);
  pt pts[100000];

  long i;
  for(i = 0; i < n; i++){
    scanf("%d %d\n", &(pts[i].x), &(pts[i].y));
  }

  qsort(pts, n, sizeof(pt), cmp);

  long upper[100000];
  long upper_position = 0;

  for(i = 0; i < n; i++){
    while(upper_position >= 2 && cross(
        pts[upper[upper_position-2]],
        pts[upper[upper_position-1]],
        pts[i]) <= 0){
      --upper_position;
    }
    upper[upper_position] = i;
    upper_position++;
  }

  upper_position--;

  long lower[100000];
  long lower_position = 0;

  for(i = n-1; i >= 0; i--){
    while(lower_position >= 2 && cross(
        pts[lower[lower_position-2]],
        pts[lower[lower_position-1]],
        pts[i]) <= 0){
      --lower_position;
    }
    lower[lower_position] = i;
    lower_position++;
  }

  lower_position--;

  long interest[100000];
  long index;
  for(index = 0, i=0; i < upper_position; i++, index++){
    interest[index] = upper[i];
  }
  for(i=0; i < lower_position; i++, index++){
    interest[index] = lower[i];
  }

  int j;
  double max = 0;
  for(i=0; i < index; i++){
    for(j=0; j < index; j++){
      double value = sqrt(
       (double)((pts[interest[i]].x - pts[interest[j]].x)
        * (pts[interest[i]].x - pts[interest[j]].x) +
       (pts[interest[i]].y - pts[interest[j]].y)
        * (pts[interest[i]].y - pts[interest[j]].y)));
      if(value > max){
        max = value;
      }
    }
  }
  printf("%lf\n", max);
}
