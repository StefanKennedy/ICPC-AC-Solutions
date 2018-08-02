int main() {
  double x, t;
  scanf("%lf %lf\n", &x, &t);
  double n;
  scanf("%lf\n", &n);

  int i;
  double cofs;
  for (i = 0; i < n; i++) {
    double y1, y2;
    double f;
    scanf("%lf %lf %lf\n" ,&y1, &y2, &f);
    
    double diff = y2 - y1;
    t -= diff;
    cofs += diff*f;
  }
  cofs += t;
  printf("%.10lf\n", x/cofs);
}
