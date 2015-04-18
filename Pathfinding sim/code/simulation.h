#ifndef SIMULATION
#define SIMULATION

#include "meth.h"
#include "misc.h"

struct memory_pool
{
    size_t size;
    void * start;
};

uint32 sphereLambertian(int x_0, int y_0, int x, int y)
{
    float xc = x_0-250;
    float yc = y_0-250;
    float nx = x/100.0;
    float ny = y/100.0;
    float nz = sqrt(1.0-sq(nx)-sq(ny));
    float pz = 200;
    float normsq = xc*xc + yc*yc + sq(pz);
    float invnorm = invsqrt(normsq);
    float lx = -xc*invnorm;
    float ly = -yc*invnorm;
    float lz = pz*invnorm;
    float lambertian = 1000.0*(lx*nx+ly*ny+nz*lz);
    int brightness = max(min(sqrt(lambertian), 255), 0);
    return brightness<<16|brightness<<8|brightness;
}

uint32 flatLambertian(int x_0, int y_0, int x, int y)
{
    float xc = x_0+x-250;
    float yc = y_0+y-250;
    float normsq = xc*xc + yc*yc + 1000.0;
    float invnorm = invsqrt(normsq);
    float lx = xc*invnorm;
    float ly = yc*invnorm;
    float lambertian = 100000.0*10.0*invnorm;
    int brightness = min(sqrt(lambertian), 255);
    return brightness<<16|brightness<<8|brightness;
}

void drawLine(uint32 * bitmap, uint stride, int x_0, int y_0, int x_1, int y_1, uint32 color)
{
    if(abs(x_1-x_0) > abs(y_1-y_0))
    {
        if(x_0 > x_1)
        {
            auto swapper = x_1;
            x_1 = x_0;
            x_0 = swapper;
            swapper = y_1;
            y_1 = y_0;
            y_0 = swapper;
        }
    
        int m = (y_1-y_0);
        for(int x = x_0; x < x_1; x++)
        {
            int y = m*(x-x_0)/(x_1-x_0)+y_0;
            bitmap[x+y*stride] = color;
        }
    }
    else
    {
        if(y_0 > y_1)
        {
            auto swapper = x_1;
            x_1 = x_0;
            x_0 = swapper;
            swapper = y_1;
            y_1 = y_0;
            y_0 = swapper;
        }
        int m = (x_1-x_0);
        for(int y = y_0; y < y_1; y++)
        {
            int x = m*(y-y_0)/(y_1-y_0)+x_0;
            bitmap[x+y*stride] = color;
        }
    }
}

void drawLine(uint32 * bitmap, uint stride, float2 p0, float2 p1, uint32 color)
{
    drawLine(bitmap, stride, p0.x, p0.y, p1.x, p1.y, color);
}

void drawLineSafe(uint32 * bitmap, uint stride, int x_0, int y_0, int x_1, int y_1, uint32 color)
{
    if(abs(x_1-x_0) > abs(y_1-y_0))
    {
        if(x_0 > x_1)
        {
            auto swapper = x_1;
            x_1 = x_0;
            x_0 = swapper;
            swapper = y_1;
            y_1 = y_0;
            y_0 = swapper;
        }
    
        int m = (y_1-y_0);
        for(int x = x_0; x < x_1; x++)
        {
            int y = m*(x-x_0)/(x_1-x_0)+y_0;
            if(x >= 0 && x < stride && y >= 0 && y <= 512)
            {
                bitmap[x+y*stride] = color;
            }
        }
    }
    else
    {
        if(y_0 > y_1)
        {
            auto swapper = x_1;
            x_1 = x_0;
            x_0 = swapper;
            swapper = y_1;
            y_1 = y_0;
            y_0 = swapper;
        }
        int m = (x_1-x_0);
        for(int y = y_0; y < y_1; y++)
        {
            int x = m*(y-y_0)/(y_1-y_0)+x_0;
            if(x >= 0 && x < stride && y >= 0 && y <= 512)
            {
                bitmap[x+y*stride] = color;
            }
        }
    }
}

void drawLineSafe(uint32 * bitmap, uint stride, float2 p0, float2 p1, uint32 color)
{
    drawLineSafe(bitmap, stride, p0.x, p0.y, p1.x, p1.y, color);
}


void drawCircleSafe(uint32 * bitmap, uint stride, int x_0, int y_0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        if(x_0+x > 0 && x_0+x < 512 && y_0+y > 0 && y_0+y < 512) bitmap[x_0+x+stride*(y_0+y)] = color;
        if(x_0+y > 0 && x_0+y < 512 && y_0+x > 0 && y_0+x < 512) bitmap[x_0+y+stride*(y_0+x)] = color;
        if(x_0-x > 0 && x_0-x < 512 && y_0-y > 0 && y_0-y < 512) bitmap[x_0-x+stride*(y_0-y)] = color;
        if(x_0-y > 0 && x_0-y < 512 && y_0-x > 0 && y_0-x < 512) bitmap[x_0-y+stride*(y_0-x)] = color;
        if(x_0+x > 0 && x_0+x < 512 && y_0-y > 0 && y_0-y < 512) bitmap[x_0+x+stride*(y_0-y)] = color;
        if(x_0+y > 0 && x_0+y < 512 && y_0-x > 0 && y_0-x < 512) bitmap[x_0+y+stride*(y_0-x)] = color;
        if(x_0-x > 0 && x_0-x < 512 && y_0+y > 0 && y_0+y < 512) bitmap[x_0-x+stride*(y_0+y)] = color;
        if(x_0-y > 0 && x_0-y < 512 && y_0+x > 0 && y_0+x < 512) bitmap[x_0-y+stride*(y_0+x)] = color;
                
        y++;
        if (err < 0){
            err += 2 * y + 1;
        }
        else{
            err += 2 * (y - x) + 1;
            x--;
        }
    }
}

