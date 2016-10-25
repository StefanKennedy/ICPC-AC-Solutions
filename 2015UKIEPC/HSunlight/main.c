#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.141592653589793238

struct stack{
    int* x;
    int* y;
    int location;
};

struct b{
    int x;
    int y;

    int maxxd;
    int maxyd;

    int minxd;
    int minyd;
};

int add(struct stack * s, int x, int y){
    s->location++;
    s->x[s->location] = x;
    s->y[s->location] = y;
    return 0;
}

int pop(struct stack * s){
    s->x[s->location] = 0;
    s->y[s->location] = 0;
    s->location--;
    return 0;
}

int main()
{
    int n;
    scanf("%d", &n);
    struct b * builds = malloc(sizeof(sizeof(int)) * 1200000);
    int i = 0;
    for(; i < n; i++){
        scanf("%d %d", &builds[i].x, &builds[i].y);
    }

    struct stack s;
    s.location = -1;
    s.x = (int *)malloc(sizeof(int)*400001);
    s.y = (int *)malloc(sizeof(int)*400001);
    add(&s, 0, 0);
    add(&s, builds[0].x, builds[0].y);

    double *max = (double*)malloc(sizeof(int)*400001);

    int c = 0;
    i = 1;
    for(;i<n;i++){
        c++;
        int x = builds[i].x, y = builds[i].y;
        double angleTop = atan2(s.y[s.location] - y, x - s.x[s.location]);
        if(angleTop < 0) angleTop = 0;
        double angleBehind = atan2(s.y[s.location - 1] - y, x - s.x[s.location - 1]);
        if(angleBehind < 0) angleBehind = 0;
        while(s.location > 0 && angleTop <= angleBehind){
            pop(&s);
            angleTop = atan2(s.y[s.location] - y, x - s.x[s.location]);
            if(angleTop < 0) angleTop = 0;
            angleBehind = atan2(s.y[s.location - 1] - y, x - s.x[s.location - 1]);
            if(angleBehind < 0) angleBehind = 0;
            c++;
        }

        max[i] = angleTop;

        add(&s, x, y);

    }

    double * min = malloc(sizeof(double)*400001);

    i = n - 2;
    s.location = 0;
    add(&s, builds[n-1].x, builds[n-1].y);
    for(;i>=0;i--){
        int x = builds[i].x, y = builds[i].y;

        double angleTop = atan2(s.y[s.location] - y, s.x[s.location] - x);
        if(angleTop < 0) angleTop = 0;
        double angleBehind = atan2(s.y[s.location - 1] - y, s.x[s.location - 1] - x);
        if(angleBehind < 0) angleBehind = 0;
        while(s.location > 0 && angleTop <= angleBehind){
            pop(&s);
            angleTop = atan2(s.y[s.location] - y, s.x[s.location] - x);
            if(angleTop < 0) angleTop = 0;
            angleBehind = atan2(s.y[s.location - 1] - y, s.x[s.location - 1] - x);
            if(angleBehind < 0) angleBehind = 0;
            c++;
        }

        min[i] = angleTop;

        add(&s, x, y);
        c++;
    }

    i = 0;
    for(;i<n;i++){
        printf("%0.7f\n", 12 * (PI - max[i] - min[i]) / PI);
    }

    free(min); free(max); free(builds);
    return 0;
}
