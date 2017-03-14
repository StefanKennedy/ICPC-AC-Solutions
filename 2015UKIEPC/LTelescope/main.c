#include <stdio.h>
#include <math.h>

long hash(long x, long y){
  return x*1002+y;
}

typedef struct {
  long x;
  long y;
} pt;

double next_up(double arb, double interv){
  double mod = fmod(arb, interv);
  if(mod < 1e-10) return arb;
  if(mod < 0.001) return arb;
  return arb + interv - mod;
}

long main(){
  long N, R, C;
  scanf("%ld %ld %ld\n", &N, &R, &C);
  double blur = 65535.0 / (N*N);
  
  double orig_grid[1002*1002];
  double* new_grid = (double*)malloc(sizeof(double)*1002*1002);
  long i, j;
  for(j = 0; j < R; j++){
    for(i = 0; i < C; i++){
      long brightness;
      scanf("%x ", &brightness);
      orig_grid[hash(i, j)] = next_up(((double)brightness), blur);
      new_grid[hash(i, j)] = 0;
    }
  }

  if(N > 1){
    long a, b;
    for(b = 0; b < R; b++){
      for(a = 0; a < C; a++){
        if(orig_grid[hash(a, b)] > blur - 1 && orig_grid[hash(a, b)] < 65535){
          for(j = 0; j < N; j++){
            for(i = 0; i < N; i++){
              orig_grid[hash(i+a, j+b)] -= blur;
            }
          } 
          new_grid[hash(a+N/2, b+N/2)] = 65535;
        }
      }
    }
  } else {
    new_grid = orig_grid;
  }

  int* o = malloc(sizeof(int) * 1002 * 1002);
  int oi;
  for(oi = 0; oi < 1002*1002; oi++){
    o[oi] = 0;
  }
  pt* q = malloc(sizeof(pt) * 1002 * 1002);
  long obj_found = 0;
  for(j = 0; j <= R - N/2; j++){
    for(i = 0; i <= C - N/2; i++){
      if(new_grid[hash(i, j)] == 65535){
        long r;
        long nextq = 0;
        long lastq = 1;
        pt b;
        b.x = i;
        b.y = j;
        q[0] = b;

        obj_found++;
        /* flood fill */
        while(nextq < lastq) {
          pt n = q[nextq];
          new_grid[hash(n.x, n.y)] = 1234;
          if(n.x+1 < C && new_grid[hash(n.x+1, n.y)] == 65535 && o[hash(n.x+1, n.y)] != 1){
            pt a;
            a.x = n.x+1;
            a.y = n.y;
            q[lastq] = a;
            o[hash(a.x, a.y)] = 1;
            lastq++;
          }
          if(n.x-1 >= 0 && new_grid[hash(n.x-1, n.y)] == 65535 && o[hash(n.x-1, n.y)] != 1){
            pt a;
            a.x = n.x-1;
            a.y = n.y;
            q[lastq] = a;
            o[hash(a.x, a.y)] = 1;
            lastq++;
          }
          if(n.y+1 < R && new_grid[hash(n.x, n.y+1)] == 65535 && o[hash(n.x, n.y+1)] != 1){
            pt a;
            a.x = n.x;
            a.y = n.y+1;
            q[lastq] = a;
            o[hash(a.x, a.y)] = 1;
            lastq++;
          }
          if(n.y-1 >= 0 && new_grid[hash(n.x, n.y-1)] == 65535 && o[hash(n.x, n.y-1)] != 1){
            pt a;
            a.x = n.x;
            a.y = n.y-1;
            q[lastq] = a;
            o[hash(a.x, a.y)] = 1;
            lastq++;
          }
          nextq++;
        }
        new_grid[hash(i, j)] = 4321;
      }
    } 
  }
  printf("%d\n", obj_found);
  
  return 0;
}
