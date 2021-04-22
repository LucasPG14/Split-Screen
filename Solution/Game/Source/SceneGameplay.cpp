#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "SceneGameplay.h"
#include "Map.h"
#include "Player.h"
#include "EntityManager.h"
#include "Font.h"
#include "Audio.h"

#include "Log.h"

SceneGameplay::SceneGameplay(App* a)
{
	name.Create("scenegameplay");

	this->app = a;
	bg = nullptr;

	// Instantiating Map, Player and EntityManager
	map = new Map();

	showColliders = true;
}

bool SceneGameplay::Load(Textures* tex, Audio* audio, Render* render, DisplayType type)
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	CreateCameras(type, render);

	// Loading map and player
	map->Load("map.tmx", tex);

	int width = map->data.width * map->data.tileWidth;
	int height = map->data.height * map->data.tileHeight;

	for (int i = 0; i < render->cameras.Count(); ++i)
	{
		Player* player = new Player(i + 1, EntityType::PLAYER, iPoint(100, 50), iPoint(width, height));
		player->Load(tex);
		player->SetCamera(render->GetCamera());
		players.Add(player);
	}

	// Loading assets
	bg = tex->Load("Assets/Textures/texture.png");

	return ret;
}

bool SceneGameplay::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	ListItem<Player*>* it = players.start;

	while (it != nullptr)
	{
		it->data->Update(input, dt);
		it = it->next;
	}

	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();
	if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) TransitionToScene(SceneType::TITLE, DisplayType::NORMAL);

	return ret;
}

void SceneGameplay::Draw(Render* render)
{
	map->Draw(render, showColliders);

	ListItem<Player*>* it = players.start;

	while (it != nullptr)
	{
		it->data->Draw(render, showColliders);
		it = it->next;
	}
}

bool SceneGameplay::UnLoad(Textures* tex, Audio* audio, Render* render)
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	tex->UnLoad(bg);
	map->CleanUp();
	
	ListItem<Player*>* it = players.start;

	while (it != nullptr)
	{
		it->data->UnLoad(tex);
		it = it->next;
	}

	players.Clear();

	RELEASE(map);

	return ret;
}

bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	return true;
}