#pragma once
#include <string>
class App;
class Module {
public:
	explicit Module(App* app, const std::string& name) : app(app), name(name) {}
	virtual ~Module() = default;
	virtual bool Init() { return true; }
	virtual bool Start() { return true; }
	virtual bool PreUpdate(float dt) { return true; }
	virtual bool Update(float dt) { return true; }
	virtual bool PostUpdate(float dt) { return true; }
	virtual bool CleanUp() { return true; }
	const std::string name;
protected:
	App* app = nullptr;
};
