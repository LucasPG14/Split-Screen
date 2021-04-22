#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"

class App;
class Player;
class Map;
class Font;
class Physics;
class EntityManager;
class PathFinding;

class SceneGameplay : public Scene
{
public:
	SceneGameplay(App* a);

	virtual ~SceneGameplay() {}

	bool Load(Textures* tex, Audio* audio, Render* render, DisplayType type) override;

	bool Update(Input* input, Audio* audio, float dt) override;

	void Draw(Render* render) override;

	bool UnLoad(Textures* tex, Audio* audio, Render* render) override;

	bool OnGuiMouseClickEvent(GuiControl* control);

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

private:
	SDL_Texture* bg;

	List<Player*> players;

	App* app;
	Map* map;
};

#endif //__SCENEGAMEPLAY_H__