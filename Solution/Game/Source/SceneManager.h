#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "Module.h"
#include "Scene.h"

struct SDL_Texture;
class Audio;

class SceneManager : public Module
{
public:

	SceneManager(App* a, Input* input, Render* render, Textures* tex);

	// Destructor
	virtual ~SceneManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	bool LoadGame(pugi::xml_node&);

	bool SaveGame(pugi::xml_node&) const;

private:
	Scene* current;
	Scene* menu;
	Scene* next;

	bool onTransition;
	bool fadeOutCompleted;
	float transitionAlpha;

	App* app;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
};

#endif // __SCENE_H__