void drawCircle(uint32 * bitmap, uint stride, int x_0, int y_0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        bitmap[x_0+x+stride*(y_0+y)] = color;
        bitmap[x_0+y+stride*(y_0+x)] = color;
        bitmap[x_0-x+stride*(y_0-y)] = color;
        bitmap[x_0-y+stride*(y_0-x)] = color;
        bitmap[x_0+x+stride*(y_0-y)] = color;
        bitmap[x_0+y+stride*(y_0-x)] = color;
        bitmap[x_0-x+stride*(y_0+y)] = color;
        bitmap[x_0-y+stride*(y_0+x)] = color;
                
        y++;
        if (err < 0){
            err += 2 * y + 1;
        }
        else{
            err += 2 * (y - x) + 1;
            x--;
        }
    }
}

void fillCircle(uint32 * bitmap, uint stride, int x_0, int y_0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        for(int i = x_0-x+stride*(y_0-y); i <= x_0+x+stride*(y_0-y); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x_0-y+stride*(y_0-x); i <= x_0+y+stride*(y_0-x); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x_0-x+stride*(y_0+y); i <= x_0+x+stride*(y_0+y); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x_0-y+stride*(y_0+x); i <= x_0+y+stride*(y_0+x); i++)
        {
            bitmap[i] = color;
        }        
        y++;
        if (err < 0){
            err += 2 * y + 1;
        }
        else{
            err += 2 * (y - x) + 1;
            x--;
        }
    }
}

void shadeCircle(uint32 * bitmap, uint stride, int x_0, int y_0, int r, uint32(*color)(int x_0, int y_0, int x, int y))
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        for(int i = x_0-y+stride*(y_0-x); i <= x_0+y+stride*(y_0-x); i++)
        {
            bitmap[i] = color(x_0, y_0, i-x_0-stride*(y_0-x), -x);
        }
        for(int i = x_0-x+stride*(y_0-y); i <= x_0+x+stride*(y_0-y); i++)
        {
            bitmap[i] = color(x_0, y_0, i-x_0-stride*(y_0-y), -y);
        }
        for(int i = x_0-x+stride*(y_0+y); i <= x_0+x+stride*(y_0+y); i++)
        {
            bitmap[i] = color(x_0, y_0, i-x_0-stride*(y_0+y), y);
        }
        for(int i = x_0-y+stride*(y_0+x); i <= x_0+y+stride*(y_0+x); i++)
        {
            bitmap[i] = color(x_0, y_0, i-x_0-stride*(y_0+x), x);
        }        
        y++;
        if (err < 0){
            err += 2 * y + 1;
        }
        else{
            err += 2 * (y - x) + 1;
            x--;
        }
    }
}


void drawRectangle(uint32 * bitmap, uint stride, int x_0, int y_0, int w, int h, uint32 color)
{
    int i = x_0+y_0*stride;
    for(; i < x_0+y_0*stride + w; i++)
    {
        bitmap[i] = color;
    }
    for(i = x_0+(y_0+1)*stride; i < x_0+(y_0+h-1)*stride; i += stride)
    {
        bitmap[i] = color;
        bitmap[i+w-1] = color;
    }
    for(; i < x_0+(y_0+h-1)*stride + w; i++)
    {
        bitmap[i] = color;
    }
}

void drawNgon(uint32 * bitmap, uint stride, uint n, float2 * verts, uint32 color)
{
    for(int i = 0; i < n-1; i++)
    {
        drawLine(bitmap, stride, verts[i], verts[i+1], color);
    }
    drawLine(bitmap, stride, verts[n-1], verts[0], color);
}

#define max_n 16
void fillNgon(uint32 * bitmap, uint stride, uint n, float2 * verts, uint32 color)
{
    //blah[0] -> left,
    //blah[1] -> right. 
    uint n_edges[2] = {0, 0};
    uint first_edge[2] = {0, 0}; //index of the edge in edges
    
    bool prev_side = (verts[0].y > verts[1].y);
    
    for(int i = 0; i < n; i++)
    {
        bool new_side = (verts[i].y > verts[(i+1)%n].y);
        if(prev_side != new_side)
        {
            first_edge[new_side] = i;
            prev_side = new_side;
        }
        
        n_edges[new_side]++;
    }

    {
        uint l = 1;
        uint r = 1;

        uint left_top = first_edge[0];
        uint right_top = first_edge[0];
        uint left_bot = (left_top+1)%n;
        uint right_bot = (right_top-1+n)%n;
        
        int y = min(verts[left_top].y, verts[right_top].y);
        
        for(;;y++)
        {
            int left_m_num = (verts[left_bot].x-verts[left_top].x);
            int right_m_num = (verts[right_bot].x-verts[right_top].x);
            int left_m_den = (floor(verts[left_bot].y)-floor(verts[left_top].y));
            int right_m_den = (floor(verts[right_bot].y)-floor(verts[right_top].y));

            for(int x = ((left_m_den != 0)?(left_m_num*(y-verts[left_top].y))/left_m_den:0.0) + verts[left_top].x;
                x < ((right_m_den != 0)?(right_m_num*(y-verts[right_top].y))/right_m_den:0.0) + verts[right_top].x;
                x++)
            {
                bitmap[x+stride*y] = color;
            }
            if(y >= floor(verts[left_bot].y))
            {
                if(l > n_edges[1])
                {
                    return;
                }
                l++;
                left_top = left_bot;
                left_bot = (left_bot+1)%n;
            }
            if(y >= floor(verts[right_bot].y))
            {
                if(r >= n_edges[1])
                {
                    return;
                }
                r++;
                right_top = right_bot;
                right_bot = (right_bot-1+n)%n;
            }
        }
    }
}

