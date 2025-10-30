#include "ModuleRenderer.h"
#include <glad/glad.h>
#include <SDL.h>
#include "ModuleWindow.h"
#include "App.h"
#include "Log.h"


bool ModuleRenderer::Init() {
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) { LOG_ERROR("GLAD init failed"); return false; }
	LOG_INFO("GL Vendor: %s", glGetString(GL_VENDOR));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	return true;
}


bool ModuleRenderer::PreUpdate(float) { glViewport(0, 0, viewport.x, viewport.y); glClearColor(bg.r, bg.g, bg.b, 1.f); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); return true; }
bool ModuleRenderer::PostUpdate(float) { SDL_GL_SwapWindow(app->window->window); return true; }