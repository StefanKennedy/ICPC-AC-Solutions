#include <stdio.h>
#include <string.h>

long main(){
  /*long p;
  for(p = 0; p < 100000; p++){
    printf("A");
  }
  printf("\n");
  for(p = 0; p < 100000; p++){
    printf("B");
  }
  printf("\n");
  return;*/
  char first[100001], second[100001];
  scanf("%s", first);
  scanf("%s", second);

  long n1 = strlen(first);
  long n2 = strlen(second);

  //printf("%ld %ld\n", n1, n2);

  long i;
  for(i = 0; i < n1 && i < n2; i++){
    if(first[i] != second[i]){
      break;
    }
  }

  long j;
  for(j = 0; j <= n1 && j <= n2 && j < (n1 - i); j++){
    if(first[n1-j-1] != second[n2-j-1]){
      break;
    }
  }
  //printf("%ld %ld\n", i, j);
  if(n2 - i - j < 0){
    //printf("Forcing\n");
    if(n2 > n1){
      printf("%ld\n", n2-n1);
    }else{
      printf("0\n");
    }
  }else{
    printf("%ld\n", n2 - i - j);
  }
  return 0;
}
