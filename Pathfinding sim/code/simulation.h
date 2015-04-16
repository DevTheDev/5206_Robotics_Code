#ifndef SIMULATION
#define SIMULATION

#include "meth.h"
#include "misc.h"

struct memory_pool
{
    size_t size;
    void * start;
};

uint32 sphereLambertian(int x0, int y0, int x, int y)
{
    float xc = x0-250;
    float yc = y0-250;
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

uint32 flatLambertian(int x0, int y0, int x, int y)
{
    float xc = x0+x-250;
    float yc = y0+y-250;
    float normsq = xc*xc + yc*yc + 1000.0;
    float invnorm = invsqrt(normsq);
    float lx = xc*invnorm;
    float ly = yc*invnorm;
    float lambertian = 100000.0*10.0*invnorm;
    int brightness = min(sqrt(lambertian), 255);
    return brightness<<16|brightness<<8|brightness;
}

void drawLine(uint32 * bitmap, uint stride, int x0, int y0, int x1, int y1, uint32 color)
{
    if(abs(x1-x0) > abs(y1-y0))
    {
        if(x0 > x1)
        {
            auto swapper = x1;
            x1 = x0;
            x0 = swapper;
            swapper = y1;
            y1 = y0;
            y0 = swapper;
        }
    
        int m = (y1-y0);
        for(int x = x0; x < x1; x++)
        {
            int y = m*(x-x0)/(x1-x0)+y0;
            bitmap[x+y*stride] = color;
        }
    }
    else
    {
        if(y0 > y1)
        {
            auto swapper = x1;
            x1 = x0;
            x0 = swapper;
            swapper = y1;
            y1 = y0;
            y0 = swapper;
        }
        int m = (x1-x0);
        for(int y = y0; y < y1; y++)
        {
            int x = m*(y-y0)/(y1-y0)+x0;
            bitmap[x+y*stride] = color;
        }
    }
}

void drawLine(uint32 * bitmap, uint stride, float2 p0, float2 p1, uint32 color)
{
    drawLine(bitmap, stride, p0.x, p0.y, p1.x, p1.y, color);
}

void drawLineSafe(uint32 * bitmap, uint stride, int x0, int y0, int x1, int y1, uint32 color)
{
    if(abs(x1-x0) > abs(y1-y0))
    {
        if(x0 > x1)
        {
            auto swapper = x1;
            x1 = x0;
            x0 = swapper;
            swapper = y1;
            y1 = y0;
            y0 = swapper;
        }
    
        int m = (y1-y0);
        for(int x = x0; x < x1; x++)
        {
            int y = m*(x-x0)/(x1-x0)+y0;
            if(x >= 0 && x < stride && y >= 0 && y <= 512)
            {
                bitmap[x+y*stride] = color;
            }
        }
    }
    else
    {
        if(y0 > y1)
        {
            auto swapper = x1;
            x1 = x0;
            x0 = swapper;
            swapper = y1;
            y1 = y0;
            y0 = swapper;
        }
        int m = (x1-x0);
        for(int y = y0; y < y1; y++)
        {
            int x = m*(y-y0)/(y1-y0)+x0;
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


void drawCircleSafe(uint32 * bitmap, uint stride, int x0, int y0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        if(x0+x > 0 && x0+x < 512 && y0+y > 0 && y0+y < 512) bitmap[x0+x+stride*(y0+y)] = color;
        if(x0+y > 0 && x0+y < 512 && y0+x > 0 && y0+x < 512) bitmap[x0+y+stride*(y0+x)] = color;
        if(x0-x > 0 && x0-x < 512 && y0-y > 0 && y0-y < 512) bitmap[x0-x+stride*(y0-y)] = color;
        if(x0-y > 0 && x0-y < 512 && y0-x > 0 && y0-x < 512) bitmap[x0-y+stride*(y0-x)] = color;
        if(x0+x > 0 && x0+x < 512 && y0-y > 0 && y0-y < 512) bitmap[x0+x+stride*(y0-y)] = color;
        if(x0+y > 0 && x0+y < 512 && y0-x > 0 && y0-x < 512) bitmap[x0+y+stride*(y0-x)] = color;
        if(x0-x > 0 && x0-x < 512 && y0+y > 0 && y0+y < 512) bitmap[x0-x+stride*(y0+y)] = color;
        if(x0-y > 0 && x0-y < 512 && y0+x > 0 && y0+x < 512) bitmap[x0-y+stride*(y0+x)] = color;
                
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

void drawCircle(uint32 * bitmap, uint stride, int x0, int y0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        bitmap[x0+x+stride*(y0+y)] = color;
        bitmap[x0+y+stride*(y0+x)] = color;
        bitmap[x0-x+stride*(y0-y)] = color;
        bitmap[x0-y+stride*(y0-x)] = color;
        bitmap[x0+x+stride*(y0-y)] = color;
        bitmap[x0+y+stride*(y0-x)] = color;
        bitmap[x0-x+stride*(y0+y)] = color;
        bitmap[x0-y+stride*(y0+x)] = color;
                
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

void fillCircle(uint32 * bitmap, uint stride, int x0, int y0, int r, uint32 color)
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        for(int i = x0-x+stride*(y0-y); i <= x0+x+stride*(y0-y); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x0-y+stride*(y0-x); i <= x0+y+stride*(y0-x); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x0-x+stride*(y0+y); i <= x0+x+stride*(y0+y); i++)
        {
            bitmap[i] = color;
        }
        for(int i = x0-y+stride*(y0+x); i <= x0+y+stride*(y0+x); i++)
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

void shadeCircle(uint32 * bitmap, uint stride, int x0, int y0, int r, uint32(*color)(int x0, int y0, int x, int y))
{
    
    int x = r;
    int y = 0;
    int err = 1-x;
    while(x >= y)
    {
        for(int i = x0-y+stride*(y0-x); i <= x0+y+stride*(y0-x); i++)
        {
            bitmap[i] = color(x0, y0, i-x0-stride*(y0-x), -x);
        }
        for(int i = x0-x+stride*(y0-y); i <= x0+x+stride*(y0-y); i++)
        {
            bitmap[i] = color(x0, y0, i-x0-stride*(y0-y), -y);
        }
        for(int i = x0-x+stride*(y0+y); i <= x0+x+stride*(y0+y); i++)
        {
            bitmap[i] = color(x0, y0, i-x0-stride*(y0+y), y);
        }
        for(int i = x0-y+stride*(y0+x); i <= x0+y+stride*(y0+x); i++)
        {
            bitmap[i] = color(x0, y0, i-x0-stride*(y0+x), x);
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


void drawRectangle(uint32 * bitmap, uint stride, int x0, int y0, int w, int h, uint32 color)
{
    int i = x0+y0*stride;
    for(; i < x0+y0*stride + w; i++)
    {
        bitmap[i] = color;
    }
    for(i = x0+(y0+1)*stride; i < x0+(y0+h-1)*stride; i += stride)
    {
        bitmap[i] = color;
        bitmap[i+w-1] = color;
    }
    for(; i < x0+(y0+h-1)*stride + w; i++)
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

void fillDigit(uint32 * bitmap, uint stride, uint n, int x0, int y0, uint32 color)
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
                bitmap[x+x0+(y+y0)*stride] = color;
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
};

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
    
    w->n_points = 0;
}


const float2 robot_rel_corner = {9*inches, 9*inches};

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
        float y0 = dot(rel_start, w->black_knight.dir);
        float y1 = dot(rel_end, w->black_knight.dir);
        float x0 = dot(rel_start, perp(w->black_knight.dir));
        float x1 = dot(rel_end, perp(w->black_knight.dir));
        if(x0*x1 < 0 && abs(dot(normalize(sub(w->walls[i].start, w->walls[i].end)), w->black_knight.dir)) < max_detection_cos)
        {
            float new_distance = (y1*abs(x0)+y0*abs(x1))/(abs(x1)+abs(x0));

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
        float y0 = dot(rel_start, w->black_knight.dir);
        float y1 = dot(rel_end, w->black_knight.dir);
        float x0 = dot(rel_start, perp(w->black_knight.dir));
        float x1 = dot(rel_end, perp(w->black_knight.dir));
        if(x0*x1 < 0 && abs(dot(normalize(sub(corner1, corner0)), w->black_knight.dir)) < max_detection_cos)
        {
            float new_distance = (y1*abs(x0)+y0*abs(x1))/(abs(x1)+abs(x0));

            if(new_distance > 0 && new_distance < us_max_range)
            {
                distance = min(new_distance, distance);
            }
        }
    }
    
    return min(distance/(inches)*2.53, 255);
}

void simulateAndRender(uint32 * bitmap, uint stride, world * w, float dt, user_input input)
{
    memset(bitmap, 0x626262, 512*512*sizeof(uint32));

    fillNgon(bitmap, stride, 6, w->center_hexagon, 0xFFFF00);
    
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
    
    fillRegularNgon(bitmap, stride, 4, w->black_knight.pos, complexx(robot_rel_corner, w->black_knight.dir), 0x0000FF);

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

            #if 0
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
            #endif
        }
    }
}

#endif
