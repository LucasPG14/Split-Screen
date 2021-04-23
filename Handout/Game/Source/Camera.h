#pragma once

#include "SDL/include/SDL.h"
#include "Point.h"

class Camera
{
public:
	Camera(iPoint b, SDL_Rect view);

	virtual ~Camera();

	SDL_Rect GetViewport();

	iPoint GetBounds();

	iPoint pos;
	SDL_Rect viewport;

	bool assigned;
private:
};