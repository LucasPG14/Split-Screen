#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "SDL/include/SDL.h"
#include "Entity.h"

class Input;
class Camera;

class Player : public Entity
{
public:
	Player(int i, EntityType type, iPoint pos, iPoint limits);

	virtual ~Player() {}

	bool Load(Textures* tex) override;

	bool Update(Input* input, float dt);

	void Draw(Render* render, bool showColliders) override;

	bool UnLoad(Textures* tex) override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	void CameraFollow();

	// Getters
	SDL_Rect GetBounds() const { return bounds; }

	iPoint GetSpeed() const { return speed; }

	// Setters
	inline void SetCamera(Camera* cam) { camera = cam; }

private:

	// Functions
	void HandleInputs(Input* input, float dt);
	void Move();

	// Variables
	SDL_Texture* texture;
	Camera* camera;

	iPoint speed;

	bool godMode;
	int id;

	iPoint mapLimits;
};

#endif //__PLAYER_H__