#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

#define pi M_PI

const float zero_radians = pi*0.001f;
const float tolerance = 0.01;

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

#define sq(a) (a)*(a)

float getAngle(float theta_in, float a, float b, float c, float d)
{
    float e = sqrt(a*a+b*b-2*a*b*cos(theta_in));
    if(e == 0.0){
        return theta_in;
    }
    float iota = acos((e*e+d*d-c*c)/(2*e*d));
    float kappa = asin(b*sin(theta_in)/e);
    if(abs(e*cos(kappa)+b*cos(theta_in)-a) > tolerance)
    {
        kappa = pi-kappa;
    }
    return pi-iota-kappa;
}

float invGetAngle(float theta, float a, float b, float c, float d)
{
    float f = sqrt(a*a+d*d-2*a*d*cos(pi-theta));
    if(f == 0.0){
        return theta;
    }
    float lambda = acos((f*f+b*b-c*c)/(2*f*b));
    float mu = asin(d*sin(pi-theta)/f);
    if(abs(f*cos(mu)-d*cos(theta)-a) > tolerance)
    {
        mu = pi-mu;
    }
    return lambda+mu;
}

float gammaFromTheta_in(float theta_in, float a, float b, float c, float d, float R)
{
    float angle_difference = (getAngle(theta_in+zero_radians, a, b, c, d)-getAngle(theta_in, a, b, c, d));
    if(angle_difference < -2*pi)
    {
        angle_difference += 2*pi;
    }
    return angle_difference/zero_radians*R;
}

float gamma(float theta, float a, float b, float c, float d, float R)
{
    float angle_difference = (invGetAngle(theta+zero_radians, a, b, c, d)-invGetAngle(theta, a, b, c, d));
    if(angle_difference < -2*pi)
    {
        angle_difference += 2*pi;
    }
    return zero_radians/angle_difference*R;
}

float v_f(float theta_0, float theta_f, float omega_m, float Tau_m, float m_b, float a, float b, float c, float d, float r, float R)
{
    float theta = theta_0;
    float omega_s = 0;
    float Tau_s = Tau_m;
    const float dt = 0.000001;
    float time = 0.0;
    float energy = 0.0;
    
    while(theta > theta_f)
    {
        Tau_s = Tau_m*(1 - omega_s/omega_m);
        float I_eq = sq(gamma(theta, a, b, c, d, R))*m_b*sq(r);
        float dgamma_over_dtheta = (gamma(theta-zero_radians, a, b, c, d, R)-gamma(theta, a, b, c, d, R))/zero_radians;
        omega_s += (Tau_s/I_eq - dgamma_over_dtheta*sq(omega_s)) * dt;
        if(omega_s > omega_m)
        {
            omega_s = omega_m;
        }
        theta -= omega_s*gamma(theta, a, b, c, d, R) * dt;

        time += dt;
        energy += Tau_s*omega_s*dt;
#if 0
        printf("time: %.4f, power: %.2f, torque: %.2f, angular speed: %.2f, angle_out: %.2f, gamma: %.2f, dgamma/dtheta: %.2f\n", time, Tau_s*omega_s, Tau_s, omega_s, theta, gamma(theta, a, b, c, d, R), dgamma_over_dtheta);
#endif
    }

#if 1
    printf("time: %.4f, energy: %.2f, angular speed: %.2f, angle_out: %.2f, gamma: %.2f\n", time, energy, omega_s, theta, gamma(theta, a, b, c, d, R));
#endif
    
    return r*omega_s*gamma(theta, a, b, c, d, R);
}

int main(int nargs, char ** args){
    float a = .064;
    float b = .080;
    float c = .096;
    float d = .096;
    float r = .09;//.1043;

    float m_b = .03;//.0117;

    float omega_m = 13.51;
    float Tau_m = 2.0*2.472;

    float theta_0 = 0.0;
    float theta_f = -pi/2;

    printf("%f\n", v_f(theta_0, theta_f, omega_m, Tau_m, m_b, a, b, c, d, r, 1.0));
    printf("%f\n", v_f(theta_0, theta_f, omega_m, Tau_m, m_b, a, b, c, d, r, 1.5));
    printf("%f\n", v_f(theta_0, theta_f, omega_m, Tau_m, m_b, a, b, c, d, r, 2.0));
    printf("%f\n", v_f(theta_0, theta_f, omega_m, Tau_m, m_b, a, b, c, d, r, 3.0));
    
    return 0;
}
