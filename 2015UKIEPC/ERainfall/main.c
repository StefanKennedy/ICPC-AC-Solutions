#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

int T;
double d;
double base;
double c;
int rain[10000];
double* mem;

int rainTotal;

long long hash(int a, int b){
    return ((long long) a) * 10000 + (long long)b;
}

double getbasecost(double start, double end){
  return base/((end+1)-start);
}

double getcost(double start, int discretestart, double end, int discreteend){
  //if(start < 0) start = 0; if(start == T) start = T-1;
  double cst = getbasecost(start, end) + rainTotal;
  //if(discreteend != end){ if(discreteend > end) cst -= rain[discreteend]; cst += (1-fabs(((double)discreteend - end))) * (double)rain[(int)(end+0.999999)]; }
  //if(discretestart != start){ if(discretestart < start) cst -= rain[discretestart]; cst += (1 - fabs(((double)discretestart - start))) * (double)rain[(int)start]; }
  return cst;
}

int getcost_shouldcountallrain(){
  T = 4;
  rain[0] = 1;
  rain[1] = 1;
  rain[2] = 4;
  rain[3] = 4;
  rainTotal +=1;
  rainTotal +=1;
  rainTotal +=4;
  rainTotal +=4;
  
  double cost = getcost(0, 0, 3, 3);
  if(cost != 10){
    printf("expected getcost() to be 10 but it was %lf\n", cost);
    assert(cost == 10);
  }
  rainTotal = 0;
  printf("getcost_shouldcountallrain passed\n");
}

int getcost_shouldcalculatesweat(){
  T = 1;
  c = 0.5;
  d = 0.34;
  base = d*d*3600*c;
  
  rain[0] = 0;

  double cost = getcost(0, 0, 0, 0);
  if(!(cost > 208.07999 && cost < 208.08001)){
    printf("expected getcost() to be 208.08, but it was %lf\n", cost);
    assert(0);
  }
  printf("getcost_shouldcalculatesweat passed\n");
}

int getcost_shouldcalculatefromendnondiscrete(){
  T = 3;
  c = 0.5;
  d = 0.34;
  base = d*d*3600*c;
  
  rain[0] = 100;
  rain[1] = 0;
  rain[2] = 100;

  rainTotal = 100;

  double cost = getcost(1, 1, 1.5, 2);
  if(!(cost > 188.71999 && cost < 188.72001)){
    printf("expected getcost() to be 188.72, but it was %lf\n", cost);
    assert(0);
  }

  rainTotal = 0;
  printf("getcost_shouldcalculatefromendnondiscrete passed\n");
}

int getcost_shouldcalculatefromendnondiscreteV2(){
  T = 3;
  c = 0.5;
  d = 0.34;
  base = d*d*3600*c;
  
  rain[0] = 100;
  rain[1] = 0;
  rain[2] = 100;

  rainTotal = 100;

  double cost = getcost(0, 0, 1.5, 1);
  if(!(cost > 233.23199 && cost < 233.23201)){
    printf("expected getcost() to be 233.232, but it was %lf\n", cost);
    assert(0);
  }

  rainTotal = 0;

  printf("getcost_shouldcalculatefromendnondiscreteV2 passed\n");
}

int getcost_shouldcalculatefromstartnondiscrete(){
  T = 3;
  c = 0.5;
  d = 0.34;
  base = d*d*3600*c;

  rain[0] = 100;
  rain[1] = 0;
  rain[2] = 100;

  rainTotal = 100;

  double cost = getcost(0.5, 0, 1, 1);
  if(!(cost > 188.71999 && cost < 188.72001)){
    printf("expected getcost() to be 188.72, but it was %lf\n", cost);
    assert(0);
  }

  rainTotal = 0;

  printf("getcost_shouldcalculatefromstartnondiscrete passed\n");
}

