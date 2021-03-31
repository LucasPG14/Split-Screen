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

class Scene
{
public:
	
	Scene() : active(false), transitionRequired(false) 
	{
	}

	~Scene() {}

	virtual bool Load(Textures* tex, Audio* audio, Render* render) { return true; }

	virtual bool Update(Input* input, Audio* audio, float dt) { return true; }

	virtual void Draw(Render* render) {}

	virtual bool UnLoad(Textures* tex, Audio* audio, Render* render) { return true; }

	virtual bool LoadState(pugi::xml_node&) { return true; }

	virtual bool SaveState(pugi::xml_node&) const { return true; }

	void TransitionToScene(SceneType scene)
	{
		LOG("Changing Scene");
		transitionRequired = true;
		nextScene = scene;
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

	Camera* camera;

	bool showColliders;
};

#endif