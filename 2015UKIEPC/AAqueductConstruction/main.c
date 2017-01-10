#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define PI 3.14159265359

typedef struct{
    int source;
    double length;
} bridge;

typedef struct{
    long x;
    long y;
    long h;
    char type;
    bridge connected[501];
    int con_size;
} hill;

int hash(int s, int t){
    return s*41+t;
}

int clearInt(int* arr, int size){
  int z;
  for(z=0;z<size;z++){
    arr[z] = 0;
  }
  return 0;
}

int maxoutDouble(double* arr, int size){
  int z;
  for(z=0;z<size;z++){
    arr[z] = 9999999;
  }
  return 0;
}

typedef struct{
    int curr;
    double dist;
    char yep[10];
} path;

long long get_dist(
        long long x1, long long x2,
        long long y1, long long y2,
        long long h1, long long h2){
    long long twodee_dist = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    long long dist = twodee_dist + (h1 - h2) * (h1 - h2);
    return dist;
}

int step = 0;
int column_set[41];
int row_set[41];
int* stars;
int* primes;

hill * hills;

int step5startx, step5starty;

double grid[1681]; // Compressed graph of source -> target
double backup[1681]; // Remembers what we had at the start (grid gets mutated)
int sources_set[501];
int sources[501];
int targets[41];

int make_square_grid(int t, int s){
    double max = -1;
    int i, j;
    for(i=0;i<t;i++){
        for(j=0;j<s;j++){
            if(grid[hash(i, j)] > max) max = grid[hash(i,j)];
        }
    }

    int a = (t > s ? t : s);
    if(t < a){
        for(i=t;i<a;i++){
            for(j=0;j<a;j++){
                grid[hash(i, j)] = max;
            }
        }
    }
    return a;
}

int impossible_fleg = 0;

int initial_reduce(int a){
    int i, j;
    for(i=0;i<a;i++){
        int impossible = 1;
        double min = -1;
        for(j=0;j<a;j++){
            int y = sources_set[sources[j]]-1;
            if(grid[hash(i, y)] == 0) grid[hash(i, y)] = 1e18;
            else impossible = 0;
            min = (min == -1 || (grid[hash(i, y)] < min) ? grid[hash(i, y)] : min);
        }
        if(impossible){
            printf("IMPOSSIBLE\n");
            impossible_fleg=1;
            break;
        }
        for(j=0;j<a;j++){
            int y = sources_set[sources[j]]-1;
            grid[hash(i, y)] = grid[hash(i, y)] - min;
        }
    }
}

int starSomeZeros(int a){
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++){
        if(grid[hash(i, j)] == 0){
            int canstar = 1;
            int k;
            for(k=0;k<a;k++){
              if(stars[hash(k, j)]){
                canstar=0;
                break;
              }
            }
            if(!canstar) continue;
            for(k=0;k<a;k++){
              if(stars[hash(i, k)]){
                canstar=0;
                break;
              }
            }
            if(!canstar) continue;
            stars[hash(i, j)] = 1;
            break;
        }
    }
}

int count = 0;
int coverColumnsWithStars(int a){
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++){
        if(stars[hash(i, j)] == 1 && !column_set[j]){
            column_set[j] = 1;
            break;
        }
    }

    step = 4;
    for(i=0;i<a;i++)
    if(!column_set[i]){
        break;
    } else if(i == a-1) {
        step = ((int)'b') + ((int)'l') - ((int)'a') - ((int)'r') + ((int)'g'); 
    }
}

int makeMoreZeros(int a){
    double smallestUncovered = -1; 
    int i, j;
    for(i=0;i<a;i++)
    if(row_set[i]) continue;
    else
    for(j=0;j<a;j++){
        if(!column_set[j])
            if(smallestUncovered > grid[hash(i,j)] || smallestUncovered < 0)
                smallestUncovered=grid[hash(i,j)];
    }
    
    for(i=0;i<a;i++)
    if(row_set[i])
    for(j=0;j<a;j++)
        grid[hash(i, j)] += smallestUncovered;

    for(j=0;j<a;j++)
    if(!column_set[j])
    for(i=0;i<a;i++)
        grid[hash(i, j)] -= smallestUncovered;

    step=4;
}

