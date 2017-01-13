#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char* steps[40];
  int reg[3];
  int num;
  char* finishRegister;
} instr;

char* currRegister = "A";

instr generateInstr(int reg0, int reg1, int reg2){
  instr i;
  i.reg[0] = reg0;
  i.reg[1] = reg1;
  i.reg[2] = reg2;
  i.num = 0;
  return i;
}

int copyRegisters(instr* from, instr* to){
  (*to).reg[0] = (*from).reg[0];
  (*to).reg[1] = (*from).reg[1];
  (*to).reg[2] = (*from).reg[2];
  return 0;
}

int copySteps(instr* from, instr* to){
  int i;
  for(i = 0; i < (*from).num; i++){
    (*to).steps[i] = (*from).steps[i];
  }
  (*to).num = (*from).num;
  return 0;
}

int addStep(instr* i, char* str){
  (*i).steps[(*i).num] = str;
  (*i).num++;
  return 0;
}

int addAllSteps(instr* from, instr* to){
  int i;
  for(i = 0; i < (*from).num; i++){
    (*to).steps[(*to).num] = (*from).steps[i];
    (*to).num++;
  }
  return 0;
}

int f(char* str, char* out){
  sprintf(out, str, currRegister);
  return 0;
}

int main(){
  int input;
  scanf("%d", &input);

  if(input == 1){
    printf("ST A\n");
    printf("DI A\n");
  }
  if(input == 0){
    printf("ZE A\n");
    printf("DI A\n");
  }
  
  instr* dp = malloc(sizeof(instr) * 256);
  dp[1] = generateInstr(1, -1, -1);
  dp[1].finishRegister = "A";
  addStep(dp+1, "ST A");

  int i;
  for(i = 2; i <= input; i++){
    dp[i].num = 99;
    int regA = -1;
    int regX = -1;
    int regY = -1;

    int j;
    for(j = 1; j < i; j++){
      int cost = dp[j].num;
      int times = i/j;
      int remainder = i%j;
      int remainder_cost = 0;
      
      regA = dp[j].reg[0];
      regX = dp[j].reg[1];
      regY = dp[j].reg[2];

      if(remainder > 0){
          if(remainder == regA)
            remainder_cost = 2;
          else
            remainder_cost = dp[remainder].num + 2; // Dont add dp[remaining] if we already have this register
      }

      int result = cost + times + (times - 1) + remainder_cost + 1;
      if(result > 40) continue;

      if(result < dp[i].num){
        copyRegisters(dp+j, dp+i);
        copySteps(dp+j, dp+i);
        currRegister = dp[j].finishRegister;
        int k;
        for(k = 0; k < times; k++){
          char* step = malloc(sizeof(char) * 5);
          f("PH %s", step);
          addStep(dp+i, step);
        }
        for(k = 0; k < times-1; k++){
          addStep(dp+i, "AD");
        }
        if(remainder > 0){
          if(remainder == regA){
            addStep(dp+i, "PH A");
          }else{
            addAllSteps(dp+remainder, dp+i);
            char* step = malloc(sizeof(char) * 5);
            f("PH %s", step);
            addStep(dp+i, step);
          }
          addStep(dp+i, "AD");
        }
        currRegister = "X";
        char* step = malloc(sizeof(char) * 5);
        f("PL %s", step);
        addStep(dp+i, step);
        dp[i].finishRegister = currRegister;
      }
    }

    if(dp[i].reg[1] < 0){
      dp[i].reg[1] = i;
    }else{
      dp[i].reg[2] = i;
    }
    if(i == input){
      int k;
      for(k = 0; k < dp[i].num; k++){
        printf("%s\n", dp[i].steps[k]);
      }
      printf("DI X\n");
    }
  }
}
