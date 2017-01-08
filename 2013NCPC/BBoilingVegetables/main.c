#include <stdio.h>

int main(){
  float T;
  int N;
  scanf("%f %d\n", &T, &N);
  float weights[1000];
  int i;
  for(i = 0; i < N; i++){
    scanf("%f ", &(weights[i]));
  }

  if(N <= 1){
    printf("0\n");
  }

  int maxweightindex = 0;
  for(i = 1; i < N; i++){
    if(weights[maxweightindex] > weights[i]) maxweightindex = i;
  }
  
  float p = 1;
  for(p = 1; p <= 500; p++){
    float elmaxweight = weights[maxweightindex] / p;
    int cuts = 0;
    int j;
    for(j = 0; j < N; j++){
      if(j==maxweightindex) continue;
      float k = 0;
      while(++k){
        float newSize = weights[j] / k;
        
        if(elmaxweight > newSize){
          if((newSize / elmaxweight) + 0.0001 <= T){
            j = N;
            break;
          } else {
            cuts += k-1;
            break;
          }
        }else{
          if(elmaxweight / newSize + 0.0001> T){
            cuts += k-1;
            break;
          }
        } 
      }
      // What if N - 1 is j????
      if(j == N - 1 - (maxweightindex == N-1)){
        cuts += p-1;
        printf("%d\n", cuts);
        p = 501;
        break;
      }
    }
  }
}