int augment_path(int a){
    int i, j; 
    int stari = -1, starj = 1;
    
    // Find an uncovered zero
    for(i=0;i<a;i++){
        if(row_set[i]) continue;
        for(j=0;j<a;j++){
            if(column_set[j]) continue;
            if(grid[hash(i, j)] == 0){
               stari = i;
               starj = j;
            }
        }
        if(stari != -1){
            break;
        }
    }
      
    if(stari == -1){
        // No zero found
        step = 6;
    }else{
        i = stari; j = starj;
        primes[hash(i, j)] = 1;
        int starinrow = -1;
        int k;
        for(k=0;k<a;k++){
            if(stars[hash(i, k)]){
                starinrow = k;
            }
        }
        if(starinrow != -1){ // Star in row
            column_set[starinrow] = 0;
            row_set[i] = 1;
        }else{ // There was no star in the row
            step5startx = j;
            step5starty = i;
            step = 5;
        }
    }
}

int fleg = 0;

int swap_stars(int a){
  int primesIndex = 1;
  int primesX[41];
  int primesY[41];
  primesX[0] = step5startx;
  primesY[0] = step5starty;

  int starsIndex = 0;
  int starsX[41];
  int starsY[41];

  int i, j;
  for(i = 0; i < a; i++){
    if(i==step5starty){
      continue;
    }
    if(stars[hash(i, step5startx)]) { // The starred zero that must have had its covering line switched
      starsX[starsIndex] = step5startx;
      starsY[starsIndex] = i;
      starsIndex++;
      for(j = 0; j < a; j++){
        if(j==step5startx) continue;
        if(grid[hash(i, j)] == 0 && primes[hash(i, j)]){
          primesX[primesIndex] = j;
          primesY[primesIndex] = i;
          primesIndex++;
          step5startx = j;
          step5starty = i;
          i = -1;
          break;
        }
      }
    }
  }

  for(i = 0; i < primesIndex; i++){
    stars[hash(primesY[i], primesX[i])] = 1;
  }

  for(i = 0; i < starsIndex; i++){
    stars[hash(starsY[i], starsX[i])] = 0;
  }
  clearInt(row_set, 41);
  clearInt(column_set, 41);
  clearInt(primes, 1681);

  step = 3;
}

int connectEverything(int n, double max_dist){
    int i, j;
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            double dist = sqrt(get_dist(hills[j].x, hills[i].x, hills[j].y, hills[i].y, hills[j].h, hills[i].h));
            if(dist < max_dist){
                if(hills[i].h < hills[j].h){ // What happens if both hills are the same height?
                    hills[i].connected[hills[i].con_size].source = j;
                    hills[i].connected[hills[i].con_size].length = dist;
                    hills[i].con_size++;
                }
                if(hills[i].h > hills[j].h){ // What happens if both hills are the same height?
                    hills[j].connected[hills[j].con_size].source = i;
                    hills[j].connected[hills[j].con_size].length = dist;
                    hills[j].con_size++;
                }
            }
        }
    } 
}


int testAugmentPathGoesToStep6WhenNoUncoveredZeros(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 0; grid[hash(3, 3)] = 1;
    column_set[0] = 1;    column_set[1] = 1;    column_set[2] = 1;    column_set[3] = 0;
    row_set[0] = 0;       row_set[1] = 0;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 1; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 1; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 1; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;

    int a = 4;
    augment_path(a);
    assert(step == 6);
    
    int expected_columns[] = {1, 1, 1, 0};
    int expected_rows[] = {0, 0, 0, 0};
    int i;
    for(i=0;i<a;i++){
      assert(column_set[i] == expected_columns[i]);
    }
    for(i=0;i<a;i++){
      assert(row_set[i] == expected_rows[i]);
    }

    printf("testAugmentPathGoesToStep6WhenNoUncoveredZeros passed!\n\n");
}

