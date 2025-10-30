#include "App.h"
#include "Log.h"
#include <iostream>

int main(int argc, char** argv)
{

    LOG_INFO("======================================");
    LOG_INFO("  YourEngine - Geometry Viewer (E1)   ");
    LOG_INFO("======================================");

    App app;

    if (!app.Init())
    {
        LOG_ERROR("App Init() failed. Exiting...");
        return -1;
    }

    if (!app.Start())
    {
        LOG_ERROR("App Start() failed. Exiting...");
        return -1;
    }

    LOG_INFO("Entering main loop...");

    bool running = true;

    while (running)
    {
        running = app.Update(); 
    }


    LOG_INFO("Cleaning up...");
    app.CleanUp();
    LOG_INFO("Engine closed successfully.");

    return 0;
}