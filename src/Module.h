#pragma once
#include <string>
#include "Log.h"


class App;

class Module
{
public:
    explicit Module(App* app, const std::string& name)
        : app(app), name(name)
    {
    }

    virtual ~Module() = default;

    virtual bool Init() { LOG_INFO("[%s] Init()", name.c_str()); return true; }
    virtual bool Start() { LOG_INFO("[%s] Start()", name.c_str()); return true; }
    virtual bool PreUpdate(float dt) { return true; }
    virtual bool Update(float dt) { return true; }
    virtual bool PostUpdate(float dt) { return true; }
    virtual bool CleanUp() { LOG_INFO("[%s] CleanUp()", name.c_str()); return true; }

    const std::string& GetName() const { return name; }

protected:
    App* app = nullptr;        
    std::string name = "Module";
};