void fillRegularNgon(uint32 * bitmap, uint stride, uint n, float2 center, float2 r, uint32 color)
{
    float2 cis = {cos(2.0*pi/n), -sin(2.0*pi/n)};
    float2 verts[max_n] = {add(r, center)};
    for(int i = 1; i < n; i++)
    {
        r = complexx(r, cis);
        verts[i] = add(r, center);
    }
    fillNgon(bitmap, stride, n, verts, color);
}

void fillDigit(uint32 * bitmap, uint stride, uint n, int x_0, int y_0, uint32 color)
{
    char digit_bitmaps[10][26] = {
        {"XXXXXX   XX   XX   XXXXXX"},//0
        {"    X    X    X    X    X"},//1
        {"XXXXX    XXXXXXX    XXXXX"},//2
        {"XXXXX    XXXXXX    XXXXXX"},//3
        {"X   XX   XXXXXX    X    X"},//4
        {"XXXXXX    XXXXX    XXXXXX"},//5
        {"XXXXXX    XXXXXX   XXXXXX"},//6
        {"XXXXX    X   X   X   X   "},//7
        {"XXXXXX   XXXXXXX   XXXXXX"},//8
        {"XXXXXX   XXXXXX    X    X"},//8
    };

    uint scale = 3;
    
    for(int x = 0; x < 5*scale; x++)
    {
        for(int y = 0; y < 5*scale; y++)
        {
            int i = x/scale+y/scale*5;
            if(digit_bitmaps[n][i] == 'X')
            {
                bitmap[x+x_0+(y+y_0)*stride] = color;
            }
        }
    }
}

//////////////simulation

struct robot
{
    float2 dir;
    float2 pos;
};

union wall
{
    struct{
        float2 start;
        float2 end;
    };
    float coords[4];
};

struct user_input
{
    uint32 buttons;
};

#define inches 512.0/144.0

const float min_neighbor_radius = 4.5*inches;
const float max_neighbor_radius = 9.01*inches;

const uint max_neighbors = 8;

struct detection_point
{
    float2 pos;
    int8 neighbors[max_neighbors];
    uint8 n_neighbors;
};

const int tube_radius = 4.04*inches;
const int base_radius = 10*inches;

const int graph_y_scale = 64;
const int graph_x_scale = 32;

const int grid_stride = 64;
const int grid_size = 8;
const uint n_max_points = 127;
struct world
{
    robot black_knight;
    robot white_knight;
    float2 goals[6];
    wall walls[16];
    float2 center_hexagon[6];
    uint16 grid[256];
    detection_point points[n_max_points];
    uint n_points;
    uint32 unwalkable[(512/graph_x_scale+1)*4*(512/graph_y_scale)/32];
};


struct link
{
    int x_0;
    int y_0;
    int x_1;
    int y_1;
};

uint indexFromLink(uint width, int x_0, int y_0, uint type)
{    
    return y_0*4*width+x_0*4+type;
}

link linkFromIndex(uint width, uint i)
{
    //TODO: special case for x == width
    link out;
    int y = i/(4*width);
    int x = (i-y*4*width)/4;
    int type = (i-y*4*width-x*4)%width;

    /*
      type == 1 -> |
      type == 0 ->  _
      type == 2 ->  \
      type == 3 ->  /
      
      00  0 +1
      00 +1  0
      00 +1 +1
      00 +1 -1

      0, type == 3, type != 0, !(type & 1)
     */
    out.x_0 = x;
    out.y_0 = y;
    out.x_1 = x + (type!=0);
    out.y_1 = y + !(type&1)-(type==3);
    return out;
}

const float2 robot_rel_corner = {9*inches, 9*inches};

void cutOffGraph(world * w, uint graph_width, int xp, int yp)
{
    for(int x = xp/graph_x_scale-2; x <= xp/graph_x_scale+3; x++)//TODO: actually use the graph_x_scale
    {
        for(int y = yp/graph_y_scale-2; y <= yp/graph_y_scale+3; y++)
        {
            for(uint type = 0; type < 4; type++)
            {
                uint graph_index = indexFromLink(graph_width, x, y, type);
                link link = linkFromIndex(graph_width, graph_index);
                float sq_dist_to_end = sq(xp-link.x_0*graph_x_scale)+sq(yp-link.y_0*graph_y_scale);
                float sq_dist_to_end1 = sq(xp-link.x_1*graph_x_scale)+sq(yp-link.y_1*graph_y_scale);
                float sq_end_to_end = sq((link.x_0-link.x_1)*graph_x_scale) + sq((link.y_0-link.y_1)*graph_y_scale);
                if(sq_dist_to_end-sq((yp-link.y_0*graph_y_scale)*(link.y_0-link.y_1))/sq_end_to_end-sq((xp-link.x_0*graph_x_scale)*(link.x_0-link.x_1))/sq_end_to_end < sq(9*inches)
                    &&
                   (((xp-link.x_0*graph_x_scale)*(link.x_0-link.x_1)*graph_x_scale+(yp-link.y_0*graph_y_scale)*(link.y_0-link.y_1)*graph_y_scale)
                    *((xp-link.x_1*graph_x_scale)*(link.x_0-link.x_1)*graph_x_scale+(yp-link.y_1*graph_y_scale)*(link.y_0-link.y_1)*graph_y_scale) < 0)
                   || sq_dist_to_end < sq(9*inches)
                   || sq_dist_to_end1 < sq(9*inches))
                {
                    w->unwalkable[graph_index/32] |= 1<<graph_index%32;
                }
            }
        }
    }
}

