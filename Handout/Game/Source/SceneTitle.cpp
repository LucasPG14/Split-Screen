#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneTitle.h"
#include "Audio.h"

#include "GuiButton.h"
#include "Log.h"

SceneTitle::SceneTitle()
{
	showColliders = true;
}

bool SceneTitle::Load(Textures* tex, Audio* audio, Render* render, DisplayType type)
{
	LOG("Loading Scene Title");
	bool ret = true;

	CreateCameras(type, render);

	twoHorizontalScreens = new GuiButton(1, { 540, 290, 200, 50 }, "HorizontalScreens");
	twoHorizontalScreens->SetObserver(this);
	
	twoVerticalScreens = new GuiButton(2, { 540, 360, 200, 50 }, "VerticalScreens");
	twoVerticalScreens->SetObserver(this);

	fourScreens = new GuiButton(3, { 540, 430, 200, 50 }, "FourScreens");
	fourScreens->SetObserver(this);

	return ret;
}

bool SceneTitle::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	twoHorizontalScreens->Update(input, dt);
	twoVerticalScreens->Update(input, dt);
	fourScreens->Update(input, dt);

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