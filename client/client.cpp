#include <Windows.h>
#include <iostream>
#include <fstream>

#define create_window_redacted(klass, title) CreateWindowA(klass, title, 0, 0, 0, 0, 0, 0, 0, 0, 0);
LRESULT CALLBACK hidden_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct data {
    char data[255];
} s_data;

int main() {

    WNDCLASS wc = { };

    LPCSTR class_name = "SharedMemoryHidden";
    LPCSTR window_title = "MsgReciever";

    wc.lpfnWndProc = hidden_wndproc;
    wc.lpszClassName = class_name;

    printf("[client] creating window\n");

    RegisterClass(&wc);
    HWND hwnd = create_window_redacted(class_name, window_title);

    if (!hwnd) {
        printf("[client] failed to create reciever window\n");
        return 0;
    }

    printf("[client] waiting for messages\n");

    //message handling 

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));

    while (msg.message != WM_QUIT) {

        if (PeekMessageA(&msg, hwnd, 0U, 0U, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
    }
}

LRESULT CALLBACK hidden_wndproc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (uMsg == WM_COPYDATA) {

        PCOPYDATASTRUCT pcds = (PCOPYDATASTRUCT)lParam;

        if (pcds->dwData == 1) {

            printf("[client] message recieved\n");

            s_data* data_ptr = (s_data*)pcds->lpData;
            const char* data = data_ptr->data;

            printf("[client] data->%s", data);
        }
	}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}   