int getCurrentRobotLink(world *w)
{
    float theta = 180.0/pi*atan2(w->black_knight.dir.y, w->black_knight.dir.x);
    int xp = (w->black_knight.pos.x)/graph_x_scale;
    int yp = (w->black_knight.pos.y)/graph_y_scale;

    if(theta < 0)
    {
        theta += 360;
    }
    if(theta > 360)
    {
        assert(0);
    }
    
    const float cutoff_angle = 180/pi*atan(2.0f)/2;
    
    if(theta >= 360-cutoff_angle || theta < cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 1);
    }
    if(theta >= cutoff_angle && theta < 45+cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 2);
    }
    if(theta >= 45+cutoff_angle && theta < 180-45-cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 0);
    }
    if(theta >= 180-45-cutoff_angle && theta < 180-cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 3);
    }
    if(theta >= 180-cutoff_angle && theta < 180+cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 1);
    }
    if(theta >= 180+cutoff_angle && theta < 180+45+cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 2);
    }
    if(theta >= 180+45+cutoff_angle && theta < 360-45-cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 0);
    }
    if(theta >= 360-45-cutoff_angle && theta < 360-cutoff_angle)
    {
        return indexFromLink(512/graph_x_scale, xp, yp, 3);
    }

    assert(0);
    return 0;//should be impossible
}

const uint max_frontier = 10000;

void rebalanceHeap(uint * frontier, float * priorities, uint frontier_start, uint frontier_end)
{
    uint current = frontier_start;
    if(frontier_start > frontier_end)
    {
        frontier_end += max_frontier;
    }
    for ever
    {
        for(int c = 0; c < 2; c++) //loop over children
        {
            uint child = 2*(current-frontier_start)+c;
            if(child > frontier_end-frontier_start)
            {
                return;
            }
            child = (child+frontier_start)%max_frontier;
            if(priorities[current] < priorities[child])
            {
                uint swap = priorities[current];
                priorities[current] = priorities[child];
                priorities[child] = swap;

                swap = frontier[current];
                frontier[current] = frontier[child];
                frontier[child] = swap;
                
                current = child;
                break;
            }
            if(c == 1)
            {
                return;
            }
        }
    }
}

uint typeFromIndex(uint width, uint i)
{
    int y = i/(4*width+1);
    int x = (i-y*4*width)/4;
    return (i-y*4*width-x)%width;
}

const float costs_by_type[] = {2, 1, sqrt(3.0f), sqrt(3.0f)};
const float theta_by_type[] = {90, 0, 180.0/pi*atan(2.0f), -180.0/pi*atan(2.0f)};
const float cost_per_degree = 1.0/45.0;

//t -> target, p -> current

uint nextGotoIndex(uint32 * bitmap, uint stride, world * w, uint graph_width, int xp, int yp, uint typep, int xt, int yt, uint typet)
{
    drawCircle(bitmap, stride, xt*graph_x_scale, yt*graph_y_scale, 10, 0x0000FF);
    
    uint indexp = indexFromLink(graph_width, xp, yp, typep);
    
    uint frontier[max_frontier];
    float priorities[max_frontier];
    frontier[0] = indexFromLink(graph_width, xt, yt, typet);
    priorities[0] = 0;
    uint frontier_start = 0;
    uint frontier_end = 1;
    
    float cost_so_far[512/graph_x_scale*4*512/graph_y_scale];
    memset(cost_so_far, 0, sizeof(cost_so_far));
    cost_so_far[frontier[0]] = 1;

    while(frontier_end!=frontier_start)
    {
        uint current = frontier[frontier_start];
        frontier_end = (frontier_end+max_frontier-1)%max_frontier;
        frontier[frontier_start] = frontier[frontier_end];
        priorities[frontier_start] = priorities[frontier_end];
        rebalanceHeap(frontier, priorities, frontier_start, frontier_end);
        
        {
            link a = linkFromIndex(512/graph_x_scale, current);
                
            drawLineSafe(bitmap, stride, a.x_0*graph_x_scale+2, a.y_0*graph_y_scale+2, a.x_1*graph_x_scale+2, a.y_1*graph_y_scale+2, 0x0000FF);
        }
        
        link current_link = linkFromIndex(graph_width, current);
        uint current_type = typeFromIndex(graph_width, current);

        if(current_link.x_0 <= 1 || current_link.y_0 <= 1)
        {
            continue;
        }
        if(current_link.x_0 >= graph_width || current_link.y_0 >= 8)
        {
            continue;
        }
        
        for(uint type = 0; type <= 4; type++) //for all visitable nodes //4 is forward
        {
            uint next;
            uint cost;
            if(type == 4)
            {
                next = indexFromLink(graph_width, current_link.x_1, current_link.y_1, current_type);
                cost = costs_by_type[current_type];
            }
            else
            {
                if(type == current_type)
                {
                    next = indexFromLink(graph_width, 2*current_link.x_0-current_link.x_1, 2*current_link.y_0-current_link.y_1, type);
                    cost = costs_by_type[type];
                }
                else
                {
                    next = indexFromLink(graph_width, current_link.x_0, current_link.y_0, type);
                    cost = abs(theta_by_type[type] - theta_by_type[current_type])*cost_per_degree;
                }
            }
            
            if((w->unwalkable[next/32]>>(next%32))&1)
            {
                continue;
            }
            
            if(next == indexp)
            {
                return current;
            }
          
            int new_cost = cost_so_far[current] + cost;
            if(cost_so_far[next] == 0.0 || new_cost < cost_so_far[next])
            {
                float heuristic = 0;//sqrtf(sq((current_link.x_0-xp)*graph_x_scale)+sq((current_link.y_0-yp)*graph_y_scale))/graph_x_scale;
                cost_so_far[next] = new_cost;
                frontier_start = (frontier_start+max_frontier-1)%max_frontier;
                frontier[frontier_start] = next;
                priorities[frontier_start] = new_cost+heuristic;
                rebalanceHeap(frontier, priorities, frontier_start, frontier_end);
            }
        }
    }

    //it is imposible to reach target
    return indexp; //stay
}

