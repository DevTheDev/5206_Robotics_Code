#ifndef _misc
#define _misc

typedef unsigned int uint;
typedef unsigned char uint8;
typedef unsigned char int8;

#define kilobyte 1024

#define ever (;;)

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

/*runs one cycle of a one dimensional kalman filter
  A = 1, B = 1, H = 1
  x_k-1 = *value
  P_k-1 = *error_estimate
  v_k = new_measurement
  u_k = control_input
  R = signal_noise
  Q = process_noise
*/
void kalmanUpdate(float * value, float * error_estimate, float new_value, float control_input, float signal_noise, float process_noise)
{
    *value += control_input;
    *error_estimate += process_noise;

    float gain = *error_estimate / (*error_estimate + signal_noise);
    *value = lerp(gain, *value, new_value);
    *error_estimate *= (1 - gain);
}

//same as the above function, but without a control input of 0
void kalmanUpdate(float * value, float * error_estimate, float new_value, float signal_noise, float process_noise)
{
    *error_estimate += process_noise;

    float gain = *error_estimate / (*error_estimate + signal_noise);
    *value = lerp(gain, *value, new_value);
    *error_estimate *= (1 - gain);
}

#endif