int testAugmentPathGoesToStep6AfterChangingOneCoveredLine(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 0;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1;
    column_set[0] = 1;    column_set[1] = 1;    column_set[2] = 1;    column_set[3] = 0;
    row_set[0] = 0;       row_set[1] = 0;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 1; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 1; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 1; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;

    int a = 4;
    augment_path(a);
    assert(step == 6);

    int expected_columns[] = {1, 1, 0, 0};
    int expected_rows[] = {0, 0, 1, 0};
    int i;
    for(i=0;i<a;i++){
      assert(column_set[i] == expected_columns[i]);
    }
    for(i=0;i<a;i++){
      assert(row_set[i] == expected_rows[i]);
    }
    printf("testAugmentPathGoesToStep6AfterChangingOneCoveredLine passed!\n\n");
}

int testAugmentPathGoesToStep5ZeroThatBecameUncoveredInCoveringLineSwitch(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 0;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 0;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 0;
    column_set[0] = 0;    column_set[1] = 1;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 0;       row_set[1] = 1;       row_set[2] = 1;       row_set[3] = 0;
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 1; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 1; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 1; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;

    int a = 4;
    augment_path(a);
    assert(step == 5);

    printf("testAugmentPathGoesToStep5ZeroThatBecameUncoveredInCoveringLineSwitch passed!\n\n");
}

int testOnlyCoverColumnsWithStarsCoversAllColumnsWithStars(){
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 1; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 1; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 1; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0; 
    column_set[0] = 0;    column_set[1] = 0;    column_set[2] = 0;    column_set[3] = 0;

    int a = 4;
    coverColumnsWithStars(a);
    
    int expected_columns[] = {1, 1, 1, 0};
    int i;
    for(i=0;i<a;i++){
      assert(column_set[i] == expected_columns[i]);
    }
    printf("testOnlyCoverColumnsWithStarsCoversAllColumnsWithStars passed!\n\n");
}

int testOnlyUniqueRowColumnZerosStarredWithOtherStars(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 0;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 0; grid[hash(3, 3)] = 1; 
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 0; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 0; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 1; stars[hash(3, 3)] = 0; 
    
    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 1; estars[hash(0, 2)] = 0; estars[hash(0, 3)] = 0;
    estars[hash(1, 0)] = 1; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 0;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 0; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 1;
    estars[hash(3, 0)] = 0; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 1; estars[hash(3, 3)] = 0; 
    int a = 4;

    starSomeZeros(a);
    
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);
    free(estars);
    printf("testOnlyUniqueRowColumnZerosStarredWithOtherStars passed!\n\n");
}

int testOnlyUniqueRowColumnZerosStarredWithOtherStarsV2(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 0;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 0;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 0; grid[hash(3, 3)] = 1; 
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 0; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 1;
    stars[hash(1, 0)] = 0; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 1; stars[hash(3, 3)] = 0; 
    
    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 0; estars[hash(0, 2)] = 0; estars[hash(0, 3)] = 1;
    estars[hash(1, 0)] = 1; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 0;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 0; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 0; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 1; estars[hash(3, 3)] = 0; 
    int a = 4;

    starSomeZeros(a);
    
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);
    free(estars);
    printf("testOnlyUniqueRowColumnZerosStarredWithOtherStarsV2 passed!\n\n");
}

int testOnlyUniqueRowColumnZerosStarred(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 0;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1; 
    clearInt(stars, 1681);
    
    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 1; estars[hash(0, 2)] = 0; estars[hash(0, 3)] = 0;
    estars[hash(1, 0)] = 1; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 0;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 0; estars[hash(2, 2)] = 1; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 0; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 0; estars[hash(3, 3)] = 0; 
    int a = 4;

    starSomeZeros(a);
    
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);
    free(estars);
    printf("testOnlyUniqueRowColumnZerosStarred passed!\n\n");
}