void resetField(world * w, uint cg)
{
    w->goals[0] = {1408.0/3.0, 128.0/3.0};
    w->goals[1] = {1408.0/3.0, 384.0/3.0};
    w->goals[2] = {1152.0/3.0, 128.0/3.0};
    w->goals[3] = {128.0/3.0, 1408.0/3.0};
    w->goals[4] = {384.0/3.0, 1408.0/3.0};
    w->goals[5] = {128.0/3.0, 1152.0/3.0};
    
    wall walls[16] = {
        //side walls
        0, 0, 0, 512,
        0, 512, 512, 512,
        512, 512, 512, 0,
        512, 0, 0, 0,
        //ramps
        //512, 512, 928.0/3.0, 512,
        3848.0/9.0, 512, 3848.0/9.0, 928.0/3.0,
        3848.0/9.0, 928.0/3.0, 512, 928.0/3.0,
        //512, 928.0/3.0, 512, 512,
            
        //0, 0, 608.0/3.0, 0,
        760.0/9.0, 0, 760.0/9.0, 608.0/3.0,
        760.0/9.0, 608.0/3.00, 0, 608.0/3.0,
        //0, 608.0/3.0, 0, 0,
        //center goal
    };
    
    float2 cg_corner = {256.0/6.0, 256.0/18.0};

    float2 cg_rot = {cos(float(cg-1)*pi/4.0), sin(float(cg-1)*pi/4.0)};

    float2 center = {256, 256};
    
    for(int i = 0; i < 4; i++)
    {
        walls[i+12].start.x = cg_corner.x*(((i^(i<<1))&2)-1.0);
        walls[i+12].start.y = cg_corner.y*((i&2)-1.0);
        walls[i+12].end.x = cg_corner.x*((((i+1)^(i+1<<1))&2)-1.0);
        walls[i+12].end.y = cg_corner.y*(((i+1)&2)-1.0);
        walls[i+12].start = add(complexx(walls[i+12].start, cg_rot), center);
        walls[i+12].end = add(complexx(walls[i+12].end, cg_rot), center);
    }

    float2 center_hexagon[] = {
        0, 27.82/2.0*inches,
        15*inches, 2.604*inches,
        15*inches, -2.604*inches,
        0, -27.82/2.0*inches,
        -15*inches, -2.604*inches,
        -15*inches, 2.604*inches,
    };
    for(int i = 0; i < len(center_hexagon); i++)
    {
        center_hexagon[i] = add(complexx(center_hexagon[i], cg_rot), center);
    }
    
    memcpy(w->center_hexagon, center_hexagon, sizeof(center_hexagon));
    
    memcpy(w->walls, walls, sizeof(walls));
    w->black_knight.pos = {469.8, 256};
    w->black_knight.dir = {0.0, -1.0};

    w->white_knight.pos = {256, 9*sqrt(2.0)*inches};
    w->white_knight.dir = {0.0, 1.0};

    memset(w->grid, 0, sizeof(w->grid));

    memset(w->unwalkable, 0, sizeof(w->unwalkable));


    w->unwalkable[indexFromLink(16, 6, 4, 3)/32] |= 1<<(indexFromLink(16, 6, 3, 3)%32);
    w->unwalkable[indexFromLink(16, 6, 4, 2)/32] |= 1<<(indexFromLink(16, 6, 4, 2)%32);
    w->unwalkable[indexFromLink(16, 9, 3, 2)/32] |= 1<<(indexFromLink(16, 9, 3, 2)%32);
    w->unwalkable[indexFromLink(16, 9, 5, 3)/32] |= 1<<(indexFromLink(16, 9, 4, 3)%32);
    
    w->n_points = 0;
}

const float2 us_rel_pos = {7.5*inches, 6*inches};
const float us_max_range = 90.0/2.53*inches;
const float max_detection_cos = 0.5;

