bool ModuleInput::PreUpdate(float) {
	SDL_Event e; wheel = 0; mouse_dx = mouse_dy = 0;
	while (SDL_PollEvent(&e)) {
		ImGui_ImplSDL2_ProcessEvent(&e); // para ImGui
		switch (e.type) {
		case SDL_QUIT: want_quit = true; break;
		case SDL_MOUSEWHEEL: wheel = e.wheel.y; break;
		case SDL_MOUSEMOTION: mouse_dx += e.motion.xrel; mouse_dy += e.motion.yrel; break;
		case SDL_DROPFILE: pending_drop.emplace_back(e.drop.file); SDL_free(e.drop.file); break;
		}
	}
	const Uint8* ks = SDL_GetKeyboardState(nullptr);
	keyW = ks[SDL_SCANCODE_W]; keyA = ks[SDL_SCANCODE_A]; keyS = ks[SDL_SCANCODE_S]; keyD = ks[SDL_SCANCODE_D];
	keyQ = ks[SDL_SCANCODE_Q]; keyE = ks[SDL_SCANCODE_E]; keyF = ks[SDL_SCANCODE_F]; keyShift = ks[SDL_SCANCODE_LSHIFT] || ks[SDL_SCANCODE_RSHIFT];
	rmb = (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT));
	lmb = (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_LEFT));
	alt = ((SDL_GetModState() & KMOD_ALT) != 0);
	return true;
}