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

float volatile robot_angle = 270;
float2 volatile robot_pos; //TODO: set the real starting point

int robot_cell_x = 9; //TODO: real start
int robot_cell_y = 4;
uint robot_cell_type = 0;

#define grid_cell_width 22.5
#define grid_cell_height 45.0
#define n_xcells 16
#define n_ycells 8

uint32 unwalkable[n_xcells*n_ycells*4/32] = {
    0xFFFFFFFF, 0xFFFFFFFF,
    0xE0000000, 0xFFFFFFFF,
    0xE0000000, 0xFFFFFFFF,
    0xE000FFFF, 0xFFFFFFFF,
    0x000FFFFF, 0xFFFFFF00,
    0x000FFFFF, 0xFFFFFF00,
    0x00000000, 0xFFFFFF00,
    0x00000000, 0xFFFFFF00,
}; //TODO: initialize this
uint indexFromLink(int x_0, int y_0, uint type)
{
    return y_0*4*n_xcells+x_0*4+type;
}

typedef struct
{
    int x_0;
    int y_0;
    int x_1;
    int y_1;
} link;

void linkFromIndex(uint i, link * out)
{
    int y = i/(4*n_xcells);
    int x = (i-y*4*n_xcells)/4;
    int type = (i-y*4*n_xcells-x*4);

    /*
      type == 1 -> |
      type == 0 ->  _
      type == 2 ->  / (these two depend on the orientation of your coordinate system)
      type == 3 ->  \

      00  0 +1
      00 +1  0
      00 +1 +1
      00 +1 -1
     */
    out->x_0 = x;
    out->y_0 = y;
    out->x_1 = x + (type!=0);
    out->y_1 = y + !(type&1)-(type==3);
}

uint typeFromIndex(uint i)
{
    int y = i/(4*n_xcells);
    int x = (i-y*4*n_xcells)/4;
    return (i-y*4*n_xcells-x*4);
}

#define robot_radius 22.5

void addUSPoint(float us)//TODO: make sure this is threadsafe
{
    float2 us_pos;

    float2 rel_us_pos;
    rel_us_pos.x = 20*cos(pi/4 + robot_angle*degrees_to_radians);//TODO: measure this
    rel_us_pos.y = 20*sin(pi/4 + robot_angle*degrees_to_radians);
    add(robot_pos, rel_us_pos, us_pos);

    float2 hit_pos;
    float2 rel_hit_pos;
    rel_hit_pos.x = us*cos(robot_angle*degrees_to_radians);
    rel_hit_pos.y = us*sin(robot_angle*degrees_to_radians);

    add(us_pos, rel_hit_pos, &hit_pos);

    int cell_x = robot_cell_x;
    int cell_y = robot_cell_y;

    if(us <= us_max_range)
    {
        for(int x = cell_x-1; x <= cell_x+1; x++) //this only works when robot_radius < grid_cell_width
        {
	        for(int y = cell_y-1; y <= cell_y+1; y++)
	        {
	            for(uint type = 0; type < 4; type++)
	            { //check for intersection between the link and the circle, can probably do the math in a better way
	                uint current_index = indexFromLink(x, y, type);
	                link current_link;
	                linkFromIndex(current_index, &current_link);

	                float sq_dist_to_end = sq(hit_pos.x-current_link.x_0*grid_cell_width)
	                                       +sq(hit_pos.y-current_link.y_0*grid_cell_height);
	                float sq_dist_to_end1 = sq(hit_pos.x-current_link.x_1*grid_cell_width)
	                                       +sq(hit_pos.y-current_link.y_1*grid_cell_height);
	                float sq_end_to_end = sq((current_link.x_0-current_link.x_1)*grid_cell_width)
	                                     +sq((current_link.y_0-current_link.y_1)*grid_cell_height);
	                if(sq_dist_to_end < sq(robot_radius))
	                {
	                    unwalkable[current_index/32] |= 1<<current_index%32;
	                }
	            }
	        }
        }
    }
}

const uint max_frontier = 1000;


