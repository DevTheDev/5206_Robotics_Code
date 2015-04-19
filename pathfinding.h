#include "actions.h"

struct float2
{
    float x;
    float y;
};

void dot(float2 a, float2 b, float2 * out)
{
  out->x = a.x*b.x;
  out->y = a.y*b.y;
}

void subt(float2 a, float2 b, float2 * out)
{
  out->x = a.x-b.x;
  out->y = a.y-b.y;
}

void add(float2 a, float2 b, float2 * out)
{
  out->x = a.x+b.x;
  out->y = a.y+b.y;
}

float dotme(float2 a)
{
    return a.x*a.x+a.y*a.y;
}

const float us_max_range = 200;

const float min_neighbor_radius = 20.0;
const float max_neighbor_radius = 40.1;

const uint max_neighbors = 8;
struct point
{
    float2 posit;
    uint8 neighbors[max_neighbors];
    uint8 n_neighbors;
};

const uint n_max_points = 127;
struct world
{
    float2 robot_pos;
    float robot_angle;
    point points[n_max_points];
    uint8 n_points;
};

void addUSPoint(world * w, float us)
{
    float2 us_pos;

    float2 rel_us_pos;
    rel_us_pos.x = 20*cos(pi/4 + w->robot_angle*degrees_to_radians);//TODO: measure this
    rel_us_pos.y = 20*sin(pi/4 + w->robot_angle*degrees_to_radians);
    add(w->robot_pos, rel_us_pos, us_pos);

    float2 hit_pos;
    float2 rel_hit_pos;
    rel_hit_pos.x = us*cos(w->robot_angle*degrees_to_radians);
    rel_hit_pos.y = us*sin(w->robot_angle*degrees_to_radians);

    add(us_pos, rel_hit_pos, &hit_pos);

    if(us <= us_max_range && w->n_points < n_max_points)
    {
        w->points[w->n_points].posit.x = hit_pos.x;
        w->points[w->n_points].posit.y = hit_pos.y;
        w->points[w->n_points].n_neighbors = 0;
        w->points[w->n_points].neighbors[0] = 0x80;

        uint8 to_link[max_neighbors];
        uint n_to_link = 0;

        for(int i = w->n_points-1; i >= 0; i--) //the more recent points are more likely to be close
        {
            float2 rel_pos;
            subt(w->points[w->n_points].posit, w->points[i].posit, &rel_pos);
            if(dotme(rel_pos) < sq(min_neighbor_radius))
            {
                goto dont_add;
            }
            if(dotme(rel_pos) < sq(max_neighbor_radius))
            {
                if(n_to_link != max_neighbors)
                {
                    to_link[n_to_link++] = i;
                }
            }
        }
        for(uint i = 0; i < n_to_link; i++)
        {
            if(w->points[to_link[i]].n_neighbors < max_neighbors && w->points[w->n_points].n_neighbors < max_neighbors)
            {
                w->points[to_link[i]].neighbors[w->points[to_link[i]].n_neighbors++] = w->n_points;
                w->points[w->n_points].neighbors[w->points[w->n_points].n_neighbors++] = to_link[i];

                drawLine(w->points[w->n_points].posit.x/3, w->points[w->n_points].posit.y/3, w->points[to_link[i]].posit.x/3, w->points[to_link[i]].posit.y/3);
            }
        }

        w->n_points++;
        if(hit_pos.x >- 0 && hit_pos.x < 100 && hit_pos.y >= 0 && hit_pos.y < 64)
        {
            setPixel(hit_pos.x, hit_pos.y);
        }
    dont_add:;
    }

#if 0 // stuff for removing points for moving robots
    for(int i = w->n_points-1; i >= 0; i--) //itterate backwards for swap remove
    {
        for(int n = w->points[i].n_neighbors-1; n >= 0; n--)
        {
            float2 rel_start = subt(w->points[i].pos, us_pos);
            float2 rel_end = subt(w->points[w->points[i].neighbors[n]].pos, us_pos);
            float y0 = dot(rel_start, w->black_knight.dir);
            float y1 = dot(rel_end, w->black_knight.dir);
            float x0 = dot(rel_start, perp(w->black_knight.dir));
            float x1 = dot(rel_end, perp(w->black_knight.dir));

            float d = (y1*abs(x0)+y0*abs(x1))/(abs(x1)+abs(x0));

            if((US(w) != 255 || abs(dot(normalize(subt(w->points[i].posit w->points[w->points[i].neighbors[n]].posit)), w->black_knight.dir)) < max_detection_cos) && min(US(w)*inches/2.53, us_max_range) > d+max_neighbor_radius/2.0+1
               && d > 0
               && x0*x1 < 0) //if seeing through where a wall should be, then remove the wall
            {
                for(int o = w->points[w->points[i].neighbors[n]].n_neighbors-1; o >= 0; o--)
                {
                    if(w->points[w->points[i].neighbors[n]].neighbors[o] == i)
                    {
                        w->points[w->points[i].neighbors[n]].neighbors[o] =
                            w->points[w->points[i].neighbors[n]].neighbors[
                                --w->points[w->points[i].neighbors[n]].n_neighbors];
                        break;
                    }
                }
                w->points[i].neighbors[n] = w->points[i].neighbors[--w->points[i].n_neighbors];
            }
        }
ints[w->n_points].neighbors[n]].n_neighbors-1; o >= 0; o--)
                    {
                        if(w->points[w->point
        if(w->points[i].n_neighbors == 0 && w->points[i].neighbors[0] < 0)
        {
            if(dot(w->black_knight.dir, normalize(subt(w->points[i].pos, us_pos))) < 0.8)
            {
                w->points[i] = w->points[--w->n_points];

                for(int n = w->points[w->n_points].n_neighbors-1; n >= 0; n--)
                {
                    for(int o = w->points[w->posit[w->n_points].neighbors[n]].neighbors[o] == w->n_points)
                        {
                            w->points[w->points[w->n_points].neighbors[n]].neighbors[o] = i;
                            break;
                        }
                    }
                }

            }
        }

        //neighbors are only made negative at birth
        if(w->points[i].n_neighbors == 0 && w->points[i].neighbors[0] >= 0)
        {
            w->points[i] = w->points[--w->n_points];

            for(int n = w->points[w->n_points].n_neighbors-1; n >= 0; n--)
            {
                for(int o = w->points[w->points[w->n_points].neighbors[n]].n_neighbors-1; o >= 0; o--)
                {
                    if(w->points[w->points[w->n_points].neighbors[n]].neighbors[o] == w->n_points)
                    {
                        w->points[w->points[w->n_points].neighbors[n]].neighbors[o] = i;
                        break;
                    }
                }
            }
        }
    }
    #endif
}

