bool ModuleCamera::Update(float dt) {
	float spd = base_speed * (app->input->keyShift ? 2.f : 1.f);
	if (app->input->rmb) { // FPS free look WASD
		yaw -= app->input->mouse_dx * mouse_sens;
		pitch -= app->input->mouse_dy * mouse_sens; pitch = std::clamp(pitch, -1.55f, 1.55f);
		Vec3 fwd = Forward(); Vec3 right = Right(); Vec3 up = Up();
		if (app->input->keyW) pos += fwd * spd * dt;
		if (app->input->keyS) pos -= fwd * spd * dt;
		if (app->input->keyA) pos -= right * spd * dt;
		if (app->input->keyD) pos += right * spd * dt;
		if (app->input->keyQ) pos -= up * spd * dt;
		if (app->input->keyE) pos += up * spd * dt;
	}
	// Zoom rueda
	if (app->input->wheel) distance = std::max(0.1f, distance - app->input->wheel * zoom_step);
	// Orbitar con Alt+LMB
	if (app->input->alt && app->input->lmb) { yaw -= app->input->mouse_dx * orbit_sens; pitch -= app->input->mouse_dy * orbit_sens; }
	// Focus F: mover cámara para enmarcar AABB seleccionado
	if (app->input->keyF) { FocusSelected(); }
	RebuildViewProj(); return true;
}