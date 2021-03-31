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

	bool Load(Textures* tex, Audio* audio, Render* render) override;

	bool Update(Input* input, Audio* audio, float dt) override;

	void Draw(Render* render) override;

	bool UnLoad(Textures* tex, Audio* audio, Render* render) override;

	bool OnGuiMouseClickEvent(GuiControl* control);
	
	void CameraFollow(float dt);

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

private:
	SDL_Texture* bg;
	SDL_Rect player2;

	App* app;
	Player* player;
	Map* map;
	Font* font;
	EntityManager* entities;
	PathFinding* path;
};

#endif //__SCENEGAMEPLAY_H__