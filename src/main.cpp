#include "../include/ant_world.h"

#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)nCmdShow;
    (void)lpCmdLine;
    (void)hPrevInstance;

    aw::AntWorld app;
    app.Initialize((void *)&hInstance, ysContextObject::DIRECTX11);
    app.Run();

    return 0;
}
