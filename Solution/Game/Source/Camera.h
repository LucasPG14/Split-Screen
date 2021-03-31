#pragma once

#include "SDL/include/SDL.h"

class Window;

class Camera
{
public:
	Camera(SDL_Rect b, SDL_Rect view);

	virtual ~Camera();

	SDL_Rect GetViewport();

	SDL_Rect GetBounds();

	SDL_Rect bounds;
	SDL_Rect viewport;
private:
};