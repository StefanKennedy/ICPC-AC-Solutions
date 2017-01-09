#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    long o;
    long i;
    long m;
    long* x;
    long* y;
} con;

int raycast_hits;

double min_line_dist(con*, int, int, int, double*, double*, double*);
double get_raycast_hit(con*, int, int, int);

int main()
{
    /*printf("1000\n");
    int x;
    for(x= 1; x < 1001; x++){
        printf("%d %d 4 %d %d %d %d %d %d %d %d\n", x, x+1, 1001 - x, 1001 - x, 1001 - x, -1*(1001-x), -1*(1001-x), -1*(1001-x), -1*(1001-x), 1001 - x);
    }
    return;*/
    /*long x;
    int blarg = 25000;
    for(x = 0; x < blarg; x++){
        printf("%ld %ld ", x - blarg, x);
    }
    for(x = 0; x < blarg; x++){
        printf("%ld %ld ", x, blarg - x);
    }
    for(x = 0; x < blarg; x++){
        printf("%ld %ld ", blarg - x, -1 * x);
    }
    for(x = 0; x < blarg; x++){
        printf("%ld %ld ", -1 * x, -1 * (blarg - x));
    }
    return;*/
    unsigned int n;
    scanf("%u\n", &n);
    if(n > 100000){
        return 1/0; // Cause a runtime error for the craic
    }

    //printf("%d\n", sizeof(con));
    con * c = malloc(100000 * sizeof(con));

    int i;
    int j;
    for(i = 0; i < n; i++){;
        scanf("%ld %ld %ld ", &c[i].o, &c[i].i, &c[i].m);
        c[i].x = malloc(c[i].m * sizeof(long));
        c[i].y = malloc(c[i].m * sizeof(long));
        for(j = 0; j < c[i].m; j++){
            double x_loc, y_loc;
            scanf("%lf %lf", &x_loc, &y_loc); // I know this is weird to use pointless variables!
            c[i].x[j] = x_loc;
            c[i].y[j] = y_loc;

        }
        scanf("\n");
    }

    /*printf("%u\n", n);

    for(i = 0; i < n; i++){
        printf("%ld %ld %ld ", c[i].o, c[i].i, c[i].m);
        for(j = 0; j < c[i].m; j++){
            printf("%ld %ld ", c[i].x[j], c[i].y[j]);
        }
        printf("\n");
    }*/

    double min_dist = -1;
    double min_x_dist;
    double min_y_dist;
    double min_z;
    double vertical;
    double ground_level;

    double min_inside_twodee_dist = -1;
    double min_outside_twodee_dist = -1;

    for(i = 0; i < n; i++){
        raycast_hits = 0;
        for(j = 1; j < c[i].m; j++){
            get_raycast_hit(c, i, j, j-1);
        }
        get_raycast_hit(c, i, 0, c[i].m-1);
        for(j = 1; j < c[i].m; j++){
            double x_loc = c[i].x[j], y_loc=c[i].y[j];

            double x_dist, y_dist, twodee_dist;
            double dist = min_line_dist(c, i, j, j-1, &x_dist, &y_dist, &twodee_dist);
            //printf("Returned distance %lf\n", dist);
            if(dist < 0){return 1/0;} // Cause a runtime error for the craic!
            if(min_dist == -1 || dist < min_dist){
                min_dist = dist;
                min_x_dist = x_dist;
                min_y_dist = y_dist;
                min_z = (c[i].i < c[i].o ? c[i].i : c[i].o); // What's the assumption here?
            }

            if(raycast_hits % 2 == 1){ // We're inside the polygon
              if(min_inside_twodee_dist == -1 || twodee_dist < min_inside_twodee_dist){ // Find the minimum dist if we end up inside
                vertical = c[i].i;
                min_inside_twodee_dist = twodee_dist;
              }
            }else{
              if(min_outside_twodee_dist == -1 || twodee_dist < min_outside_twodee_dist){ // Find the minimum dist if we end up outside
                ground_level = c[i].o; 
                min_outside_twodee_dist = twodee_dist;
              }
            }
        }
        double x_dist, y_dist, twodee_dist;
        double dist = min_line_dist(c, i, 0, c[i].m-1, &x_dist, &y_dist, &twodee_dist);
        //printf("Returned distance %lf\n", dist);
        if(dist < 0){return 1/0;} // Cause a runtime error for the craic!
        if(min_dist == -1 || dist < min_dist){
            min_dist = dist;
            min_x_dist = x_dist;
            min_y_dist = y_dist;
            min_z = (c[i].i < c[i].o ? c[i].i : c[i].o); // What's the assumption here? One is that we're inside the polygon! Check the vertical logic below
        }

        if(raycast_hits % 2 == 1){ // We're inside the polygon
            if(min_inside_twodee_dist == -1 || twodee_dist < min_inside_twodee_dist){ // Find the minimum dist if we end up inside
                vertical = c[i].i;
                min_inside_twodee_dist = twodee_dist;
            }
        }else{
            if(min_outside_twodee_dist == -1 || twodee_dist < min_outside_twodee_dist){ // Find the minimum dist if we end up outside
                ground_level = c[i].o; 
                min_outside_twodee_dist = twodee_dist;
            }
        }

        //printf("Raycast intersections: %d\n", raycast_hits);
    }

    // If origin is inside mountain/ditch then check a vertical drill
    //printf("vertical: %.10lf\n", vertical);
    if(min_inside_twodee_dist != -1){
      min_dist = (vertical < min_dist ? vertical : min_dist);
    }else{
      if(ground_level < min_dist) min_dist = ground_level;
    }
    //printf("min dist before calculations: %.10lf\n", min_dist);
    //printf("Calculating from: %lf %lf %lf\n", min_x_dist, min_y_dist, min_z);
    double calc_twodee_dist = sqrt((min_x_dist * min_x_dist) + (min_y_dist * min_y_dist));
    double calc_distance = sqrt((calc_twodee_dist*calc_twodee_dist) + (min_z * min_z));
    //printf("calc_distance is: %lf\n", calc_distance);
    printf("%.10lf\n", (min_dist < calc_distance ? min_dist : calc_distance)); // Can this mess up?

    return 0;
}