void siftDown(uint16 * frontier, float * priorities, uint frontier_start, uint frontier_end)
{
    uint current = frontier_start;
    if(frontier_start > frontier_end)
    {
        frontier_end += max_frontier;
    }
    for ever
    {
        int best_child = -1;
        {
            uint child = 2*(current-frontier_start)+1;
            if(child > frontier_end-frontier_start)
            {
                return;
            }
            child = (child+frontier_start)%max_frontier;
            if(priorities[current] > priorities[child])
            {
                best_child = child;
            }
        }
        {
            uint child = 2*(current-frontier_start)+2;
            if(child > frontier_end-frontier_start)
            {
                if(best_child >= 0)
                {
                    uint swap = priorities[current];
                    priorities[current] = priorities[best_child];
                    priorities[best_child] = swap;

                    swap = frontier[current];
                    frontier[current] = frontier[best_child];
                    frontier[best_child] = swap;
                }
                return;
            }
            child = (child+frontier_start)%max_frontier;
            if(best_child >= 0)
            {
                if(priorities[best_child] > priorities[child])
                {
                    best_child = child;
                }
            }
            else
            {
                if(priorities[current] > priorities[child])
                {
                    best_child = child;
                }
            }
        }
        if(best_child <= 0)
        {
            return;
        }

        {//swap with the best child
            uint swap = priorities[current];
            priorities[current] = priorities[best_child];
            priorities[best_child] = swap;

            swap = frontier[current];
            frontier[current] = frontier[best_child];
            frontier[best_child] = swap;

            current = best_child;
        }

    }
}

void siftUp(uint16 * frontier, float * priorities, int frontier_start, int frontier_end)
{
    int current = frontier_end-1;
    for ever
    {
        int parent = frontier_start+(current-frontier_start-1)/2;
        if(parent == frontier_start)
        {
            return;
        }
        if(priorities[current] < priorities[parent]){//swap with the best child
            uint swap = priorities[current];
            priorities[current] = priorities[parent];
            priorities[parent] = swap;

            swap = frontier[current];
            frontier[current] = frontier[parent];
            frontier[parent] = swap;

            current = parent;
        }
        else
        {
            return;
        }
    }
}

const float costs_by_type[4] = {grid_cell_height, grid_cell_width, sqrt(sq(grid_cell_height) + sq(grid_cell_width)), sqrt(sq(grid_cell_height) + sq(grid_cell_width))};
const float theta_by_type[4] = {90, 0, (radians_to_degrees*atan(grid_cell_height/grid_cell_width)), (360-radians_to_degrees*atan(grid_cell_height/grid_cell_width))};
const float cost_per_degree = 0.5; //how many cm could have been traveled in the time it takes to turn 1 degree

uint16 frontier[max_frontier];
float priorities[max_frontier];
float cost_so_far[n_xcells*n_ycells*4];

uint16 came_from[n_xcells*n_ycells*4];

