#ifndef __SCENE_H__
#define __SCENE_H__

#include "Log.h"
#include "PugiXml/src/pugixml.hpp"

class Audio;
class GuiControl;

enum class SceneType
{
	LOGO,
	TITLE,
	GAMEPLAY,
	ENDING
};

enum class DisplayType
{
	NORMAL,
	TWO_HORIZONTAL,
	TWO_VERTICAL,
	FOUR_SCREENS
};

class Scene
{
public:
	
	Scene() : active(false), transitionRequired(false) 
	{
	}

	~Scene() {}

	virtual bool Load(Textures* tex, Audio* audio, Render* render, DisplayType type) { return true; }

	virtual bool Update(Input* input, Audio* audio, float dt) { return true; }

	virtual void Draw(Render* render) {}

	virtual bool UnLoad(Textures* tex, Audio* audio, Render* render) { return true; }

	virtual bool LoadState(pugi::xml_node&) { return true; }

	virtual bool SaveState(pugi::xml_node&) const { return true; }

	void TransitionToScene(SceneType scene, DisplayType display)
	{
		LOG("Changing Scene");
		transitionRequired = true;
		nextScene = scene;
		nextDisplay = display;
	}

	void CreateCameras(DisplayType display, Render* render)
	{
		switch (display)
		{
		case DisplayType::NORMAL:
			render->AddCamera({ 0, 0 }, { 0, 0, 1280, 720 });
			break;
		case DisplayType::TWO_HORIZONTAL:
			render->AddCamera({ 0, 0 }, { 2, 2, 1276, 357 });
			render->AddCamera({ 0, 0 }, { 2, 361, 1276, 357 });
			break;
		case DisplayType::TWO_VERTICAL:
			render->AddCamera({ 0, 0 }, { 2, 2, 637, 716 });
			render->AddCamera({ 0, 0 }, { 641, 2, 637, 716 });
			break;
		case DisplayType::FOUR_SCREENS:
			render->AddCamera({ 0, 0 }, { 2, 2, 637, 357 });
			render->AddCamera({ 0, 0 }, { 641, 2, 637, 357 });
			render->AddCamera({ 0, 0 }, { 2, 361, 637, 357 });
			render->AddCamera({ 0, 0 }, { 641, 361, 637, 357 });
			break;
		}
	}

	// Define multiple Gui Event methods
	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:
	SString name;
	bool active;

	bool transitionRequired;
	SceneType nextScene;
	DisplayType nextDisplay;

	bool showColliders;
};

#endif