int testMoreZerosAreMadeInUncoveredColumn(){
    grid[hash(0, 0)] = 2; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 4; grid[hash(0, 3)] = 5;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 3; grid[hash(1, 2)] = 7; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 5; grid[hash(2, 1)] = 7; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 2; grid[hash(3, 1)] = 2; grid[hash(3, 2)] = 0; grid[hash(3, 3)] = 1; 
    column_set[0] = 1;    column_set[1] = 1;    column_set[2] = 1;    column_set[3] = 0;
    row_set[0] = 0;       row_set[1] = 0;       row_set[2] = 0;       row_set[3] = 0;

    double* egrid = malloc(sizeof(double) * 1681);
    egrid[hash(0, 0)] = 2; egrid[hash(0, 1)] = 0; egrid[hash(0, 2)] = 4; egrid[hash(0, 3)] = 4;
    egrid[hash(1, 0)] = 0; egrid[hash(1, 1)] = 3; egrid[hash(1, 2)] = 7; egrid[hash(1, 3)] = 0;
    egrid[hash(2, 0)] = 5; egrid[hash(2, 1)] = 7; egrid[hash(2, 2)] = 0; egrid[hash(2, 3)] = 0;
    egrid[hash(3, 0)] = 2; egrid[hash(3, 1)] = 2; egrid[hash(3, 2)] = 0; egrid[hash(3, 3)] = 0; 
    int a = 4;
    makeMoreZeros(a);
    // alright blarg
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(grid[hash(i, j)] == egrid[hash(i, j)]);
    
    
    free(egrid);
    printf("testMoreZerosAreMadeInUncoveredColumn passed!\n\n");
}

int testDoubleCoveredAreIncreased(){
    grid[hash(0, 0)] = 2; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 4; grid[hash(0, 3)] = 5;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 3; grid[hash(1, 2)] = 7; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 5; grid[hash(2, 1)] = 7; grid[hash(2, 2)] = 0; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 2; grid[hash(3, 1)] = 2; grid[hash(3, 2)] = 0; grid[hash(3, 3)] = 1; 
    column_set[0] = 1;    column_set[1] = 1;    column_set[2] = 1;    column_set[3] = 0;
    row_set[0] = 1;       row_set[1] = 1;       row_set[2] = 0;       row_set[3] = 0;

    double* egrid = malloc(sizeof(double) * 1681);
    egrid[hash(0, 0)] = 3; egrid[hash(0, 1)] = 1; egrid[hash(0, 2)] = 5; egrid[hash(0, 3)] = 5;
    egrid[hash(1, 0)] = 1; egrid[hash(1, 1)] = 4; egrid[hash(1, 2)] = 8; egrid[hash(1, 3)] = 1;
    egrid[hash(2, 0)] = 5; egrid[hash(2, 1)] = 7; egrid[hash(2, 2)] = 0; egrid[hash(2, 3)] = 0;
    egrid[hash(3, 0)] = 2; egrid[hash(3, 1)] = 2; egrid[hash(3, 2)] = 0; egrid[hash(3, 3)] = 0; 
    int a = 4;
    makeMoreZeros(a);
    // alright blarg
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(grid[hash(i, j)] == egrid[hash(i, j)]);
    
    
    free(egrid);
    printf("testDoubleCoveredAreIncreased passed!\n\n");
}