uint8 US(world * w)
{
    float2 us_pos = add(complexx(us_rel_pos, w->black_knight.dir), w->black_knight.pos);

    float distance = 255*inches;

    for(int i = 0; i < 6; i++)
    {
        float min_dist_to_center = abs(dot(sub(w->goals[i], us_pos), perp(w->black_knight.dir)));
        float dist_to_min_dist_to_center_point = dot(sub(w->goals[i], us_pos), w->black_knight.dir);
        if(min_dist_to_center < tube_radius && dist_to_min_dist_to_center_point > 0)
        {
            distance = dist_to_min_dist_to_center_point - sqrt(sq(tube_radius) - sq(min_dist_to_center));
        }
    }
    
    for(int i = 0; i < len(w->walls); i++)
    {
        float2 rel_start = sub(w->walls[i].start, us_pos);
        float2 rel_end = sub(w->walls[i].end, us_pos);
        float y_0 = dot(rel_start, w->black_knight.dir);
        float y_1 = dot(rel_end, w->black_knight.dir);
        float x_0 = dot(rel_start, perp(w->black_knight.dir));
        float x_1 = dot(rel_end, perp(w->black_knight.dir));
        if(x_0*x_1 < 0 && abs(dot(normalize(sub(w->walls[i].start, w->walls[i].end)), w->black_knight.dir)) < max_detection_cos)
        {
            float new_distance = (y_1*abs(x_0)+y_0*abs(x_1))/(abs(x_1)+abs(x_0));

            if(new_distance > 0 && new_distance < us_max_range)
            {
                distance = min(new_distance, distance);
            }
        }
    }

    {
        float2 reverse_dir = w->white_knight.dir;
        reverse_dir.y = -reverse_dir.y;
        float2 quadrent = complexx(sub(us_pos, w->white_knight.pos), reverse_dir);
        
        float2 robot_rel_corner1 = robot_rel_corner;
        
        float2 corner0;
        if(quadrent.y < 0)
        {
            robot_rel_corner1.y = -robot_rel_corner1.y;
        }
        if(quadrent.x < 0)
        {
            robot_rel_corner1.x = -robot_rel_corner1.x;
        }
        corner0 = add(w->white_knight.pos, complexx(robot_rel_corner1, w->white_knight.dir));

        float2 corner1;
        if(dot(perp(sub(corner0, us_pos)), w->black_knight.dir) > 0)
        {
            float swap = robot_rel_corner1.x;
            robot_rel_corner1.x = -robot_rel_corner1.y;
            robot_rel_corner1.y = swap;
        }
        else
        {
            float swap = robot_rel_corner1.x;
            robot_rel_corner1.x = robot_rel_corner1.y;
            robot_rel_corner1.y = -swap;
        }        
        corner1 = add(w->white_knight.pos, complexx(robot_rel_corner1, w->white_knight.dir));

        float2 rel_start = sub(corner0, us_pos);
        float2 rel_end = sub(corner1, us_pos);
        float y_0 = dot(rel_start, w->black_knight.dir);
        float y_1 = dot(rel_end, w->black_knight.dir);
        float x_0 = dot(rel_start, perp(w->black_knight.dir));
        float x_1 = dot(rel_end, perp(w->black_knight.dir));
        if(x_0*x_1 < 0 && abs(dot(normalize(sub(corner1, corner0)), w->black_knight.dir)) < max_detection_cos)
        {
            float new_distance = (y_1*abs(x_0)+y_0*abs(x_1))/(abs(x_1)+abs(x_0));

            if(new_distance > 0 && new_distance < us_max_range)
            {
                distance = min(new_distance, distance);
            }
        }
    }
    
    return min(distance/(inches)*2.53, 255);
}

bool inBounds(int x, int y)
{
    int robot_radius = 9*inches;
    return (x > 0+robot_radius &&
            y > 0+robot_radius &&
            x < 512-robot_radius &&
            y < 512-robot_radius &&
            !(x < 760.0/9.0+robot_radius && y < 608.0/3.0+robot_radius) &&
            !(x > 512-760.0/9.0-robot_radius && y > 512-608.0/3.0-robot_radius) &&
            !(x > 256-80+robot_radius &&
              y > 256-80+robot_radius &&
              x < 256+80-robot_radius &&
              y < 256+80-robot_radius)
        );
}

