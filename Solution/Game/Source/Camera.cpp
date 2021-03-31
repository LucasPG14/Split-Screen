#include "App.h"
#include "Window.h"

#include "Camera.h"

Camera::Camera(SDL_Rect b, SDL_Rect view)
{
    bounds = b;
    viewport = view;
}

Camera::~Camera()
{
}

SDL_Rect Camera::GetViewport()
{
    return viewport;
}

SDL_Rect Camera::GetBounds()
{
    return bounds;
}