#ifndef _misc
#define _misc

#define kilobyte 1024

int max(int a, int b){
  if(a > b){
    return a;
  }
  return b;
}

int min(int a, int b){
  if(a < b){
    return a;
  }
  return b;
}

#define clamp(a, min, max) (((a) < (min)) ? (min) : (((a) > (max)) ? (max) : (a)))

float absf(float a)
{
    return abs(a);
}

#define sq(a) (a)*(a)

float lerp(float t, float p1, float p2)
{
   return (1-t)*p1 + t*p2;
}

float quadBezier(float t, float p1, float p2, float p3)
{
    return sq(1-t)*p1 + 2.0*t*(1-t)*p2 + sq(t)*p3;
}

#endif
