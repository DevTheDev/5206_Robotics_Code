#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#define pi M_PI

const float zero_radians = pi*0.005f;
const float zero_cm = 0.05f;

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

int main(int nargs, char ** args){
    printf("crunch crunch crunch...\n");
    float theta = 0.0f;

    float a = 1.0f;
    float b = 1.0f;
    float c = 1.0f;
    float d = 1.0f;

    //compute using the gradient
    theta = 0.0f;
    a = 1.0f;
    b = 10.0f;
    c = 10.0f;
    d = 1.5f;

    for(int i = 0; i < 1000; i++)
    {
        printf("speed: %f; theta: %.4f; a: %.4f; b: %.4f; c: %.4f; d: %.4f\n", speed(theta, a, b, c, d), theta, a, b, c, d);
        float spd = speed(theta, a, b, c, d);
        float dtheta = (speed(theta+zero_radians, a, b, c, d)-spd)/zero_radians;
        float db = (speed(theta, a, b+zero_cm, c, d)-spd)/zero_cm;
        float dc = (speed(theta, a, b, c+zero_cm, d)-spd)/zero_cm;
        float dd = (speed(theta, a, b, c, d+zero_cm)-spd)/zero_cm;
        theta += dtheta;
        b += db;
        c += dc;
        d += dd;
        if(b <= a){
            b = a+zero_cm;
        }

        if(c <= b){
            c = b+zero_cm;
        }

        if(d <= a){
            d = a+zero_cm;
        }
        
        while(a+c-b-d >=0 || d+a-b-c >= 0 || d+c-a-b <= 0)
        {
            c -= zero_radians;
        }   
        {
            float longest;
            float shortest;
            do
            {
                longest = max(max(max(a, b), c), d);
                shortest = min(min(min(a, b), c), d);
                c -= zero_radians;
            }
            while(2*(longest+shortest) >= a+b+c+d);
        }
    }

    return 0;
}
