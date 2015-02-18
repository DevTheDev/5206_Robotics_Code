#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#define pi M_PI

const float zero_radians = pi*0.005f;
const float zero_cm = 0.5f;

float max(float a, float b){
    if(a >= b){
        return a;
    }
    return b;
}

float min(float a, float b){
    if(a <= b){
        return a;
    }
    return b;
}

float getAngle(float theta, float a, float b, float c, float d)
{

    float e = sqrt(a*a+b*b-2*a*b*cos(theta));
    if(e == 0){
        return theta;
    }
    float iota = acos((e*e+d*d-c*c)/(2*e*d));
    float kappa = pi-asin(b*sin(theta)/e);
    return pi-iota-kappa;
}

float speed(float theta, float a, float b, float c, float d)
{
    float angle_difference = (getAngle(theta+zero_radians, a, b, c, d)-getAngle(theta, a, b, c, d));
    if(angle_difference < -pi)
    {
        angle_difference += pi;
    }
    return angle_difference;
}

int main(int nargs, char ** args){
    printf("crunch crunch crunch...\n");
    float theta = 0.0f;

    float a = 1.0f;
    float b = 1.0f;
    float c = 1.0f;
    float d = 1.0f;

    float max_speed = speed(0, 1, 1, 1, 1);
    float min_speed = speed(0, 1, 1, 1, 1);

    //should probably make a struct for this
    float max_theta = 0.0;
    float max_b = 1;
    float max_c = 1;
    float max_d = 1;

    float min_theta = 0.0;
    float min_b = 1;
    float min_c = 1;
    float min_d = 1;
    
    /*
      T_1 = a+c-b-d < 0
      T_2 = d+a-b-c < 0
      T_3 = d+c-a-b > 0
      b > a+c-d
      b > a-c+d
      b < d+c-a
      b > a
      
      c >-a+b+d
      c < a+d-b
      c > a+b-d
      c > b

      d > a-b+c
      d < b+c-a
      d > a+b-c
      d > a
      
    */
    for(b = a+zero_cm; b < 10.0; b += zero_cm){
        for(c = b+zero_cm; c < 10.0; c += zero_cm){
            for(d = a+zero_cm; d < b+c-a; d += zero_cm){
                for(theta = 0; theta < 2*pi; theta += zero_radians)
                {
                    if(a+c-b-d >=0 || d+a-b-c >= 0 || d+c-a-b <= 0)
                    {
                        goto outercontinue;
                    }

                    {
                        float longest = max(max(max(a, b), c), d);
                        float shortest = min(min(min(a, b), c), d);
                        if(2*(longest+shortest) >= a+b+c+d){
                            goto outercontinue;
                        }
                    }
                    
                    float spd = speed(theta, a, b, c, d);
                    if(spd > max_speed)
                    {
                        max_speed = spd;
                        max_theta = theta;
                        max_b = b;
                        max_c = c;
                        max_d = d;
                    }
		
                    if(spd < min_speed)
                    {
                        min_speed = spd;
                        min_theta = theta;
                        min_b = b;
                        min_c = c;
                        min_d = d;
                    }
                }
            outercontinue:;
            }
        }
    }

    printf("maximum: %f; theta: %f; a: %f; b: %f; c: %f; d: %f\n", max_speed, max_theta, a, max_b, max_c, max_d);

    float inaccurate_max_theta = max_theta;
    float inaccurate_max_b = max_b;
    float inaccurate_max_c = max_c;
    float inaccurate_max_d = max_d;
    
    float less_than_zero_cm = 0.25;
    float less_than_zero_radians = pi*0.00001;
    
    for(b = max(inaccurate_max_b-zero_cm, a+less_than_zero_cm); b < inaccurate_max_b+zero_cm; b += less_than_zero_cm){
        for(c = max(inaccurate_max_c-zero_cm, b+less_than_zero_cm); c < inaccurate_max_c+zero_cm; c += less_than_zero_cm){
            for(d = max(inaccurate_max_d-zero_cm, a+less_than_zero_cm); d < inaccurate_max_d+zero_cm; d += less_than_zero_cm){
                for(theta = inaccurate_max_theta-zero_radians; theta < inaccurate_max_theta+zero_radians; theta += less_than_zero_radians)
                {
                    if(a+c-b-d >=0 || d+a-b-c >= 0 || d+c-a-b <= 0)
                    {
                        goto outercontinue2;
                    }

                    {
                        float longest = max(max(max(a, b), c), d);
                        float shortest = min(min(min(a, b), c), d);
                        if(2*(longest+shortest) >= a+b+c+d){
                            goto outercontinue2;
                        }
                    }
                    
                    float spd = speed(theta, a, b, c, d);
                    if(spd > max_speed)
                    {
                        max_speed = spd;
                        max_theta = theta;
                        max_b = b;
                        max_c = c;
                        max_d = d;
                    }
                }
            outercontinue2:;
            }
        }
    }
    
    printf("maximum: %f; theta: %f; a: %f; b: %f; c: %f; d: %f\n", max_speed, max_theta, a, max_b, max_c, max_d);
    printf("%f\n", getAngle(max_theta, a, max_b, max_c, max_d));
    printf("minimum: %f; theta: %f; a: %f; b: %f; c: %f; d: %f\n", min_speed, min_theta, a, min_b, min_c, min_d);
#if 0
    //compute using the gradient
    theta = 0.0f;
    a = 1.0f;
    b = 1.0f;
    c = 1.0f;
    d = 1.0f;

    for(int i = 0; i < 1000; i++)
    {
        printf("speed: %f; theta: %.4f; a: %.4f; b: %.4f; c: %.4f; d: %.4f\n", speed(theta, a, b, c, d), theta, a, b, c, d);
        float dtheta = (speed(theta+zero_radians, a, b, c, d)-speed(theta, a, b, c, d));
        float db = (speed(theta, a, b+zero_cm, c, d)-speed(theta, a, b, c, d));
        float dc = (speed(theta, a, b, c+zero_cm, d)-speed(theta, a, b, c, d));
        float dd = (speed(theta, a, b, c, d+zero_cm)-speed(theta, a, b, c, d));
        theta += dtheta;
        b += db;
        c += dc;
        d += dd;
        if(b <= a){
            b = a+less_than_zero_cm;
        }

        if(c <= b){
            c = b+less_than_zero_cm;
        }

        if(d <= a){
            d = a+less_than_zero_cm;
        }
        
    }
#endif	
    return 0;
}
