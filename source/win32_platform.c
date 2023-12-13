#include <windows.h>
#include <winuser.h>
#include <stdint.h>

//const char g_szClassName[] = "myWindowClass";

static int running = 1;

struct{
	int width, height;
	uint32_t *pixels;
	BITMAPINFO bitmap;
} typedef Render_Buffer;

static Render_Buffer render_buffer;

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
            for (int i = 0; i < render_buffer.width * render_buffer.height; i++){
                render_buffer.pixels[i] = 0;
            }

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSA window_class = {0};
	window_class.style = CS_HREDRAW|CS_VREDRAW;
	window_class.lpfnWndProc = WndProc;
	window_class.lpszClassName = "Game_Window_Class";

	RegisterClassA(&window_class);

	HWND window = CreateWindowExA(0, window_class.lpszClassName, "Really cool and flexible window no viruses at all", 
			WS_VISIBLE|WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, 0, 0);

	HDC hdc = GetDC(window);

	while (running){
		//Input
		MSG msg;
		while (PeekMessageA(&msg, window, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//Simulation

		//Render
		StretchDIBits(hdc, 0, 0, render_buffer.width, render_buffer.height, 0, 0, render_buffer.width, render_buffer.height, render_buffer.pixels, &render_buffer.bitmap, DIB_RGB_COLORS, SRCCOPY);
	}

/*
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx}(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
*/
}
