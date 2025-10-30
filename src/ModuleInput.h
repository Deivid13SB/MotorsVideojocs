#pragma once

#include "Module.h"
#include <SDL.h>
#include <vector>
#include <string>

class ModuleInput : public Module
{
public:
    explicit ModuleInput(App* app);
    ~ModuleInput() override = default;

    bool Init() override;
    bool PreUpdate(float dt) override;
    bool CleanUp() override;

    bool keyW = false, keyA = false, keyS = false, keyD = false;
    bool keyQ = false, keyE = false;
    bool keyShift = false, keyCtrl = false, keyAlt = false;
    bool keyF = false;

 
    int mouse_x = 0, mouse_y = 0;
    int mouse_dx = 0, mouse_dy = 0;
    int wheel = 0;
    bool lmb = false;   
    bool rmb = false;   
    bool mmb = false;  


    bool quit = false;


    std::vector<std::string> pending_drop;

private:
   
    int last_x = 0;
    int last_y = 0;
};
