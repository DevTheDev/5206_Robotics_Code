#include <windows.h>
#include <windowsx.h>

#include "misc.h"
#include "simulation.h"

WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

void fullscreen(HWND hwnd)
{
    DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);
    if (dwStyle & WS_OVERLAPPEDWINDOW) {
        MONITORINFO mi = { sizeof(mi) };
        if (GetWindowPlacement(hwnd, &g_wpPrev) &&
            GetMonitorInfo(MonitorFromWindow(hwnd,
                                             MONITOR_DEFAULTTOPRIMARY), &mi)) {
            SetWindowLong(hwnd, GWL_STYLE,
                          dwStyle & ~WS_OVERLAPPEDWINDOW);
            SetWindowPos(hwnd, HWND_TOP,
                         mi.rcMonitor.left, mi.rcMonitor.top,
                         mi.rcMonitor.right - mi.rcMonitor.left,
                         mi.rcMonitor.bottom - mi.rcMonitor.top,
                         SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    } else {
        SetWindowLong(hwnd, GWL_STYLE,
                      dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(hwnd, &g_wpPrev);
        SetWindowPos(hwnd, NULL, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                     SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{    
    switch(msg)
    {
        case WM_SIZE:
            RedrawWindow(hwnd, 0, 0, RDW_INVALIDATE);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            auto dc = BeginPaint(hwnd, &ps);
            PatBlt(dc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom, BLACKNESS);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{    
    WNDCLASSEX wc;
    {//init the window class
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon(0, IDI_APPLICATION);
        wc.hCursor = LoadCursor(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.lpszMenuName = 0;
        wc.lpszClassName = "class name";
        wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

        if(!RegisterClassEx(&wc))
        {
            MessageBox(0, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }
    }
    
    HWND hwnd;
    {//init hwnd
        hwnd = CreateWindowEx(
            WS_EX_APPWINDOW,//extended window style
            "class name",//the class name
            "Pathfinding Sim",//The title
            WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,//window style
            CW_USEDEFAULT, CW_USEDEFAULT,//position
            550, 550,//size
            0,//handle to the parent window, this has no parents
            0,//menu handle
            hInstance,//duh
            0//lparam
            );
        if(hwnd == 0)
        {
            MessageBox(0, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
            return 0;
        }
    }
    
    memory_pool memory;
    memory.size = 1*gigabyte;
    memory.start = VirtualAlloc(0, memory.size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    
    LARGE_INTEGER timer_frequency;
    LARGE_INTEGER last_time = {0};
    LARGE_INTEGER this_time = {0};
    
    QueryPerformanceFrequency(&timer_frequency);    
    
    MSG msg;
        
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

//    float2 vert = {60, 0};

    world w;
    resetField(&w, 1);

    user_input input = {0};
    
    do
    {//main loop        
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            switch(msg.message)
            {
                case WM_KEYDOWN:
                    switch(msg.wParam)
                    {
                        case '1':
                            resetField(&w, 1);
                            break;
                        case '2':
                            resetField(&w, 2);
                            break;
                        case '3':
                            resetField(&w, 3);
                            break;
                        case 'W':
                            input.buttons |= 1;
                            break;
                        case 'S':
                            input.buttons |= 2;
                            break;
                        case 'A':
                            input.buttons |= 4;
                            break;
                        case 'D':
                            input.buttons |= 8;
                            break;
                        case VK_UP:
                            input.buttons |= 16;
                            break;
                        case VK_DOWN:
                            input.buttons |= 32;
                            break;
                        case VK_LEFT:
                            input.buttons |= 64;
                            break;
                        case VK_RIGHT:
                            input.buttons |= 128;
                            break;
                        case 'F':
                            fullscreen(hwnd);
                            break;
                        case VK_ESCAPE:
                            return 0;
                            break;
                    }
                    break;
                case WM_KEYUP:
                    switch(msg.wParam)
                    {
                        case 'W':
                            input.buttons &= ~1;
                            break;
                        case 'S':
                            input.buttons &= ~2;
                            break;
                        case 'A':
                            input.buttons &= ~4;
                            break;
                        case 'D':
                            input.buttons &= ~8;
                            break;
                        case VK_UP:
                            input.buttons &= ~16;
                            break;
                        case VK_DOWN:
                            input.buttons &= ~32;
                            break;
                        case VK_LEFT:
                            input.buttons &= ~64;
                            break;
                        case VK_RIGHT:
                            input.buttons &= ~128;
                            break;
                    }
                    break;
                default:
                    DispatchMessage(&msg);
            }
        }

        uint32 * bitmap = (uint32*)memory.start;

        // static int c_x = 100;
        // static int c_y = 100;
        // if(GetKeyState(VK_RBUTTON) & 0x100)
        // {
        //     POINT cursor;
        //     GetCursorPos(&cursor);

        //     ScreenToClient(hwnd, &cursor);
                
        //     c_x = cursor.x;
        //     c_y = cursor.y;
        // }

        
        // fillCircle(bitmap, 512, 100, 100, 10, 0xEEFF33);
        // shadeCircle(bitmap, 512, c_x, c_y, 100, flatLambertian);
        // float2 poly[] = {
        //     100.0, 100.0,
        //     50.0, 150.0,
        //     100.0, 300.0,
        //     200.0, 200.0,
        // };

        // fillNgon(bitmap, 512, len(poly), poly, 0x00FF00);

        // float2 poly1[] = {
        //     100.0+200, 100.0,
        //     50.0+200, 150.0,
        //     100.0+200, 300.0,
        //     200.0+200, 200.0,
        // };

        // drawNgon(bitmap, 512, len(poly1), poly, 0xFF0000);

        // float2 center = {400, 400};

        // if(GetKeyState(VK_LBUTTON) & 0x100)
        // {
        //     POINT cursor;
        //     GetCursorPos(&cursor);

        //     ScreenToClient(hwnd, &cursor);
                
        //     vert.x = cursor.x-center.x;
        //     vert.y = cursor.y-center.y;
        // }
        
        // fillRegularNgon(bitmap, 512, 3, center, vert, 0x0000FF);

        simulateAndRender(bitmap, 512, &w, 1.0/120.0, input);
        
        {
            HDC dc = GetDC(hwnd);

            int w = 512;
            int h = 512;

            BITMAPINFO bmi;
            bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            RECT window_rect;
            GetClientRect(hwnd, &window_rect);
            
            StretchDIBits(dc,
                          (window_rect.right-window_rect.left-w)/2, (window_rect.bottom-window_rect.top-h)/2,
                          w, h, //destination
                          0, 0, w, h, //source
                          bitmap,
                          &bmi,
                          DIB_RGB_COLORS,
                          SRCCOPY);
        }

        do
        {
            QueryPerformanceCounter(&this_time);
        }
        while(this_time.QuadPart - last_time.QuadPart < 1.0/120.0*timer_frequency.QuadPart);
        last_time = this_time;
    }
    while(msg.message != WM_QUIT);

    return 0;
}
