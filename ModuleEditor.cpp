#include "ModuleEditor.h"
#include "App.h"
#include "ModuleWindow.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>


bool ModuleEditor::Init() { IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGuiIO& io = ImGui::GetIO(); io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; ImGui::StyleColorsDark(); ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->window->gl_context); ImGui_ImplOpenGL3_Init("#version 460"); return true; }
bool ModuleEditor::PreUpdate(float) { ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplSDL2_NewFrame(); ImGui::NewFrame(); return true; }
bool ModuleEditor::PostUpdate(float) {
	if (show_toolbar) DrawMainMenu();
	if (show_console) DrawConsole();
	if (show_config) DrawConfig();
	if (show_hierarchy) DrawHierarchy();
	if (show_inspector) DrawInspector();
	ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); return true;
}
bool ModuleEditor::CleanUp() { ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplSDL2_Shutdown(); ImGui::DestroyContext(); return true; }

void ModuleEditor::DrawMainMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) request_quit = true;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {
			ImGui::MenuItem("Console", nullptr, &show_console);
			ImGui::MenuItem("Config", nullptr, &show_config);
			ImGui::MenuItem("Hierarchy", nullptr, &show_hierarchy);
			ImGui::MenuItem("Inspector", nullptr, &show_inspector);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("Docs")) OpenURL(repo_url + "/docs");
			if (ImGui::MenuItem("Report a bug")) OpenURL(repo_url + "/issues");
			if (ImGui::MenuItem("Download latest")) OpenURL(repo_url + "/releases");
			if (ImGui::MenuItem("About")) show_about = true;
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}