int testDoubleCoveredElementsAreIncreased(){
    grid[hash(0, 0)] = 3; grid[hash(0, 1)] = 2; grid[hash(0, 2)] = 3; grid[hash(0, 3)] = 2;
    grid[hash(1, 0)] = 2; grid[hash(1, 1)] = 3; grid[hash(1, 2)] = 2; grid[hash(1, 3)] = 3;
    grid[hash(2, 0)] = 3; grid[hash(2, 1)] = 2; grid[hash(2, 2)] = 3; grid[hash(2, 3)] = 2;
    grid[hash(3, 0)] = 2; grid[hash(3, 1)] = 3; grid[hash(3, 2)] = 2; grid[hash(3, 3)] = 3; 
    column_set[0] = 0;    column_set[1] = 1;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 0;       row_set[1] = 1;       row_set[2] = 0;       row_set[3] = 0;

    double* egrid = malloc(sizeof(double) * 1681);
    egrid[hash(0, 0)] = 1; egrid[hash(0, 1)] = 2; egrid[hash(0, 2)] = 1; egrid[hash(0, 3)] = 0;
    egrid[hash(1, 0)] = 2; egrid[hash(1, 1)] = 5; egrid[hash(1, 2)] = 2; egrid[hash(1, 3)] = 3;
    egrid[hash(2, 0)] = 1; egrid[hash(2, 1)] = 2; egrid[hash(2, 2)] = 1; egrid[hash(2, 3)] = 0;
    egrid[hash(3, 0)] = 0; egrid[hash(3, 1)] = 3; egrid[hash(3, 2)] = 0; egrid[hash(3, 3)] = 1; 
    int a = 4;
    makeMoreZeros(a);
    // alright blarg
    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(grid[hash(i, j)] == egrid[hash(i, j)]);
    
    
    free(egrid);
    printf("testDoubleCoveredElementsAreIncreased passed!\n\n");
}

int testSwapStarsImprovesSolution(){
    grid[hash(0, 0)] = 0; grid[hash(0, 1)] = 1; grid[hash(0, 2)] = 0; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 1; grid[hash(1, 1)] = 0; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 0;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 0; grid[hash(2, 2)] = 1; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 0; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1;
    column_set[0] = 0;    column_set[1] = 0;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 1;       row_set[1] = 1;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 1; stars[hash(0, 1)] = 0; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 0; stars[hash(1, 1)] = 1; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;
    primes[hash(0, 0)] = 0; primes[hash(0, 1)] = 0; primes[hash(0, 2)] = 1; primes[hash(0, 3)] = 0;
    primes[hash(1, 0)] = 0; primes[hash(1, 1)] = 0; primes[hash(1, 2)] = 0; primes[hash(1, 3)] = 1;
    primes[hash(2, 0)] = 0; primes[hash(2, 1)] = 1; primes[hash(2, 2)] = 0; primes[hash(2, 3)] = 0;
    primes[hash(3, 0)] = 0; primes[hash(3, 1)] = 0; primes[hash(3, 2)] = 0; primes[hash(3, 3)] = 0;

    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 1; estars[hash(0, 1)] = 0; estars[hash(0, 2)] = 0; estars[hash(0, 3)] = 0;
    estars[hash(1, 0)] = 0; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 1;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 1; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 0; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 0; estars[hash(3, 3)] = 0; 

    int a = 4;
    step5startx = 1;
    step5starty = 2;
    swap_stars(a);

    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);

    assert(step == 3);

    printf("testSwapStarsImprovesSolution passed!\n\n");
}

