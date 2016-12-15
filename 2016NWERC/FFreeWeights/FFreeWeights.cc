#include <iostream>
#include <stdio.h>
#include <unordered_set>

using namespace std;

int main(){

  /*int s = 1000000;
  long highasfuck = 1000000000L;
  cout << 1000000 << endl;
  for(int i = 0; i < s; i++){
    cout << highasfuck - s + i<< " ";
  }
  cout << endl;
  for(int i = 0; i < s; i++){
    cout << highasfuck - s + i<< " ";
  }
  cout << endl;

  return 0;*/
  long n;
  cin.tie(nullptr);
  scanf("%ld\n", &n);

  unordered_set<long> toprow;
  long* top = (long*)malloc(sizeof(long) * 1000000);
  long* bottom = (long*)malloc(sizeof(long) * 1000000);

  for(int i = 0; i < n; i++){
    scanf("%ld ", top + i);
    if(toprow.find(top[i]) == toprow.end()){
      toprow.insert(top[i]);
    }else{
      toprow.erase(top[i]);
    }
  }

  scanf("\n");
  for(int i = 0; i < n; i++){
    scanf("%ld ", bottom + i);
  }

  long maxweight = 0;
  for(const long& i : toprow){
    maxweight = (maxweight > i ? maxweight : i);
  }

  long low = maxweight, high = 1000000000L;

  while(1){
    long mid = low + (high-low)/2;

    long discovered = -1;

      for(int i = 0; i <= n; i++){
        if(i==n){
          high = mid;
        }
        if(top[i] <= mid){
          continue; // We can move this weight so it doesn't matter what happens to it
        }
        if(discovered < 0){
          discovered = top[i];
          continue;
        }
        if(top[i] == discovered){
          discovered = -1;
          continue;
        }
        // Something is in the way, making this impossible
        low = mid+1;
        break;
      }

    if(high==low){
      break;
    }
  }

  high = 1000000000L;
  while(1){
    long mid = low + (high-low)/2;

    long discovered = -1;

      for(int i = 0; i <= n; i++){
        if(i==n){
          high = mid;
        }
        if(bottom[i] <= mid){
          continue; // We can move this weight so it doesn't matter what happens to it
        }
        if(discovered < 0){
          discovered = bottom[i];
          continue;
        }
        if(bottom[i] == discovered){
          discovered = -1;
          continue;
        }
        // Something is in the way, making this impossible
        low = mid+1;
        break;
      }

    if(high==low){
      cout << low << endl;
      break;
    }
  }


}
