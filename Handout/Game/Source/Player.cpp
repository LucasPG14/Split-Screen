#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "Player.h"

#include "Log.h"

Player::Player(int i, EntityType type, iPoint pos, iPoint limits) : Entity(type)
{
	name.Create("player");
	bounds = { pos.x, pos.y, 32, 64 };
	speed = iPoint(0,0);
	mapLimits = limits;
	texture = nullptr;
	camera = nullptr;
	id = i;

	godMode = false;
}

bool Player::Load(Textures* tex)
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

bool Player::Update(Input* input, float dt)
{
	LOG("Updating Player");
	bool ret = true;

	SDL_Rect tempPlayerBounds = bounds;

	speed.x = speed.y = 0;

	HandleInputs(input, dt);
	CameraFollow();

	Move();

	return ret;
}

void Player::Draw(Render* render, bool showColliders)
{
	//render->DrawTexture(texture, bounds.x, bounds.y, NULL);
	switch (id)
	{
	case 1:
		render->DrawRectangle(bounds, 255, 0, 0, 255);
		break;
	case 2:
		render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	case 3:
		render->DrawRectangle(bounds, 0, 0, 255, 255);
		break;
	case 4:
		render->DrawRectangle(bounds, 150, 150, 0, 255);
		break;
	}
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

void Player::CameraFollow()
{
	int midPlayerPosX = (bounds.w / 2) + bounds.x;
	int midPlayerPosY = (bounds.h / 2) + bounds.y;

	int camX = -camera->pos.x + camera->viewport.x;
	int camY = -camera->pos.y + camera->viewport.y;

	if (midPlayerPosX > -camera->pos.x + (camera->viewport.w / 2) || midPlayerPosX < -camera->pos.x + (camera->viewport.w / 2))
	{
		camera->pos.x = (midPlayerPosX - (camera->viewport.w / 2));
	}

	if (midPlayerPosY > -camera->pos.y + (camera->viewport.h / 2) || midPlayerPosY < -camera->pos.y + (camera->viewport.h / 2))
	{
		camera->pos.y = (midPlayerPosY - (camera->viewport.h / 2));
	}

	if (camera->pos.x <= 0) camera->pos.x = 0;
	if (camera->pos.y <= 0) camera->pos.y = 0;
	if (camera->pos.x + camera->viewport.w >= mapLimits.x) camera->pos.x = mapLimits.x - camera->viewport.w;
	if (camera->pos.y + camera->viewport.h >= mapLimits.y) camera->pos.y = mapLimits.y - camera->viewport.h;
}

void Player::SetCamera(Camera* cam)
{
	camera = cam;
	bounds.x = 100 + cam->GetViewport().x;
	bounds.y = 100 + cam->GetViewport().y;
}

void Player::HandleInputs(Input* input, float dt)
{
	switch (id)
	{
	case 1:
		if (input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) speed.y = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) speed.x = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) speed.y = 200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) speed.x = 200.0f * dt;
		break;
	case 2:
		if (input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) speed.y = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) speed.x = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) speed.y = 200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) speed.x = 200.0f * dt;
		break;
	case 3:
		if (input->GetKey(SDL_SCANCODE_I) == KEY_REPEAT) speed.y = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) speed.x = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) speed.y = 200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) speed.x = 200.0f * dt;
		break;
	case 4:
		if (input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT) speed.y = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT) speed.x = -200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT) speed.y = 200.0f * dt;
		if (input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT) speed.x = 200.0f * dt;
		break;
	}
}

void Player::Move()
{
	bounds.x += speed.x;
	bounds.y += speed.y;
}