int testRepeatedSwapStarsGetsSolution(){
    grid[hash(0, 0)] = 0; grid[hash(0, 1)] = 1; grid[hash(0, 2)] = 0; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 1; grid[hash(1, 1)] = 0; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 0;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 0; grid[hash(2, 2)] = 1; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 0; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1;
    column_set[0] = 0;    column_set[1] = 0;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 1;       row_set[1] = 1;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 1; stars[hash(0, 1)] = 0; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 0; stars[hash(1, 1)] = 1; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;
    primes[hash(0, 0)] = 0; primes[hash(0, 1)] = 0; primes[hash(0, 2)] = 1; primes[hash(0, 3)] = 0;
    primes[hash(1, 0)] = 0; primes[hash(1, 1)] = 0; primes[hash(1, 2)] = 0; primes[hash(1, 3)] = 1;
    primes[hash(2, 0)] = 0; primes[hash(2, 1)] = 1; primes[hash(2, 2)] = 0; primes[hash(2, 3)] = 0;
    primes[hash(3, 0)] = 0; primes[hash(3, 1)] = 0; primes[hash(3, 2)] = 0; primes[hash(3, 3)] = 0;

    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 0; estars[hash(0, 2)] = 1; estars[hash(0, 3)] = 0;
    estars[hash(1, 0)] = 0; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 1;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 1; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 1; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 0; estars[hash(3, 3)] = 0; 

    int a = 4;
    step5startx = 1;
    step5starty = 2;
    swap_stars(a);
    step5startx = 0;
    step5starty = 3;
    primes[hash(0, 0)] = 0; primes[hash(0, 1)] = 0; primes[hash(0, 2)] = 1; primes[hash(0, 3)] = 0;
    primes[hash(1, 0)] = 0; primes[hash(1, 1)] = 0; primes[hash(1, 2)] = 0; primes[hash(1, 3)] = 0;
    primes[hash(2, 0)] = 0; primes[hash(2, 1)] = 0; primes[hash(2, 2)] = 0; primes[hash(2, 3)] = 0;
    primes[hash(3, 0)] = 1; primes[hash(3, 1)] = 0; primes[hash(3, 2)] = 0; primes[hash(3, 3)] = 0;
    swap_stars(a);

    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);

    printf("testRepeatedSwapStarsGetsSolution passed!\n\n");
}

int testSwapStarsWorksOnAChain(){
    grid[hash(0, 0)] = 1; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 0; grid[hash(0, 3)] = 1;
    grid[hash(1, 0)] = 0; grid[hash(1, 1)] = 0; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 1; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 1; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 0; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1;
    column_set[0] = 0;    column_set[1] = 0;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 1;       row_set[1] = 0;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 0; stars[hash(0, 1)] = 1; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 1; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;
    primes[hash(0, 0)] = 0; primes[hash(0, 1)] = 0; primes[hash(0, 2)] = 1; primes[hash(0, 3)] = 0;
    primes[hash(1, 0)] = 0; primes[hash(1, 1)] = 1; primes[hash(1, 2)] = 0; primes[hash(1, 3)] = 0;
    primes[hash(2, 0)] = 0; primes[hash(2, 1)] = 0; primes[hash(2, 2)] = 0; primes[hash(2, 3)] = 0;
    primes[hash(3, 0)] = 1; primes[hash(3, 1)] = 0; primes[hash(3, 2)] = 0; primes[hash(3, 3)] = 0;

    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 0; estars[hash(0, 2)] = 1; estars[hash(0, 3)] = 0;
    estars[hash(1, 0)] = 0; estars[hash(1, 1)] = 1; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 0;
    estars[hash(2, 0)] = 0; estars[hash(2, 1)] = 0; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 1; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 0; estars[hash(3, 3)] = 0; 

    int a = 4;
    step5startx = 0;
    step5starty = 3;
    swap_stars(a);

    int i, j;
    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);

    printf("testSwapStarsWorksOnAChain passed!\n\n");
}

