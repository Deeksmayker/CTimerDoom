#include <windows.h>
#include <winuser.h>
#include <stdint.h>
#include<math.h>
#include <time.h>
#include <stdio.h>

//const char g_szClassName[] = "myWindowClass";

static int running = 1;

struct{
	int width, height;
	uint32_t *pixels;
	BITMAPINFO bitmap;
} typedef Render_Buffer;

static Render_Buffer render_buffer;

#include "rendering.c"

char *int_to_string(int number);

int_vector2 timer_pos = {300, 300};
int timer_move_speed = 5;

static clock_t clock_ticks;
float game_time = 0;
int timer_running = 1;
float idle_timer_time = 0;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_KEYDOWN:
            switch(wParam){
                case VK_RIGHT:
                    timer_pos.x += timer_move_speed;
                    break;
                case VK_LEFT:
                    timer_pos.x -= timer_move_speed;
                    break;
                case VK_UP:
                    timer_pos.y += timer_move_speed;
                    break;
                case VK_DOWN:
                    timer_pos.y -= timer_move_speed;
                    break;
                case VK_SPACE:
                    timer_running = !timer_running;
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
			render_buffer.width = rect.right - rect.left;
			render_buffer.height = rect.bottom - rect.top;

            if (render_buffer.pixels){
                free(render_buffer.pixels);
            }

            render_buffer.pixels = malloc(sizeof(uint32_t)*render_buffer.width * render_buffer.height);
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

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "COOL AND GREAT real timer no shit and no viruses at all", WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);

    HDC hdc = GetDC(window);

    static clock_t old_clock;
    old_clock = clock();
    clock_ticks = clock();

    while (running){
        //Input
        MSG msg;
        while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)){
            switch (msg.message){
                // case WM_KEYUP:
                //     running = 0;
                //     break;
                default:
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

            } 

        }

        //game_time += ((float)(clock() - clock_ticks)) / CLOCKS_PER_SEC;

        // static int delta_time = 0;
        // //delta_time = ((float)clock() - (float)old_clock) / CLOCKS_PER_SEC;
        // delta_time = clock() - old_clock;
        
        static float previous_game_time = 0;

        game_time = (float)clock() / CLOCKS_PER_SEC;

        static float timer_value = 0;
        static float dt = 0;


        dt = (game_time - previous_game_time);

        if (timer_running){
            timer_value += dt;
        } else{
            idle_timer_time += dt;
        }

        //Simulation
        

        clear_screen_gradient(0x1b85b8, 0xffffff);

        draw_text("0.123:456789", (int_vector2){500, 500}, 5, 0xffffff);

        char timeArray[20];
        sprintf(timeArray, "%.3f", timer_value);
        draw_text(timeArray, timer_pos, 7, 0xffffff);

        //Render
        StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap, DIB_RGB_COLORS, SRCCOPY);


        //Frame End
        clock_ticks = clock();
        old_clock = clock();

        previous_game_time = game_time;
    }
}

// char* int_to_string(int number){
//     if (number == 0) return "0";
//     int size = log10(number) + 1;
//
//     char* result = malloc(sizeof(char) * size);
//     for (int i = size-1; i >= 0; i--){
//         result[i] = (number%10) + '0';
//         number/=10;
//     }
//
//     return result;
// }
