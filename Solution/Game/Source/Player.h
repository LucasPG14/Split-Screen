#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "Entity.h"

class Input;
class Physics;

class Player : public Entity
{
public:
	Player(EntityType type, iPoint pos);

	virtual ~Player() {}

	bool Load(Textures* tex) override;

	bool Update(Input* input, float dt);

	void Draw(Render* render, bool showColliders) override;

	bool UnLoad(Textures* tex) override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	// Getters
	SDL_Rect GetBounds() const { return bounds; }

	iPoint GetSpeed() const { return speed; }

	void SetCamera(int w, int h);

private:

	// Functions
	void HandleInputs(Input* input, float dt);
	void Move();

	// Variables
	SDL_Texture* texture;

	iPoint speed;

	bool godMode;

	SDL_Rect camera;
};

#endif //__PLAYER_H__