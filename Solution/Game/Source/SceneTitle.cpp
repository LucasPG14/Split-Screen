#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneTitle.h"
#include "GuiControl.h"
#include "Audio.h"

#include "Log.h"

SceneTitle::SceneTitle()
{
	bg = nullptr;
	camera = new Camera({ 0,0,640,360 }, { 0,0,640,360 });
	cam2 = new Camera({ 1280,0,640,360 }, { 640,0,640,360 });
	cam3 = new Camera({ 0,360,640,360 }, { 0,360,640,360 });
	cam4 = new Camera({ 640,360,640,360 }, { 640,360,640,360 });

	showColliders = false;
}

bool SceneTitle::Load(Textures* tex, Audio* audio, Render* render)
{
	LOG("Loading Scene Title");
	bool ret = true;

	bg = tex->Load("Assets/Textures/texture.png");
	audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	render->AddCamera(camera);
	render->AddCamera(cam2);
	render->AddCamera(cam3);
	render->AddCamera(cam4);

	player = { 50,50, 16, 32 };

	return ret;
}

bool SceneTitle::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		player.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		player.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		player.x -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		player.x += 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		cam2->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		cam2->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cam2->bounds.x -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cam2->bounds.x += 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		camera->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		camera->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

	return ret;
}

void SceneTitle::Draw(Render* render)
{
	render->DrawTexture(camera, bg, 0, 0, &camera->bounds);
	//render->DrawTexture(cam2, bg, cam2->viewport.x, cam2->viewport.y, &cam2->bounds);
	//render->DrawTexture(cam3, bg, cam3->viewport.x, cam3->viewport.y, &cam3->bounds);
	//render->DrawTexture(cam4, bg, cam4->viewport.x, cam4->viewport.y, &cam4->bounds);
	render->DrawRectangle(camera, player, 255, 0, 0);
}

bool SceneTitle::UnLoad(Textures* tex, Audio* audio, Render* render)
{
	LOG("Unloading Scene Title");
	bool ret = true;

	tex->UnLoad(bg);
	audio->Reset();

	return ret;
}

bool SceneTitle::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON: break;
	case GuiControlType::CHECKBOX: break;
	case GuiControlType::SLIDER: break;
	}
	return true;
}