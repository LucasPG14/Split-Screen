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

	bg = nullptr;
	camera = new Camera({ 0,0,0,0 }, { 0,0,0,0 });

	// Instantiating Map, Player and EntityManager
	map = new Map();
	player = new Player(EntityType::PLAYER, iPoint(0, 0));
	entities = new EntityManager();

	showColliders = false;
	this->app = a;
}

bool SceneGameplay::Load(Textures* tex, Audio* audio, Render* render)
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	// Loading map and player
	map->Load("level1.tmx", tex);

	player->Load(tex);

	// Loading assets
	bg = tex->Load("Assets/Textures/texture.png");

	font = new Font("", tex);

	return ret;
}

bool SceneGameplay::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	SDL_Rect temp = player->bounds;

	player->Update(input, dt);

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) camera->bounds.y += 200 * dt;
	if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) camera->bounds.y -= 200 * dt;

	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	CameraFollow(dt);

	return ret;
}

void SceneGameplay::Draw(Render* render)
{
	//map->Draw(render, showColliders);
	render->DrawTexture(camera, bg, 0, 0);
	player->Draw(render, showColliders);
	//render->DrawRectangle(player2, 255, 255, 0);
	//render->DrawCircle(circle->GetX(), circle->GetY() + circle->GetRadius(), circle->GetRadius(), 255, 0, 0);
}

bool SceneGameplay::UnLoad(Textures* tex, Audio* audio, Render* render)
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	tex->UnLoad(bg);
	player->UnLoad(tex);
	entities->UnLoad(tex);
	map->CleanUp();
	audio->Reset();
	render->EraseCamera(camera);

	delete font;
	delete entities;
	delete player;
	delete map;

	return ret;
}

bool SceneGameplay::OnGuiMouseClickEvent(GuiControl* control)
{
	return false;
}

void SceneGameplay::CameraFollow(float dt)
{
	//if (player->GetBounds().x + player->GetBounds().w >= abs(camera.x) + (camera.w / 2) && player->GetSpeed().x > 0)
	//	camera.x -= 200.0f * dt;

	//if (player->GetBounds().x + player->GetBounds().w <= abs(camera.x) + (camera.w / 2) && player->GetSpeed().x < 0)
	//	camera.x += 200.0f * dt;

	//if (player->GetBounds().y + player->GetBounds().h >= abs(camera.y) + (camera.h / 2) && player->GetSpeed().y > 0)
	//	camera.y -= 200.0f * dt;

	//if (player->GetBounds().y + player->GetBounds().h <= abs(camera.y) + (camera.h / 2) && player->GetSpeed().y < 0)
	//	camera.y += 200.0f * dt;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	player->LoadState(load.child(player->name.GetString()));

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	player->SaveState(save.append_child(player->name.GetString()));

	return true;
}