void simulateAndRender(uint32 * bitmap, uint stride, world * w, float dt, user_input input)
{
    memset(bitmap, 0x626262, 512*512*sizeof(uint32));

    fillNgon(bitmap, stride, 6, w->center_hexagon, 0xFFFF00);

       
    fillRegularNgon(bitmap, stride, 4, w->black_knight.pos, complexx(robot_rel_corner, w->black_knight.dir), 0x0000FF);
    
    {//unwalkable graph stuff
        link start_link = linkFromIndex(512/graph_x_scale, getCurrentRobotLink(w));
        uint start_type = typeFromIndex(512/graph_x_scale, getCurrentRobotLink(w));
        drawCircle(bitmap, stride, start_link.x_0*graph_x_scale, start_link.y_0*graph_y_scale, 10, 0xFF0000);
        drawLineSafe(bitmap, stride, start_link.x_0*graph_x_scale+1, start_link.y_0*graph_y_scale+1, start_link.x_1*graph_x_scale+1, start_link.y_1*graph_y_scale+1, 0xFF0000);
        
        uint next_index = nextGotoIndex(bitmap, stride, w, 512/graph_x_scale, start_link.x_0, start_link.y_0, start_type, 5, 5, 0);
        
        for(int i = 0; i < indexFromLink(512/graph_x_scale, 512/graph_x_scale, 512/graph_y_scale, 0); i++)
        {
            link a = linkFromIndex(512/graph_x_scale, i);
            
            if(inBounds(a.x_0*graph_x_scale, a.y_0*graph_y_scale) && inBounds(a.x_1*graph_x_scale, a.y_1*graph_y_scale))
            {
                drawLine(bitmap, stride, a.x_0*graph_x_scale, a.y_0*graph_y_scale, a.x_1*graph_x_scale, a.y_1*graph_y_scale, (i==next_index)?(0xFFFFFF):(((w->unwalkable[i/32]>>(i%32))&1)?0xFF00FF:0x505050));
            }
            else
            {
                w->unwalkable[i/32] |= 1<<(i%32);
            }
        }
    }

    {
        link start_link = linkFromIndex(512/graph_x_scale, getCurrentRobotLink(w));
;
        uint start_type = typeFromIndex(512/graph_x_scale, getCurrentRobotLink(w));

            for(int i = 0; i < 10; i++)
            {
                uint next_index = nextGotoIndex(bitmap, stride, w, 512/graph_x_scale, start_link.x_0, start_link.y_0, start_type, 5, 5, 0);
                drawLine(bitmap, stride, start_link.x_0*graph_x_scale, start_link.y_0*graph_y_scale, start_link.x_1*graph_x_scale, start_link.y_1*graph_y_scale, 0xFFFFFF);
        
                start_link = linkFromIndex(512/graph_x_scale, next_index);
                start_type = typeFromIndex(512/graph_x_scale, next_index);
            }
    }
    
    for(int x = 0; x < 512; x++)
    {
        for(int y = 0; y < 512; y++)
        {
            if((w->grid[((x/grid_size)+(y/grid_size)*grid_stride)>>4]>>(((x/grid_size)+(y/grid_size)*grid_stride)%16)) & 1)
            {
                bitmap[x+y*stride] = 0x454545;
            }
        }
    }
    
    for(int i = 0; i < len(w->walls); i++)
    {
        drawLine(bitmap, stride, w->walls[i].start, w->walls[i].end, 0x000000);
    }
    drawRectangle(bitmap, 512, 0, 0, 512, 512, 0xFFFFFF);

    uint8 display = US(w);
    for(int d = 0; d < 3; d++)
    {
        fillDigit(bitmap, stride, display%10, 45-20*d, 10, 0x00FF00);
        display /= 10;
    }

    for(int i = 0; i < 3; i++)
    {
        float2 r = {0, base_radius};
        fillRegularNgon(bitmap, stride, 5, w->goals[i], r, 0x0000FF);
    }
    for(int i = 3; i < 6; i++)
    {
        float2 r = {0, base_radius};
        fillRegularNgon(bitmap, stride, 5, w->goals[i], r, 0xFF0000);
    }
    
    if(input.buttons&1)
    {
        w->black_knight.pos = add(w->black_knight.pos, scale(w->black_knight.dir, 100.0*dt));
    }
    if(input.buttons&2)
    {
        w->black_knight.pos = add(w->black_knight.pos, scale(w->black_knight.dir, -100.0*dt));
    }
    if(input.buttons&4)
    {
        float2 rotation = {cos(1.0*dt), -sin(1.0*dt)};
        w->black_knight.dir = normalize(complexx(w->black_knight.dir, rotation));
    }
    if(input.buttons&8)
    {
        float2 rotation = {cos(1.0*dt), sin(1.0*dt)};
        w->black_knight.dir = normalize(complexx(w->black_knight.dir, rotation));
    }

    if(input.buttons&16)
    {
        w->white_knight.pos = add(w->white_knight.pos, scale(w->white_knight.dir, 100.0*dt));
    }
    if(input.buttons&32)
    {
        w->white_knight.pos = add(w->white_knight.pos, scale(w->white_knight.dir, -100.0*dt));
    }
    if(input.buttons&64)
    {
        float2 rotation = {cos(1.0*dt), -sin(1.0*dt)};
        w->white_knight.dir = normalize(complexx(w->white_knight.dir, rotation));
    }
    if(input.buttons&128)
    {
        float2 rotation = {cos(1.0*dt), sin(1.0*dt)};
        w->white_knight.dir = normalize(complexx(w->white_knight.dir, rotation));
    }

    float2 us_pos = add(complexx(us_rel_pos, w->black_knight.dir), w->black_knight.pos);
    fillCircle(bitmap, stride, us_pos.x, us_pos.y, 3, 0xFFFFFF);
    {
        float2 rotation = {cos(1.0*dt), sin(1.0*dt)};
        //w->white_knight.dir = complexx(w->white_knight.dir, rotation);        
    }
    fillRegularNgon(bitmap, stride, 4, w->white_knight.pos, complexx(robot_rel_corner, w->white_knight.dir), 0xFF0000);

    for(int i = 0; i < 6; i++)
    {
        drawCircle(bitmap, stride, w->goals[i].x, w->goals[i].y, tube_radius, 0xFFFFFF);
    }
    
    float2 max_range_point = add(us_pos, scale(w->black_knight.dir, us_max_range));
    float2 hit_point = add(us_pos, scale(w->black_knight.dir, US(w)*inches/2.53));
        
    drawCircleSafe(bitmap, stride, max_range_point.x, max_range_point.y, 3, 0xFF0000);
    drawLineSafe(bitmap, stride, us_pos, max_range_point, 0xFF0000);
    drawCircleSafe(bitmap, stride, hit_point.x, hit_point.y, 3, 0xFFFF00);
    drawLineSafe(bitmap, stride, us_pos, hit_point, 0x00FF00);
    
#if 0 //grid 
    {
        float2 hit_pos = add(us_pos, scale(w->black_knight.dir, min(US(w)*inches/2.53, us_max_range)));
        float xi = hit_pos.x;
        float yi = hit_pos.y;
        float xf = us_pos.x;
        float yf = us_pos.y;
        xi /= grid_size;
        yi /= grid_size;
        xf /= grid_size;
        yf /= grid_size;
        if(abs(xf-xi) > abs(yf-yi))
        {
            if(xf < xi)
            {
                auto swap = xf;
                xf = xi;
                xi = swap;
                swap = yf;
                yf = yi;
                yi = swap;
            }
            for(int x = xi; x < xf; x++)
            {
                int y = round((yf-yi)*(x-xi)/(xf-xi)+yi);
                drawCircleSafe(bitmap, stride, x*grid_size+grid_size/2, y*grid_size+grid_size/2, 3, 0xFFFFFF);
                if(x >= 0 && y >= 0 && x < grid_stride && y < grid_stride) w->grid[(x+y*grid_stride)>>4] &= ~(1<<((x+y*grid_stride)%16));
            }
        }
        else
        {
            if(yf < yi)
            {
                auto swap = xf;
                xf = xi;
                xi = swap;
                swap = yf;
                yf = yi;
                yi = swap;
            }
            for(int y = yi; y < yf; y++)
            {
                int x = round((xf-xi)*(y-yi)/(yf-yi)+xi);
                drawCircleSafe(bitmap, stride, x*grid_size+grid_size/2, y*grid_size+grid_size/2, 3, 0xFFFFFF);
                if(x >= 0 && y >= 0 && x < grid_stride && y < grid_stride) w->grid[(x+y*grid_stride)>>4] &= ~(1<<((x+y*grid_stride)%16));
            }
        }
        /* xf = (hit_pos.x)/grid_size; */
        /* yf = (hit_pos.y)/grid_size; */
        /* if(US(w) < us_max_range) */
        /* { */
        /*     if(xf >= 0 && yf >= 0 && xf < grid_stride && yf < grid_stride) w->grid[(xf+yf*grid_stride)>>4] |= (1<<((xf+yf*grid_stride)%16)); */
        /* } */
    }

    {
        float2 hit_pos = add(us_pos, scale(w->black_knight.dir, min(US(w)*inches/2.53, us_max_range)));
        
        int xi = hit_pos.x;
        int yi = hit_pos.y;
        int xf = max_range_point.x;
        int yf = max_range_point.y;
        xi /= grid_size;
        yi /= grid_size;
        xf /= grid_size;
        yf /= grid_size;
        if(abs(xf-xi) > abs(yf-yi))
        {
            if(xf < xi)
            {
                auto swap = xf;
                xf = xi;
                xi = swap;
                swap = yf;
                yf = yi;
                yi = swap;
            }
            for(int x = xi; x < xf; x++)
            {
                int y = round((float)(yf-yi)*(x-xi)/(xf-xi)+yi);
                drawCircleSafe(bitmap, stride, x*grid_size+grid_size/2, y*grid_size+grid_size/2, 3, 0x000000);
                if(x >= 0 && y >= 0 && x < grid_stride && y < grid_stride) w->grid[(x+y*grid_stride)>>4] |= (1<<((x+y*grid_stride)%16));
            }
        }
        else
        {
            if(yf < yi)
            {
                auto swap = xf;
                xf = xi;
                xi = swap;
                swap = yf;
                yf = yi;
                yi = swap;
            }
            for(int y = yi; y < yf; y++)
            {
                int x = round((float)(xf-xi)*(y-yi)/(yf-yi)+xi);
                drawCircleSafe(bitmap, stride, x*grid_size+grid_size/2, y*grid_size+grid_size/2, 3, 0x000000);
                if(x >= 0 && y >= 0 && x < grid_stride && y < grid_stride) w->grid[(x+y*grid_stride)>>4] |= (1<<((x+y*grid_stride)%16));
            }
        }
    }
#endif

    /////////////////////points
    {
        float2 hit_pos = add(us_pos, scale(w->black_knight.dir, US(w)*inches/2.53));
        
        if(US(w)*inches/2.53 <= us_max_range && w->n_points < n_max_points)
        {
            w->points[w->n_points].pos = hit_pos;
            w->points[w->n_points].n_neighbors = 0;
            w->points[w->n_points].neighbors[0] = -1;

            int8 to_link[max_neighbors]; 
            uint n_to_link = 0;
            
            for(int i = w->n_points-1; i >= 0; i--) //the more recent points are more likely to be close
            {
                if(dotme(sub(w->points[w->n_points].pos, w->points[i].pos)) < sq(min_neighbor_radius))
                {
                    goto dont_add;
                }
                if(dotme(sub(w->points[w->n_points].pos, w->points[i].pos)) < sq(max_neighbor_radius))
                {
                    if(n_to_link != max_neighbors)
                    {
                        to_link[n_to_link++] = i;
                    }
                }
            }
            for(int i = 0; i < n_to_link; i++)
            {
                if(w->points[to_link[i]].n_neighbors < max_neighbors && w->points[w->n_points].n_neighbors < max_neighbors)
                {
                    cutOffGraph(w, 512/graph_x_scale, w->points[w->n_points].pos.x, w->points[w->n_points].pos.y);
                    cutOffGraph(w, 512/graph_x_scale, w->points[to_link[i]].pos.x, w->points[to_link[i]].pos.y);

                    
                    w->points[to_link[i]].neighbors[w->points[to_link[i]].n_neighbors++] = w->n_points;
                    w->points[w->n_points].neighbors[w->points[w->n_points].n_neighbors++] = to_link[i];
                }
            }

            w->n_points++;
        dont_add:;
        }
        
        for(int i = w->n_points-1; i >= 0; i--) //itterate backwards for swap remove
        {
            for(int n = 0; n < w->points[i].n_neighbors; n++)
            {
                drawLineSafe(bitmap, stride, w->points[i].pos, w->points[w->points[i].neighbors[n]].pos, 0xFFFFFF);
            }
            drawCircleSafe(bitmap, stride, w->points[i].pos.x, w->points[i].pos.y, 2, 0xFFFFFF);
            drawCircleSafe(bitmap, stride, w->points[i].pos.x, w->points[i].pos.y, min_neighbor_radius, 0x777777);
            drawCircleSafe(bitmap, stride, w->points[i].pos.x, w->points[i].pos.y, 9*inches, 0xFF00FF);
            
            #if 0
            for(int n = w->points[i].n_neighbors-1; n >= 0; n--)
            {
                float2 rel_start = sub(w->points[i].pos, us_pos);
                float2 rel_end = sub(w->points[w->points[i].neighbors[n]].pos, us_pos);
                float y_0 = dot(rel_start, w->black_knight.dir);
                float y_1 = dot(rel_end, w->black_knight.dir);
                float x_0 = dot(rel_start, perp(w->black_knight.dir));
                float x_1 = dot(rel_end, perp(w->black_knight.dir));
                
                float d = (y_1*abs(x_0)+y_0*abs(x_1))/(abs(x_1)+abs(x_0));

                if((US(w) != 255 || abs(dot(normalize(sub(w->points[i].pos, w->points[w->points[i].neighbors[n]].pos)), w->black_knight.dir)) < max_detection_cos) && min(US(w)*inches/2.53, us_max_range) > d+max_neighbor_radius/2.0+1
                   && d > 0
                   && x_0*x_1 < 0) //if seeing through where a wall should be, then remove the wall
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
            #endif
        }
    }
}

#endif
