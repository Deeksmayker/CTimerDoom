#include <windows.h>
#include <winuser.h>
#include <stdint.h>
#include<math.h>

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

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
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

int time = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSA window_class = {0};
    window_class.style = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc = WndProc;
    window_class.lpszClassName = "Game_Window_Class";

    RegisterClassA(&window_class);

    HWND window = CreateWindowExA(0, window_class.lpszClassName, "Really cool and flexible window no viruses at all", WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);

    HDC hdc = GetDC(window);

    while (running){
        //Input
        MSG msg;
        while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)){
            switch (msg.message){
                // case WM_KEYDOWN:
                //     running = 0;
                //     break;
                // case WM_KEYUP:
                //     running = 0;
                //     break;
                default:
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

            } 

        }

        time++;

        //Simulation
        clear_screen_gradient(0x1b85b8, 0xffffff);
        draw_rect((int_vector2){500, 250}, (int_vector2){500, 250}, 0x551100);
        draw_text("0.123:456789", 12, (int_vector2){500, 500}, 5, 0xffffff);
        draw_text(int_to_string(time), log10(time)+1, (int_vector2){500, 300}, 6, 0xffff33);
        int lerp_value = lerp_int(0, 10000, (float)time/10000);
        draw_text(int_to_string(lerp_value), log10(lerp_value) + 1, (int_vector2){300, 300}, 5, 0xffffff);



		//Render
		StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap, DIB_RGB_COLORS, SRCCOPY);
	}
}

char* int_to_string(int number){
    int size = log10(number) + 1;

    char* result = malloc(sizeof(char) * size);
    for (int i = size-1; i >= 0; i--){
        result[i] = (number%10) + '0';
        number/=10;
    }

    return result;
}