int testRepeatedIgnoresUnprimedZeros(){
    grid[hash(0, 0)] = 0; grid[hash(0, 1)] = 0; grid[hash(0, 2)] = 1; grid[hash(0, 3)] = 0;
    grid[hash(1, 0)] = 1; grid[hash(1, 1)] = 1; grid[hash(1, 2)] = 1; grid[hash(1, 3)] = 1;
    grid[hash(2, 0)] = 0; grid[hash(2, 1)] = 1; grid[hash(2, 2)] = 1; grid[hash(2, 3)] = 1;
    grid[hash(3, 0)] = 1; grid[hash(3, 1)] = 1; grid[hash(3, 2)] = 1; grid[hash(3, 3)] = 1;
    column_set[0] = 0;    column_set[1] = 0;    column_set[2] = 0;    column_set[3] = 0;
    row_set[0] = 1;       row_set[1] = 0;       row_set[2] = 0;       row_set[3] = 0;
    stars[hash(0, 0)] = 1; stars[hash(0, 1)] = 0; stars[hash(0, 2)] = 0; stars[hash(0, 3)] = 0;
    stars[hash(1, 0)] = 0; stars[hash(1, 1)] = 0; stars[hash(1, 2)] = 0; stars[hash(1, 3)] = 0;
    stars[hash(2, 0)] = 0; stars[hash(2, 1)] = 0; stars[hash(2, 2)] = 0; stars[hash(2, 3)] = 0;
    stars[hash(3, 0)] = 0; stars[hash(3, 1)] = 0; stars[hash(3, 2)] = 0; stars[hash(3, 3)] = 0;
    primes[hash(0, 0)] = 0; primes[hash(0, 1)] = 0; primes[hash(0, 2)] = 0; primes[hash(0, 3)] = 1;
    primes[hash(1, 0)] = 0; primes[hash(1, 1)] = 0; primes[hash(1, 2)] = 0; primes[hash(1, 3)] = 0;
    primes[hash(2, 0)] = 1; primes[hash(2, 1)] = 0; primes[hash(2, 2)] = 0; primes[hash(2, 3)] = 0;
    primes[hash(3, 0)] = 0; primes[hash(3, 1)] = 0; primes[hash(3, 2)] = 0; primes[hash(3, 3)] = 0;

    int* estars = malloc(sizeof(int) * 1681);
    estars[hash(0, 0)] = 0; estars[hash(0, 1)] = 0; estars[hash(0, 2)] = 0; estars[hash(0, 3)] = 1;
    estars[hash(1, 0)] = 0; estars[hash(1, 1)] = 0; estars[hash(1, 2)] = 0; estars[hash(1, 3)] = 0;
    estars[hash(2, 0)] = 1; estars[hash(2, 1)] = 0; estars[hash(2, 2)] = 0; estars[hash(2, 3)] = 0;
    estars[hash(3, 0)] = 0; estars[hash(3, 1)] = 0; estars[hash(3, 2)] = 0; estars[hash(3, 3)] = 0; 

    int a = 4;
    step5startx = 0;
    step5starty = 2;
    swap_stars(a);

    int i, j;

    for(i=0;i<a;i++)
    for(j=0;j<a;j++)
      assert(stars[hash(i, j)] == estars[hash(i, j)]);

    printf("testRepeatedIgnoresUnprimedZeros passed!\n\n");
}

int test(){
    testAugmentPathGoesToStep6WhenNoUncoveredZeros();
    testAugmentPathGoesToStep6AfterChangingOneCoveredLine();
    testAugmentPathGoesToStep5ZeroThatBecameUncoveredInCoveringLineSwitch();
    testOnlyCoverColumnsWithStarsCoversAllColumnsWithStars();
    testMoreZerosAreMadeInUncoveredColumn();
    testDoubleCoveredAreIncreased();
    testDoubleCoveredElementsAreIncreased();
    testSwapStarsImprovesSolution();
    testRepeatedSwapStarsGetsSolution();
    testOnlyUniqueRowColumnZerosStarred();
    testRepeatedIgnoresUnprimedZeros();
    testOnlyUniqueRowColumnZerosStarredWithOtherStars();
    testOnlyUniqueRowColumnZerosStarredWithOtherStarsV2();
    testSwapStarsWorksOnAChain();
}

