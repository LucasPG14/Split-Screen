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
	
	cam1 = new Camera({ 0, 0, 640, 360 }, { 0, 0, 640, 360 });
	cam2 = new Camera({ 0, 0, 640, 360 }, { 640, 0, 640, 360 });
	cam3 = new Camera({ 0, 0, 640, 360 }, { 0, 360, 640, 360 });
	cam4 = new Camera({ 0, 0, 640, 360 }, { 640, 360, 640, 360 });

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

	render->AddCamera(cam1);
	render->AddCamera(cam2);
	render->AddCamera(cam3);
	render->AddCamera(cam4);

	// Loading map and player
	map->Load("map.tmx", tex);

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

	if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		cam2->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		cam2->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		cam2->bounds.x -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		cam2->bounds.x += 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	{
		cam1->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	{
		cam1->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	{
		cam1->bounds.x += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	{
		cam1->bounds.x -= 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	{
		cam3->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	{
		cam3->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	{
		cam3->bounds.x -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	{
		cam3->bounds.x += 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
	{
		cam4->bounds.y -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
	{
		cam4->bounds.y += 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
	{
		cam4->bounds.x -= 200 * dt;
	}
	if (input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
	{
		cam4->bounds.x += 200 * dt;
	}

	if (input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveGameRequest();
	if (input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadGameRequest();

	CameraFollow(dt);

	return ret;
}

void SceneGameplay::Draw(Render* render)
{
	//render->DrawTexture(bg, 0, 0);
	map->Draw(render, showColliders);
	//render->DrawTexture(camera, bg, 0, 0);
	//player->Draw(render, showColliders);
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
	render->EraseCamera(cam1);

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