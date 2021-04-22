#include "App.h"
#include "Window.h"

#include "Camera.h"

Camera::Camera(iPoint b, SDL_Rect view)
{
    pos = b;
    viewport = view;
    assigned = false;
}

Camera::~Camera()
{
}

SDL_Rect Camera::GetViewport()
{
    return viewport;
}

iPoint Camera::GetBounds()
{
    return pos;
}