double get_raycast_hit(con* c, int i, int j, int prev){
    double x_loc = c[i].x[j];
    double y_loc = c[i].y[j];
    double prev_y_loc = c[i].y[prev];
    double prev_x_loc = c[i].x[prev];

    if(((y_loc>0) != (prev_y_loc>0))
       && (0 < (prev_x_loc-x_loc) * (0-y_loc) / (prev_y_loc - y_loc) + x_loc)){ // Horizontal Lines on the x axis?!? They'll break this Check this formula for more things. What if x is not to the left of the origin
        //printf("y_loc %.40lf, prev_y_loc %.40lf %d,%d,%d,%d\n", y_loc, prev_y_loc, y_loc<0, prev_y_loc>0, y_loc>0, prev_y_loc<0);
        raycast_hits++;
    }
    return 0;
}

double min_line_dist(con* c, int i, int j, int prev, double* x_dist, double* y_dist, double* twodee_dist){
    double x_loc = c[i].x[j];
    double y_loc = c[i].y[j];

    long x_change = c[i].x[j] - c[i].x[prev]; // My IDE had a bug or something that forced me to create these :-(
    long y_change = c[i].y[j] - c[i].y[prev];
    long z = (c[i].o < c[i].i ? c[i].o : c[i].i); // Making assumption here! Check me!

    double prev_y_loc = c[i].y[prev];
    double prev_x_loc = c[i].x[prev];

    double gradient = (c[i].x[j] == c[i].x[prev] ? 1000000000000000LL : ((double)(y_change)) / (x_change));
    double y_intercept = y_loc - x_loc * gradient;
    double perpendicular = (gradient == 0 ? 1000000000000000LL : -1*(1/gradient));

    double x = -1*(y_intercept/(gradient - perpendicular));
    double y = perpendicular * x;

    if(x_loc == prev_x_loc) x = x_loc;
    if(y_loc == prev_y_loc) y = y_loc;

    //printf("Gradient: %lf, y_intercept: %lf, Perpendicular: %lf x: %.15lf, y: %lf x_loc: %lf prev_x_loc %lf\n", gradient, y_intercept, perpendicular, x, y, x_loc, prev_x_loc);

    //printf("%.15lf < %.15lf\n", x, x_loc);
    //printf("%d,%d,%d,%d\n", x < x_loc, x < prev_x_loc, x > x_loc, x > prev_x_loc);
    if((((x < x_loc && x < prev_x_loc) || (x > x_loc && x > prev_x_loc)) || ((y < y_loc && y < prev_y_loc) || (y > y_loc && y > prev_y_loc)))
       || fabs(x) <= 1e-30){
        double dist_curr = ((x_loc - x) * (x_loc - x)) + ((y_loc - y) * (y_loc - y));
        double dist_prev = ((prev_x_loc - x) * (prev_x_loc - x)) + ((prev_y_loc - y) * (prev_y_loc - y));
        if(dist_curr < dist_prev){
            x = x_loc;
            y = y_loc;
        } else {
            x = prev_x_loc;
            y = prev_y_loc;
        }
    }

    (*x_dist) = x;
    (*y_dist) = y;

    (*twodee_dist) = sqrt((x * x) + (y * y));
    return sqrt(((*twodee_dist) * (*twodee_dist)) + (z * z));
}
