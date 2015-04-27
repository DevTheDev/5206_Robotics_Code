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

float I_bar(float rho, float l)
{
    return (1.0/3.0)*rho*l*l*l;
}

float I_bars(float theta, float rho, float a, float b, float c, float d, float l_out, float R)
{
    return
        I_bar(rho, b)/sq(gamma(theta, a, b, c, d, R)) +
        I_bar(rho, l_out) +
        (1.0/3.0)*sq(rho*c)*(sq(b) +
                             sq(gamma(theta, a, b, c, d, R)*d) +
                             gamma(theta, a, b, c, d, R)*b*d*(sin(invGetAngle(theta, a, b, c, d))*sin(theta) +
                                                           cos(invGetAngle(theta, a, b, c, d))*cos(theta)));
}

float v_f(float theta_0, float theta_f, float omega_m, float Tau_m, float m_b, float rho, float a, float b, float c, float d, float l_out, float r, float R)
{
    float omega_0 = R*omega_m * (I_bars(theta_0, rho, a, b, c, d, l_out, R)/(I_bars(theta_0, rho, a, b, c, d, l_out, R) + m_b * sq(r)*gamma(theta_0, a, b, c, d, R)));

    float v_0 = gamma(theta_0, a, b, c, d, R)*omega_0;

    float K_bars_0 = (1.0/2.0)*I_bars(theta_0, rho, a, b, c, d, l_out, R)*sq(omega_0);

    return sqrt((2.0*K_bars_0 + 2.0*Tau_m*abs(invGetAngle(theta_f, a, b, c, d)-invGetAngle(theta_0, a, b, c, d))/R + m_b*sq(v_0)) / (m_b + I_bars(theta_f, rho, a, b, c, d, l_out, R)/sq(r*gamma(theta_f, a, b, c, d, R))));
}

int main(int nargs, char ** args){
    float a = .064;
    float b = .080;
    float c = .096;
    float d = .096;
    float l_out = d;
    float r = .09;//.1043;

    float rho = 0.0;//2700*(sq(0.019)-sq(0.016)); //The "optimal" launcher is a relativistic fly wheel
    float m_b = .03;//.0117;

    float omega_m = 13.51;
    float Tau_m = 2.0*2.472;

    float theta_0 = 0.0;
    float theta_f = -pi/2;

    float R = 1.5;
    
    float v_f_Tau = v_f(theta_0, theta_f, omega_m, Tau_m, m_b, rho, a, b, c, d, l_out, r, R);
    float v_f_omega = r*gamma(theta_f, a, b, c, d, R)*omega_m;

    printf("\n");
    printf("v_f_Tau: %f\nv_f_omega: %f\nR: %f\ngamma: %f\n", v_f_Tau, v_f_omega, R, gamma(theta_f, a, b, c, d, R));
    printf("\n");
    
    while(abs(v_f_Tau-v_f_omega) > tolerance)
    {
        if(v_f_Tau > v_f_omega)
        {
            R += 0.001;
        }
        else
        {
            R -= 0.001;
        }
        v_f_Tau = v_f(theta_0, theta_f, omega_m, Tau_m, m_b, rho, a, b, c, d, l_out, r, R);
        v_f_omega = r*gamma(theta_f, a, b, c, d, R)*omega_m;
    }

    printf("v_f_Tau: %f\nv_f_omega: %f\nR: %f\ngamma: %f\n", v_f_Tau, v_f_omega, R, gamma(theta_f, a, b, c, d, R));

    printf("\n");

    R = 2.0;
    v_f_Tau = v_f(theta_0, theta_f, omega_m, Tau_m, m_b, rho, a, b, c, d, l_out, r, R);
    v_f_omega = r*gamma(theta_f, a, b, c, d, R)*omega_m;

    printf("v_f_Tau: %f\nv_f_omega: %f\nR: %f\ngamma: %f\n", v_f_Tau, v_f_omega, R, gamma(theta_f, a, b, c, d, R));

    
    return 0;
}
