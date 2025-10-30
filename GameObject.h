#pragma once
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include <algorithm>

#include "Component.h"


struct C_Transform;

class GameObject
{
public:
    std::string name = "GameObject";
    bool        active = true;

    GameObject* parent = nullptr;
    std::vector<std::unique_ptr<GameObject>> children;
    std::vector<std::unique_ptr<Component>>  components;

public:
    GameObject() = default;
    explicit GameObject(std::string n) : name(std::move(n)) {}
    ~GameObject() = default;


    GameObject* CreateChild(const std::string& childName = "GameObject")
    {
        auto child = std::make_unique<GameObject>(childName);
        child->parent = this;
        GameObject* raw = child.get();
        children.emplace_back(std::move(child));
        return raw;
    }

    void SetParent(GameObject* newParent)
    {
        if (parent == newParent) return;


        if (parent)
        {
            auto& siblings = parent->children;
            siblings.erase(
                std::remove_if(siblings.begin(), siblings.end(),
                    [&](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == this; }),
                siblings.end()
            );
        }

        parent = newParent;

        if (newParent)
        {

            newParent->AdoptExternalChild(this);
        }
    }

    void AdoptExternalChild(GameObject* existing)
    {
        if (!existing) return;
        existing->parent = this;

        children.emplace_back(std::unique_ptr<GameObject>(existing));
    }

    void RemoveChild(GameObject* child)
    {
        if (!child) return;
        children.erase(
            std::remove_if(children.begin(), children.end(),
                [&](const std::unique_ptr<GameObject>& ptr) { return ptr.get() == child; }),
            children.end()
        );
    }

    GameObject* FindByName(const std::string& target)
    {
        if (name == target) return this;
        for (auto& c : children)
        {
            if (auto* r = c->FindByName(target)) return r;
        }
        return nullptr;
    }

    template <class T, class... Args>
    T* Add(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "T debe heredar de Component");
        auto c = std::make_unique<T>(std::forward<Args>(args)...);
        c->owner = this;
        T* raw = c.get();
        components.emplace_back(std::move(c));
        return raw;
    }


    template <class T>
    T* Get() const
    {
        static_assert(std::is_base_of<Component, T>::value, "T debe heredar de Component");
        for (const auto& c : components)
        {
            if (auto* p = dynamic_cast<T*>(c.get()))
                return p;
        }
        return nullptr;
    }

    template <class T>
    void GetAll(std::vector<T*>& out) const
    {
        static_assert(std::is_base_of<Component, T>::value, "T debe heredar de Component");
        for (const auto& c : components)
        {
            if (auto* p = dynamic_cast<T*>(c.get()))
                out.push_back(p);
        }
    }

    template <class T>
    bool Remove()
    {
        static_assert(std::is_base_of<Component, T>::value, "T debe heredar de Component");
        auto it = std::find_if(components.begin(), components.end(),
            [](const std::unique_ptr<Component>& c) { return dynamic_cast<T*>(c.get()) != nullptr; });
        if (it != components.end())
        {
            components.erase(it);
            return true;
        }
        return false;
    }

    void SetActive(bool value)
    {
        active = value;
        for (auto& c : components) c->active = value;
        for (auto& ch : children)  ch->SetActive(value);
    }


    template <class Fn>
    void ForEach(Fn&& fn)
    {
        fn(*this);
        for (auto& c : children) c->ForEach(fn);
    }

    C_Transform* Transform() const;
};
