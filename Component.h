#pragma once
#include <string>


class GameObject;

struct Component
{
    enum Type
    {
        TRANSFORM = 0,
        MESH = 1,
        TEXTURE = 2,
    
    };

    explicit Component(Type t)
        : type(t)
    {
    }

    virtual ~Component() = default;


    virtual bool Enable() { active = true;  return true; }
    virtual bool Start() { return true; }
    virtual bool Update(float) { return true; }
    virtual bool Disable() { active = false; return true; }


    virtual void OnEditor() {}


    Type        type;
    bool        active = true;
    GameObject* owner = nullptr;
};