uint nextGotoIndex(int xp, int yp, uint typep, int xt, int yt, uint typet)
{
    uint indexp = indexFromLink(xp, yp, typep);

    uint indext = indexFromLink(xt, yt, typet);

    frontier[0] = indexFromLink(xt, yt, typet);
    priorities[0] = 0;
    uint frontier_start = 0;
    uint frontier_end = 1;

    memset(cost_so_far, 0, sizeof(cost_so_far));
    cost_so_far[frontier[0]] = 1;

    while(frontier_end!=frontier_start)
    {
        uint current = frontier[frontier_start];
        frontier_end = (frontier_end+max_frontier-1)%max_frontier;
        frontier[frontier_start] = frontier[frontier_end];
        priorities[frontier_start] = priorities[frontier_end];
        siftDown(frontier, priorities, frontier_start, frontier_end);

        link current_link;
        linkFromIndex(current, &current_link);
        uint current_type = typeFromIndex(current);

        //make sure it doesn't crash without this, I added bounds checking to the neighbor search
        if(current_link.x_0 <= 0 || current_link.y_0 <= 0)
        {
            continue;
        }
        if(current_link.x_0 >= n_xcells || current_link.y_0 >= n_ycells)
        {
            continue;
        }

        for(uint type_ = 0; type_ <= 4; type_++) //for all visitable nodes //4 is forward
        {
            uint next;
            uint cost;
            int next_x;
            int next_y;
            uint next_type;
            if(type_ == 4) //go forward
            {
                next_x = current_link.x_1;
                next_y = current_link.y_1;
                next_type = current_type;
                next = indexFromLink(current_link.x_1, current_link.y_1, current_type);
                cost = costs_by_type[current_type];
            }
            else
            {
                if(type_ == current_type) //go backward
                {
                    next_x = 2*current_link.x_0-current_link.x_1;
                    next_y = 2*current_link.y_0-current_link.y_1;
                    next_type = current_type;
                    next = indexFromLink(2*current_link.x_0-current_link.x_1, 2*current_link.y_0-current_link.y_1, type_);
                    cost = costs_by_type[type_];
                }
                else //turn
                {
                    next_x = current_link.x_0;
                    next_y = current_link.y_0;
                    next_type = type_;
                    next = indexFromLink(current_link.x_0, current_link.y_0, type_);
                    cost = abs(theta_by_type[type_] - theta_by_type[current_type])*cost_per_degree;
                }
            }

        if(next_x <= 0
         ||next_y <= 0
         ||next_x >= n_xcells
         ||next_y >= n_ycells)
        {
            continue;
        }
            if((unwalkable[next/32]>>(next%32))&1)
            {
                continue;
            }

            if(next == indexp)
            {
                if(current == indext)
                {
                    return indexp;
                }
                return current;
            }

            float heuristic = sqrt(sq((next_x-xp)*grid_cell_width)+sq((next_y-yp)*grid_cell_height))/grid_cell_width;

            int new_cost = cost_so_far[current] + cost;
            if(cost_so_far[next] == 0.0 || new_cost < cost_so_far[next])
            {
                came_from[next] = current;
                cost_so_far[next] = new_cost;
                frontier[frontier_end] = next;
                priorities[frontier_end++] = new_cost+heuristic;
                siftUp(frontier, priorities, frontier_start, frontier_end);
            }
        }
    }

    //it is imposible to reach target
    playSound(soundBeepBeep);
    return indexp; //stay
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

void orientAngle(float final_angle, int motor_vIs)
{
    float delta_angle = final_angle - robot_angle;
    delta_angle -= 360.0*floor(delta_angle/360.0); //this works for negative angles
    //0 <= delta_angle <360 after this
    clearTimer(T1);
    if(delta_angle < 90)
    {
        motor[driveR] = motor_vIs;
        motor[driveL] = -motor_vIs;
    }
    else if(delta_angle >= 90 && delta_angle < 180)
    {
	    	final_angle = 180+final_angle;
		    final_angle -= 360.0*floor(final_angle/360.0); //this works for negative angles
    		delta_angle = 180.0 - delta_angle;
        motor[driveR] = -motor_vIs;
        motor[driveL] = motor_vIs;
    }
    else if(delta_angle >= 180 && delta_angle < 270)
    {
	    	final_angle = 180+final_angle;
		    final_angle -= 360.0*floor(final_angle/360.0); //this works for negative angles
	    	delta_angle = delta_angle - 180.0;
        motor[driveR] = motor_vIs;
        motor[driveL] = -motor_vIs;
    }
    else if(delta_angle >= 270 && delta_angle < 360)
    {
        delta_angle = 360 - delta_angle;
        motor[driveR] = -motor_vIs;
        motor[driveL] = motor_vIs;
    }

    delta_angle *= (68.0/90.0);
    float initial_angle = robot_angle;

    float theta = 0;
    while(abs(theta) < delta_angle)
    {
        float dt = time1[T1]/(1000.0);
        clearTimer(T1);

        float omega = SensorValue[gyro]-offset;
        theta += dt*omega;//*gyro_adjustment; //rectangular approx.

        robot_angle = initial_angle+theta;
    }
    robot_angle = final_angle;
    motor[driveR] = 0;
    motor[driveL] = 0;
    writeDebugStreamLine("Fa: %.3f, Init: %.3f, Delta: %.3f, Curr: %.3f", final_angle, initial_angle, delta_angle, robot_angle);
}

//needs to start in the correct orientation
void gotoCoord(float x_f, float y_f, int motor_vIs)
{
    float distance = sqrt(sq(x_f - robot_pos.x) + sq(y_f - robot_pos.y))-5;
    resetDriveEncoders();

    if(cos(robot_angle*degrees_to_radians)*(x_f - robot_pos.x)+sin(robot_angle*degrees_to_radians)*(y_f - robot_pos.y) > 0)//If we need to go backwards at all
    {
        motor[driveL] = motor_vIs;
        motor[driveR] = motor_vIs;
    }
    else
    {
        motor[driveL] = -motor_vIs;
        motor[driveR] = -motor_vIs;
    }

    float x_i = robot_pos.x;
    float y_i = robot_pos.x;

    while(abs(nMotorEncoder[driveR]*drive_cm_per_tick) < distance)
    {
        robot_pos.x = lerp(abs(nMotorEncoder[driveR]*drive_cm_per_tick)/distance, x_i, x_f);
        robot_pos.y = lerp(abs(nMotorEncoder[driveR]*drive_cm_per_tick)/distance, x_i, x_f);
    }
    robot_pos.x = x_f;
    robot_pos.y = y_f;

    motor[driveL] = 0;
    motor[driveR] = 0;
}
