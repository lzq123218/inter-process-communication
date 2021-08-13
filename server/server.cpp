#include <Windows.h>
#include <iostream>
#include <string>

//data struct, must be same as client
typedef struct data {
    char data[255];
} s_data;

int main() {

    HWND msg_window = nullptr;

    printf("[server] waiting for client\n");

    while (!msg_window) {
        msg_window = FindWindowA("SharedMemoryHidden", "MsgReciever");
        Sleep(100);
    }

    printf("[server] input data->");

    std::string data;

    do {                     
        std::getline(std::cin, data);
    } while (data.empty());

    s_data* data_struct = new s_data;
    strcpy_s(data_struct->data, data.c_str());

    COPYDATASTRUCT* cds = new COPYDATASTRUCT;

    cds->dwData = 1;
    cds->cbData = sizeof(data_struct);
    cds->lpData = data_struct;

    printf("[server] sending message\n");

    SendMessageA(msg_window, WM_COPYDATA, (WPARAM) msg_window, (LPARAM)(LPVOID) cds);
}