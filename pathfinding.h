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

void sub(float2 a, float2 b, float2 * out)
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

const float us_max_range = 80;

const float min_neighbor_radius = 20.0;
const float max_neighbor_radius = 40.1;

const uint max_neighbors = 8;
struct point
{
    float2 pos;
    int8 neighbors[max_neighbors];
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
    rel_us_pos.x = 20*cos(pi/2 + w->robot_angle);//TODO: measure this
    rel_us_pos.y = 20*sin(pi/2 + w->robot_angle);
    add(w->robot_pos, rel_us_pos, us_pos);

    float2 hit_pos;
    float2 rel_hit_pos;
    rel_hit_pos.x = us*cos(w->robot_angle);
    rel_hit_pos.y = us*sin(w->robot_angle);

    add(us_pos, rel_hit_pos, hit_pos);

    if(us <= us_max_range && w->n_points < n_max_points)
    {
        w->points[w->n_points].pos = hit_pos;
        w->points[w->n_points].n_neighbors = 0;
        w->points[w->n_points].neighbors[0] = 0x80;

        int8 to_link[max_neighbors];
        uint n_to_link = 0;

        for(int i = w->n_points-1; i >= 0; i--) //the more recent points are more likely to be close
        {
            float2 rel_pos;
            sub(w->points[w->n_points].pos, w->points[i].pos, rel_pos);
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

                drawLine(hit_pos.x, hit_pos.y, w->points[to_link[i]].pos.x, w->points[to_link[i]].pos.y);
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
            float2 rel_start = sub(w->points[i].pos, us_pos);
            float2 rel_end = sub(w->points[w->points[i].neighbors[n]].pos, us_pos);
            float y0 = dot(rel_start, w->black_knight.dir);
            float y1 = dot(rel_end, w->black_knight.dir);
            float x0 = dot(rel_start, perp(w->black_knight.dir));
            float x1 = dot(rel_end, perp(w->black_knight.dir));

            float d = (y1*abs(x0)+y0*abs(x1))/(abs(x1)+abs(x0));

            if((US(w) != 255 || abs(dot(normalize(sub(w->points[i].pos, w->points[w->points[i].neighbors[n]].pos)), w->black_knight.dir)) < max_detection_cos) && min(US(w)*inches/2.53, us_max_range) > d+max_neighbor_radius/2.0+1
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

        if(w->points[i].n_neighbors == 0 && w->points[i].neighbors[0] < 0)
        {
            if(dot(w->black_knight.dir, normalize(sub(w->points[i].pos, us_pos))) < 0.8)
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
