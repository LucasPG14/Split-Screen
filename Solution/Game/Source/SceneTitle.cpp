#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneTitle.h"
#include "Audio.h"

#include "GuiButton.h"
#include "Log.h"

SceneTitle::SceneTitle()
{
	bg = nullptr;

	showColliders = true;
}

bool SceneTitle::Load(Textures* tex, Audio* audio, Render* render, DisplayType type)
{
	LOG("Loading Scene Title");
	bool ret = true;

	bg = tex->Load("Assets/Textures/texture.png");
	CreateCameras(type, render);

	player = { 50,50, 16, 32 };

	twoHorizontalScreens = new GuiButton(1, { 540, 350, 200, 50 }, "HorizontalScreens");
	twoHorizontalScreens->SetObserver(this);
	
	twoVerticalScreens = new GuiButton(2, { 540, 420, 200, 50 }, "VerticalScreens");
	twoVerticalScreens->SetObserver(this);

	fourScreens = new GuiButton(3, { 540, 490, 200, 50 }, "FourScreens");
	fourScreens->SetObserver(this);

	return ret;
}

bool SceneTitle::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		cam1->pos.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		cam1->pos.y -= 200 * dt;
	}

	twoHorizontalScreens->Update(input, dt);
	twoVerticalScreens->Update(input, dt);
	fourScreens->Update(input, dt);

	//if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::GAMEPLAY);

	return ret;
}

void SceneTitle::Draw(Render* render)
{
	SDL_Rect rect = { 0,0, 1280, 720 };
	render->DrawRectangle(rect, 255, 255, 255, 150);

	twoHorizontalScreens->Draw(render, showColliders);
	twoVerticalScreens->Draw(render, showColliders);
	fourScreens->Draw(render, showColliders);
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
	case GuiControlType::BUTTON: 
		if (control->id == 1) TransitionToScene(SceneType::GAMEPLAY, DisplayType::TWO_HORIZONTAL);
		if (control->id == 2) TransitionToScene(SceneType::GAMEPLAY, DisplayType::TWO_VERTICAL);
		if (control->id == 3) TransitionToScene(SceneType::GAMEPLAY, DisplayType::FOUR_SCREENS);
		break;
	case GuiControlType::CHECKBOX: break;
	case GuiControlType::SLIDER: break;
	}
	return true;
}