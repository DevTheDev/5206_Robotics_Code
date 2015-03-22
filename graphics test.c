#pragma DebuggerWindows("nxtLCDScreen")

#define pi 3.14159265358979323846264338327950

#define len(a) sizeof(a)/sizeof(a[0])
#define ever (;;)

float theta = 0.0;
float scale = 10;
float x_off = 48;
float y_off = 60;
float y_cam = -1.2;
float persp_off = 3.0;
float time = 0;
float sine = 0.0;
float cosine = 1.0;

void drawHorizontalLines(float x0, float x1, float y, float z)
{
    for(int r = 0; r < 4; r++)
    {
        if(r==2)
        {
            y = -y;
        }
        
        float xy0 = ((r&1)?x0:y);
        float xy1 = ((r&1)?x1:y);
        float yx0 = ((r&1)?y:x0);
        float yx1 = ((r&1)?y:x1);

        float persp0 = persp_off -cosine*xy0-sine*yx0;
        float persp1 = persp_off -cosine*xy1-sine*yx1;

        nxtDrawLine((scale*persp0*(sine*xy0-cosine*yx0) +x_off), (scale*persp0*(z+y_cam) + y_off),
                    (scale*persp1*(sine*xy1-cosine*yx1) +x_off), (scale*persp1*(z+y_cam) + y_off));
    }
}

void drawVerticalLines(float x, float y, float z0, float z1)
{
    for(int r = 0; r < 4; r++)
    {
        float temp = x;
        x = -y;
        y = temp;
        
        float persp = persp_off -cosine*x-sine*y;

        nxtDrawLine((scale*persp*(sine*x-cosine*y) +x_off), (scale*persp*(z0+y_cam) + y_off),
                    (scale*persp*(sine*x-cosine*y) +x_off), (scale*persp*(z1+y_cam) + y_off));
    }

}

task main()
{
    //pixel by pixel test, way too slow
    /* clearTimer(T1); */
    /* for ever */
    /* { */
    /*     for(int x = 0; x < 100; x++) */
    /*     { */
    /*         for(int y = 0; y < 32; y++) */
    /*         { */
    /*             if((x*y +time1[T1]/16) % 100 <= 10) */
    /*             { */
    /*                 nxtSetPixel(x, y); */
    /*             } */
    /*             else */
    /*             { */
    /*                 nxtClearPixel(x, y); */
    /*             } */
    /*         } */
    /*     } */
    /* } */

    clearTimer(T1);

    eraseDisplay();

    for ever
    {
        wait1MSec(20);
        eraseDisplay();
        time = time1[T1];
        theta = 0.0005*time1[T1];
        theta -= floor(theta/(pi*2.0))*pi*2.0;
        sine   = sin(theta);
        cosine = cos(theta);

        drawHorizontalLines(-0.5, 0.5, 0.5, 1.0);
        //drawHorizontalLines(-1.0, 1.0, 0.5, 0.5); //weird precision issues
        drawHorizontalLines(-0.5, 0.5, 0.5, 0.5);
        drawHorizontalLines(0.5, 1.0, 0.5, 0.5);
        drawHorizontalLines(-0.5, -1.0, 0.5, 0.5);
        drawHorizontalLines(-0.5, 0.5, 1.0, 0.5);
        drawHorizontalLines(-0.5, 0.5, 1.0, 0.0);
        //drawHorizontalLines(-1.0, 1.0, 0.5, 0.0);
        //drawHorizontalLines(-0.5, 0.5, 0.5, 0.0);
        drawHorizontalLines(0.5, 1.0, 0.5, 0.0);
        drawHorizontalLines(-0.5, -1.0, 0.5, 0.0);

        drawVerticalLines(0.5, 0.5, 1.0, 0.0);
        drawVerticalLines(1.0, 0.5, 0.0, 0.5);
        drawVerticalLines(0.5, 1.0, 0.0, 0.5);

        //stump
        nxtDrawEllipse(scale*persp_off*(-0.5) +x_off, scale*(persp_off-0.5)*(0.0+y_cam)+y_off,
                       scale*persp_off*(0.5) +x_off, scale*(persp_off+0.5)*(0.0+y_cam)+y_off);
        nxtDrawLine(scale*persp_off*(-0.5) +x_off, scale*persp_off*(0.0+y_cam)+y_off,
                    scale*persp_off*(-0.5) +x_off, scale*persp_off*(-0.5+y_cam)+y_off);
        nxtDrawLine(scale*persp_off*(0.5) +x_off, scale*persp_off*(0.0+y_cam)+y_off,
                    scale*persp_off*(0.5) +x_off, scale*persp_off*(-0.5+y_cam)+y_off);
        nxtDrawEllipse(scale*persp_off*(-0.5) +x_off, scale*(persp_off-0.5)*(-0.5+y_cam)+y_off,
                       scale*persp_off*(0.5) +x_off, scale*(persp_off+0.5)*(-0.5+y_cam)+y_off);
    }
}