int getcost_shouldcalculatefromstartnondiscreteV2(){
  T = 3;
  c = 0.5;
  d = 0.34;
  base = d*d*3600*c;

  rain[0] = 100;
  rain[1] = 0;
  rain[2] = 100;

  rainTotal = 100;

  double cost = getcost(0.5, 1, 2, 2);
  if(!(cost > 233.23199 && cost < 233.23201)){
    printf("expected getcost() to be 233.232, but it was %lf\n", cost);
    assert(0);
  }

  rainTotal = 0;

  printf("getcost_shouldcalculatefromstartnondiscreteV2 passed\n");
}

test(){
  getcost_shouldcountallrain();
  getcost_shouldcalculatesweat();
  getcost_shouldcalculatefromendnondiscrete();
  getcost_shouldcalculatefromendnondiscreteV2();
  getcost_shouldcalculatefromstartnondiscrete();
  getcost_shouldcalculatefromstartnondiscreteV2();
}

int main(){
  //test(); return;
  scanf("%d\n", &T);
  scanf("%lf\n", &c);
  scanf("%lf\n", &d); 
  base = d*d*3600*c;
  int i;
  for(i = 0; i < T; i++){
    scanf("%d\n", rain+i);
  }

  // Finished reading input, solution is below:

  int lastmid = -1;
  double min = -1;
  int k, j;
  for(k = 0; k < T; k++){ // T (without - 1) correct here?
    rainTotal = 0;
    for(j = k; j < T; j++){ // starting from k+1 correct here?
      rainTotal += rain[j];
      if(rainTotal > min && min > 0) break;
      if(k > 0 && j < T - 1 && getbasecost(k-1, j+1) + rainTotal > min) continue;
      
      double cost = getbasecost(k, j) + rainTotal;
      
      lastmid = -1;

      double start; int discretestart; double end; int discreteend;
      if(j == T-1 || (k > 0 && rain[k-1] < rain[j+1])){
        double lowerstart = k, upperstart = k;
        if(lowerstart > 0) lowerstart-=1;
        while(lowerstart < upperstart - 0.000001){
          double mid = lowerstart + (upperstart-lowerstart)/2;
          if((int)(mid*100000) == lastmid) break;
          lastmid = (int)(mid*100000);
  
          ////// PASTED FUNCTION INLINE HERE
          start = mid+0.000001; discretestart = k; end = j;
          double upcost = getbasecost(start, end) + rainTotal + ((fabs(((double)discretestart - start))) * rain[(int)start]);
          /////  END OF PASTED FUNCTION INLINE
          ////// PASTED FUNCTION INLINE HERE
          start = mid-0.000001; discretestart = k; end = j;
          double downcost = getbasecost(start, end) + rainTotal + ((fabs(((double)discretestart - start))) * rain[(int)start]);
          /////  END OF PASTED FUNCTION INLINE
          if(downcost < cost) cost = downcost;
      
          if(upcost < downcost){
            lowerstart = mid;
          } else if(upcost > downcost){
            upperstart = mid;
          }
        }
  
      } else {
    
        lastmid = -1;
        double lowerend = j, upperend = j;
        if(upperend < T-1) upperend+=1;
        while(lowerend < upperend - 0.000001){
          double mid = lowerend + (upperend-lowerend)/2;
          if((int)(mid*100000) == lastmid) break;
          lastmid = (int)(mid*100000);
          
          ////// PASTED FUNCTION INLINE HERE
          start = k; end = mid+0.000001; discreteend = j;
          double upcost = getbasecost(start, end) + rainTotal + ((fabs(((double)discreteend - end))) * rain[(int)(end+0.999999)]);
          /////  END OF PASTED FUNCTION INLINE
          ////// PASTED FUNCTION INLINE HERE
          start = k; end = mid-0.000001; discreteend = j;
          double downcost = getbasecost(start, end) + rainTotal + ((fabs(((double)discreteend - end))) * rain[(int)(end+0.999999)]);
          /////  END OF PASTED FUNCTION INLINE
          if(downcost < cost) cost = downcost;
      
          if(upcost < downcost){
            lowerend = mid;
          } else if(upcost > downcost){
            upperend = mid;
          }
        }

      }
    
      if(cost < min || min < 0){
        min = cost;
      }
    }
  } 
  printf("%lf\n", min);
}