float netDist(float pos_0, float pos_f)
{
    return pos_f - pos_0;
}

#if 0
void gotox(float2 robot_position, world facing_dir, float x_coord, int motor_vIs)//Just an example for my benefit, so I can rewrite it to take a single distance. I doubt we'd want to move x and then move y.
{
    if(facing_dir.robot_angle != 180 || facing_dir.robot_angle != 0)//Reference angle from the north (face side), with 180 deg facing (0,0) and 0 deg facing (0, max x)
    {
        if(facing_dir.robot_angle >= 90 && facing_dir.robot_angle < 270)
        {
            collsionTurn((facing_dir.robot_angle <= 180 ? 180 - facing_dir.robot_angle : facing_dir.robot_angle - 180));//Turn while collsion avoiding to make sure robot is facing in an x direction it can move in
        }
        else if(facing_dir.robot_angle < 90 && facing_dir.robot_angle >= 270)
        {
            collsionTurn((facing_dir.robot_angle < 90 ? 0 - facing_dir.robot_angle : facing_dir.robot_angle - 360));
        }
    }
    int direction = 0;

    if(abs(netDist(robot_position.x, x_coord)) == netDist(robot_position.x, x_coord))//Determine which way we need to move
    {
        direction = 1;
    }
    else
    {
        direction = -1;
    }

    while(!isCollsionAvoiding() && abs(nMotorEncoder[driveR]*drive_cm_per_tick) <= abs(x_coord))//If there's nothing in the way, and less than the absolute distance needed to travel, keep moving
    {
        motor[driveL] = motor_vIs*direction;
        motor[driveR] = motor_vIs*direction;
    }
    motor[driveL] = 0;
    motor[driveR] = 0;
}
#endif

void collisionTurn(float angle, int motor_vIs)
{
}

void orientAngle(float current_angle, float final_angle, int motor_vIs)
{
    float delta_angle = final_angle - current_angle;
    if(delta_angle < -360)
    {
        delta_angle += 360;
    }
    delta_angle -= 360*floor(delta_angle/360);

    if(delta_angle > 180)
    {
        delta_angle -= 180;
        motor[driveR] = motor_vIs;
        motor[driveL] = -motor_vIs;
    }
    else
    {
        motor[driveR] = -motor_vIs;
        motor[driveL] = motor_vIs;
    }

    delta_angle = abs(delta_angle);

    float theta = 0;
    while(abs(theta) < delta_angle)
    {
        float dt = time1[T1]/(1000.0);
        clearTimer(T1);

        float omega = SensorValue[gyro]-offset;
        theta += dt*omega;//*gyro_adjustment; //rectangular approx.
    }
    motor[driveR] = 0;
    motor[driveL] = 0;

    //if(facing_dir.robot_angle < final_angle)
    //{
    //   collisionTurn(final_angle - facing_dir.robot_angle, motor_vIs);
    //}
    //else if(facing_dir.robot_angle > final_angle)
    //{
    //    collisionTurn(facing_dir.robot_angle - final_angle, motor_vIs);
    //}
    //facing_dir.robot_angle = final_angle;
}

void gotoCoord(float2 current_posit, float x_f, float y_f, int motor_vIs)
{
    float distance = sqrt(sq(x_f - current_posit.x) + sq(y_f - current_posit.y));
    nMotorEncoder[driveR] = 0;
    if(cos(angle/degrees_per_radian)*(x_f - current_posit.x)+sin(angle/degrees_per_radian)*(y_f - current_posit.y) < 0)//If we need to go backwards at all
    {
        motor[driveL] = -motor_vIs;
        motor[driveR] = -motor_vIs;
    }
    else
    {
        motor[driveL] = motor_vIs;
        motor[driveR] = motor_vIs;
    }

    while(us_dist > 40 && abs(nMotorEncoder[driveR]*drive_cm_per_tick) < distance);

    motor[driveL] = 0;
    motor[driveR] = 0;
}

#if 0
void turnToGoal()
{

}
#endif
