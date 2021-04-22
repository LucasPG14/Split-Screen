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
	player1 = new Player(1, EntityType::PLAYER, iPoint(10, 50), iPoint(width, height));
	player1->Load(tex);
	player1->SetCamera(render->GetCamera());

	player2 = new Player(2, EntityType::PLAYER, iPoint(750, 50), iPoint(width, height));
	player2->Load(tex);
	player2->SetCamera(render->GetCamera());

	player3 = new Player(3, EntityType::PLAYER, iPoint(10, 500), iPoint(width, height));
	player3->Load(tex);
	player3->SetCamera(render->GetCamera());

	player4 = new Player(4, EntityType::PLAYER, iPoint(750, 500), iPoint(width, height));
	player4->Load(tex);
	player4->SetCamera(render->GetCamera());

	// Loading assets
	bg = tex->Load("Assets/Textures/texture.png");

	font = new Font("", tex);

	return ret;
}

bool SceneGameplay::Update(Input* input, Audio* audio, float dt)
{
	bool ret = true;

	SDL_Rect temp = player1->bounds;

	player1->Update(input, dt);
	player2->Update(input, dt);
	player3->Update(input, dt);
	player4->Update(input, dt);

	//if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	//{
	//	cam2->bounds.y -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	//{
	//	cam2->bounds.y += 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	//{
	//	cam2->bounds.x -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	//{
	//	cam2->bounds.x += 200 * dt;
	//}

	//if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
	//{
	//	cam1->bounds.y += 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
	//{
	//	cam1->bounds.y -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
	//{
	//	cam1->bounds.x += 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
	//{
	//	cam1->bounds.x -= 200 * dt;
	//}

	//if (input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT)
	//{
	//	cam3->bounds.y -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT)
	//{
	//	cam3->bounds.y += 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT)
	//{
	//	cam3->bounds.x -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
	//{
	//	cam3->bounds.x += 200 * dt;
	//}

	//if (input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT)
	//{
	//	cam4->bounds.y -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT)
	//{
	//	cam4->bounds.y += 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT)
	//{
	//	cam4->bounds.x -= 200 * dt;
	//}
	//if (input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT)
	//{
	//	cam4->bounds.x += 200 * dt;
	//}

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
	player1->Draw(render, showColliders);
	player2->Draw(render, showColliders);
	player3->Draw(render, showColliders);
	player4->Draw(render, showColliders);
}

bool SceneGameplay::UnLoad(Textures* tex, Audio* audio, Render* render)
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	tex->UnLoad(bg);
	player1->UnLoad(tex);
	player2->UnLoad(tex);
	player3->UnLoad(tex);
	player4->UnLoad(tex);
	entities->UnLoad(tex);
	map->CleanUp();
	audio->Reset();
	//render->EraseCamera(cam1);

	delete font;
	delete entities;
	delete player1;
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
	player1->LoadState(load.child(player1->name.GetString()));

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	player1->SaveState(save.append_child(player1->name.GetString()));

	return true;
}