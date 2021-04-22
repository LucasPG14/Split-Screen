#ifndef __SCENETITLE_H__
#define __SCENETITLE_H__

#include "Scene.h"
#include "Camera.h"

class GuiButton;

class SceneTitle : public Scene
{
public:
	SceneTitle();

	virtual ~SceneTitle() {}

	bool Load(Textures* tex, Audio* audio, Render* render, DisplayType type) override;

	bool Update(Input* input, Audio* audio, float dt) override;

	void Draw(Render* render) override;

	bool UnLoad(Textures* tex, Audio* audio, Render* render) override;

	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* bg;
	Camera* cam2;
	Camera* cam3;
	Camera* cam4;
	SDL_Rect player;

	GuiButton* twoHorizontalScreens;
	GuiButton* twoVerticalScreens;
	GuiButton* fourScreens;
};

#endif //__SCENETITLE_H__