int main()
{
    stars = malloc(sizeof(int) * 1681);
    primes = malloc(sizeof(int) * 1681);
    //test(); return 0;
    clearInt(column_set, 41);
    clearInt(row_set, 41);
    clearInt(stars, 1681);
    int n, s, t;
    long q;
    scanf("%d %d %d %ld\n", &n, &s, &t, &q);

    hill * hills = (hill*)malloc(sizeof(hill) * 501);

    int i;
    int j;
    for(i=0;i < n;i++){
        scanf("%ld %ld %ld\n", &(hills[i].x), &(hills[i].y), &(hills[i].h));
        hills[i].con_size=0;
    }
    clearInt(sources_set, 501);
    for(i=0;i<s;i++){
        int hill_i;
        scanf("%d ", &hill_i); 
        hills[hill_i-1].type = 's';
        sources[i] = hill_i-1;
        sources_set[hill_i-1] = i+1;
    }
    scanf("\n");
    for(i=0;i<t;i++){
        int hill_i;
        scanf("%d ", &hill_i);
        hills[hill_i-1].type = 't';
        targets[i] = hill_i-1;
    }

    double max_dist = q;
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            double dist = sqrt(get_dist(hills[j].x, hills[i].x,
                                        hills[j].y, hills[i].y,
                                        hills[j].h, hills[i].h));
            if(dist <= max_dist){
                if(hills[i].h < hills[j].h){
                    hills[i].connected[hills[i].con_size].source = j;
                    hills[i].connected[hills[i].con_size].length = dist;
                    hills[i].con_size++;
                }
                if(hills[i].h > hills[j].h){
                    hills[j].connected[hills[j].con_size].source = i;
                    hills[j].connected[hills[j].con_size].length = dist;
                    hills[j].con_size++;
                }
            }
        }
    } 

    for(i=0;i<t;i++){
        path q[200000];
        int q_start = 0, q_next = 1;
        q[0].curr = targets[i];
        q[0].dist = 0;

        double visited[501];
        maxoutDouble(visited, 501);

        while(q_start < q_next){
            path popped = q[q_start];
            int hill_index = popped.curr;
            for(j=0;j<hills[hill_index].con_size;j++){
                int found = hills[hill_index].connected[j].source;
                double next_dist =
                    popped.dist + hills[hill_index].connected[j].length;
                if(next_dist >= visited[found]) continue;
                                             // I should've used Dijkstra here 
                                             // instead, but im in C so I'd have
                                             // to implement my own heap. It still
                                             // runs in time with this (I hope, but
                                             // ill find out)
                q[q_next].curr = found;
                q[q_next].dist = next_dist;
                int w;
                for(w = 0; w < 10; w++){
                  q[q_next].yep[w] = popped.yep[w];
                }
                q[q_next].yep[popped.curr] = 'A';
                visited[found] = next_dist;
                if(sources_set[found]){
                    grid[hash(i, sources_set[found]-1)] = next_dist;
                }
                q_next++;
            }
            q_start++;
        }
    }

    int a = make_square_grid(t, s);

    for(i=0;i<a;i++){
        for(j=0;j<a;j++){
            backup[hash(i, j)] = grid[hash(i, j)];
        }
    }

    initial_reduce(a);
    if(impossible_fleg){
      return 0;
    }

    starSomeZeros(a);

    step = 3;
    int loop = 1;
    while(loop){
        switch(step){
          case 3:
              coverColumnsWithStars(a);
              break;
          case 4:
              augment_path(a);
              break;
          case 5:
              swap_stars(a);
              break;
          case 6:
              makeMoreZeros(a);
              break;
          case 98:
              loop = 0;
              break;
        }
    }

    double total = 0;
    for(i=0;i<t;i++){
        for(j=0;j<a;j++){
            if(stars[hash(i, j)]){
                if(!backup[hash(i, j)]){
                  printf("IMPOSSIBLE\n");
                  return 0;
                }
                total += backup[hash(i, j)];
            }
        }
    }
    printf("%lf\n", total);

    return 0;
}
