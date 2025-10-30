#include "App.h"
#include <cstdio>
int main() {
	App app; if (!app.Init()) return -1; if (!app.Start()) return -1;
	bool running = true; while (running) { running = app.Update(); }
	app.CleanUp(); return 0;
}