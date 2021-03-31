#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

#include "Log.h"

Player::Player(EntityType type, iPoint pos) : Entity(type)
{
	name.Create("player");
	bounds = { pos.x, pos.y, 32, 64 };
	speed = iPoint(0,0);
	texture = nullptr;

	camera = { pos.x, pos.y, 0, 0 };

	godMode = false;
}

bool Player::Load(Textures* tex)
{
	LOG("Loading Player");
	bool ret = true;

	texture = tex->Load("Assets/");

	return ret;
}

bool Player::Update(Input* input, float dt)
{
	LOG("Updating Player");
	bool ret = true;

	SDL_Rect tempPlayerBounds = bounds;

	speed.x = speed.y = 0;

	HandleInputs(input, dt);

	Move();

	return ret;
}

void Player::Draw(Render* render, bool showColliders)
{
	//render->DrawTexture(texture, bounds.x, bounds.y, NULL);
	if (showColliders); //render->DrawRectangle(bounds, 255, 0, 0, 255, camera);
}

bool Player::UnLoad(Textures* tex)
{
	LOG("Unloading Player");
	bool ret = true;

	tex->UnLoad(texture);

	return ret;
}

bool Player::LoadState(pugi::xml_node& load)
{
	bool ret = true;

	bounds.x = load.child("position").attribute("x").as_int();
	bounds.y = load.child("position").attribute("y").as_int();

	return ret;
}

bool Player::SaveState(pugi::xml_node& save) const
{
	bool ret = true;

	return ret;
}

void Player::SetCamera(int w, int h)
{
	camera.w = w;
	camera.h = h;
}

void Player::HandleInputs(Input* input, float dt)
{
	if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) speed.y = -200.0f * dt;
	if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) speed.x = -200.0f * dt;
	if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) speed.y = 200.0f * dt;
	if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) speed.x = 200.0f * dt;
	if (input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) godMode = !godMode;
}

void Player::Move()
{
	bounds.x += speed.x;
	bounds.y += speed.y;
}