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
    if(e == 0.0){
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

struct refineAnswer_return{
    float speed;
    float theta;
    float a;
    float b;
    float c;
    float d;
};

//may be inaccurate if old_zero_cm is not a multiple of less_than_zero_cm
refineAnswer_return refineAnswer(float inaccurate_max_speed, float inaccurate_max_theta, float a, float inaccurate_max_b, float inaccurate_max_c, float inaccurate_max_d, float old_zero_cm, float old_zero_radians, float less_than_zero_cm, float less_than_zero_radians)
{
    float max_speed = inaccurate_max_speed;
    float max_theta = inaccurate_max_theta;
    float max_b = inaccurate_max_b;
    float max_c = inaccurate_max_c;
    float max_d = inaccurate_max_d;
    
    for(float b = max(inaccurate_max_b-old_zero_cm, a); b < inaccurate_max_b+old_zero_cm; b += less_than_zero_cm){
        for(float c = max(inaccurate_max_c-old_zero_cm, b); c < inaccurate_max_c+old_zero_cm; c += less_than_zero_cm){
            for(float d = max(inaccurate_max_d-old_zero_cm, a); d < inaccurate_max_d+old_zero_cm; d += less_than_zero_cm){
                for(float theta = inaccurate_max_theta-old_zero_radians; theta < inaccurate_max_theta+old_zero_radians; theta += less_than_zero_radians)
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
    
    printf("maximum: %f; theta: %f; a: %f; b: %f; c: %f; d: %f\n\n", max_speed/zero_radians, max_theta, a, max_b, max_c, max_d);
    
    return {max_speed, max_theta, a, max_b, max_c, max_d}; 
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

    printf("precision: %fx base_length\n", zero_cm);
    printf("maximum: %f; theta: %f; a: %f; b: %f; c: %f; d: %f\n\n", max_speed/zero_radians, max_theta, a, max_b, max_c, max_d);

    for(int i = 0; i <= 10; i++)
    {
        printf("precision: %fx base_length\n", zero_cm/(2<<i));
        auto new_max = refineAnswer(max_speed, max_theta, a, max_b, max_c, max_d, zero_cm/(1<<i), (i==0)?zero_radians:pi*0.00001, zero_cm/(2<<i), pi*0.00001);
        max_speed = new_max.speed;
        max_theta = new_max.theta;
        max_b = new_max.b;
        max_c = new_max.c;
        max_d = new_max.d;
    }
    
    return 0;
}
