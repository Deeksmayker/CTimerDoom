#include <windows.h>
#include <winuser.h>
#include <stdint.h>
//#include <math.h>
#include <time.h>
#include <stdio.h>

//const char g_szClassName[] = "myWindowClass";

#define u32 uint32_t

static int running = 1;

struct{
	int width, height;
	u32 *pixels;
	BITMAPINFO bitmap;
} typedef Render_Buffer;

static Render_Buffer render_buffer;

struct {
    int vertical;
} typedef Input;

static Input input;

#include "rendering.c"

#include "timer.c"

#include "pong.c"

LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
            switch(wParam){
                case VK_RIGHT:
                    break;
                case VK_LEFT:
                    break;
                case VK_UP:
                    input.vertical = 1;
                    break;
                case VK_DOWN:
                    input.vertical = -1;
                    break;
                case VK_SPACE:
                    break;
                default:
                    break;
            }
            break;
        case WM_KEYUP:
            switch(wParam){
                case VK_RIGHT:
                    break;
                case VK_LEFT:
                    break;
                case VK_UP:
                    if (input.vertical == 1)  input.vertical = 0;
                    break;
                case VK_DOWN:
                    if (input.vertical == -1) input.vertical = 0;
                    break;
                case VK_SPACE:
                    break;
                default:
                    break;
            }
            break;
		case WM_CLOSE:
			DestroyWindow(window);
			running = 0;
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			running = 0;
			break;
		case WM_SIZE: {
			RECT rect;
			GetWindowRect(window, &rect);
			if (rect.right < 0){
			    break;
			}
			clamp_int(&rect.left, 0, render_buffer.width-1);
			render_buffer.width = rect.right - rect.left;
			render_buffer.height = rect.bottom - rect.top;

            if (render_buffer.pixels){
                free(render_buffer.pixels);
            }

            render_buffer.pixels = malloc(sizeof(u32)*render_buffer.width * render_buffer.height);
            // for (int i = 0; i < render_buffer.width * render_buffer.height; i++){
            //     render_buffer.pixels[i] = 0;
            // }

            render_buffer.bitmap.bmiHeader.biSize = sizeof(render_buffer.bitmap.bmiHeader);
            render_buffer.bitmap.bmiHeader.biWidth = render_buffer.width;
            render_buffer.bitmap.bmiHeader.biHeight = render_buffer.height;
            render_buffer.bitmap.bmiHeader.biPlanes = 1;
            render_buffer.bitmap.bmiHeader.biBitCount = 32;
            render_buffer.bitmap.bmiHeader.biCompression = BI_RGB;
            // render_buffer.bitmap.bmiHeader.biSizeImage = 0;
            // render_buffer.bitmap.bmiHeader.biXPelsPerMeter = 0;
            // render_buffer.bitmap.bmiHeader.biYPelsPerMeter = 0;
            // render_buffer.bitmap.bmiHeader.biClrUsed = 0;
            // render_buffer.bitmap.bmiHeader.biClrImportant = 0;
			break;
		}
		default:
			return DefWindowProcA(window, msg, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = WndProc;
    window_class.lpszClassName = "Game_Window_Class";

    RegisterClassA(&window_class);

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Reeal pong", WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);

    HDC hdc = GetDC(window);

/*
    static clock_t old_clock;
    old_clock = clock();
    clock_ticks = clock();
*/  
    //AllocConsole();
    AttachConsole(ATTACH_PARENT_PROCESS);
    //for printf to work
    freopen("CONOUT$", "w", stdout);
    
    //Pong init
    Pong *pong_game = init_pong();
    
    while (running){
        //Input
        MSG msg;
        while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)){
            switch (msg.message){
                default:
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
            } 
        }
        


        //Simulation
        //time stuff
        game_time = (float)clock() / CLOCKS_PER_SEC;
        delta_time = (game_time - previous_game_time);

        if (timer_running){
            timer_value += delta_time;
        } else{
            idle_timer_time += delta_time;
        }
        
        
        
        //Render
        u32 *colors = malloc(3 * sizeof(int));
        colors[0] = 0xff2222;
        colors[1] = 0x2277ff;
        colors[2] = 0xffffff;
        clear_screen_gradient(colors, 3);
        free(colors);
        //clear_screen_three_color(0xf06553, 0xffffff, 0x1b85b8);
        
        update_pong(pong_game);

        //draw_text("0.123:456789", (Vector2_int){500, 500}, 5, 0xffffff);

        int hours   = (int)timer_value / 60 / 60;
        int minutes = (int)timer_value / 60;
        int seconds = (int)timer_value % 60;
        
        Vector2_int time_position = {render_buffer.width/2, render_buffer.height/2};
        draw_time(hours, minutes, seconds, time_position, 7, 0xffffff); 

/*
        char timeArray[20];
        sprintf(timeArray, "%.3f", timer_value);
        draw_text(timeArray, (Vector2_int){100, 100}, 5, 0xffffff);
*/

        
        StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap, DIB_RGB_COLORS, SRCCOPY);


        //Frame End
        //clock_ticks = clock();
        //old_clock = clock();

        previous_game_time = game_time;

        //Sleep(20);
    }
}
