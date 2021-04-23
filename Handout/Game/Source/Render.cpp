#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "SDL/include/SDL_opengl.h" 

#define VSYNC true

Render::Render(Window* window) : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
	scale = 1;

	this->win = window;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(win->window, -1, flags);

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background
	ListItem<Camera*>* item = cameras.start;
	for (; item != nullptr; item = item->next)
	{
		SDL_RenderGetViewport(renderer, &item->data->GetViewport());
	}

	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);

	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");

	cameras.Clear();
	SDL_DestroyRenderer(renderer);

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::AddCamera(iPoint bounds, SDL_Rect viewport)
{
	// TODO 1: Create the camera and add it to the cameras list.
}

void Render::ClearCameras()
{
	// TODO 1: Erase all the cameras of the list. 
}

Camera* Render::GetCamera()
{
	// TODO 2: Iterate the cameras list and check if the camera is assigned and return it.

	return nullptr;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	ListItem<Camera*>* item = cameras.start;
	for (; item != cameras.end; item = item->next)
	{
		SDL_RenderSetViewport(renderer, &item->data->GetViewport());
	}
}

void Render::ResetViewPort()
{
	ListItem<Camera*>* item = cameras.start;
	for (; item != cameras.end; item = item->next)
	{
		SDL_RenderSetViewport(renderer, &item->data->GetViewport());
	}
}

// Blit to screen
bool Render::DrawSectionTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_Rect cam, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	SDL_Rect rect = {0,0,0,0};
	ListItem<Camera*>* it = cameras.start;
	for (; it != nullptr; it = it->next)
	{
		// TODO 5: Calculate the position of the texture, with the x and y that you receive in the function
		// and the x and y of the camera, and also you will have to use the position of the viewport
		
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}
			
		// TODO 6: Check if the rect is inside of the viewport or not. If not, you don't want to draw it

		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivotX != INT_MAX && pivotY != INT_MAX)
		{
			pivot.x = pivotX;
			pivot.y = pivotY;
			p = &pivot;
		}
		if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

bool Render::DrawTexture(SDL_Texture* texture, int x, int y, SDL_Rect cam, float speed, double angle, int pivotX, int pivotY)
{
	bool ret = true;
	SDL_Rect rect = { 0,0,0,0 };
	for (ListItem<Camera*>* it = cameras.start; it != nullptr; it = it->next)
	{
		rect.x = (int)(-it->data->GetBounds().x + it->data->GetViewport().x * speed) + x * scale;
		rect.y = (int)(-it->data->GetBounds().y + it->data->GetViewport().y * speed) + y * scale;

		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);

		rect.w -= it->data->GetViewport().w;
		rect.h -= it->data->GetViewport().h;
		
		rect.w *= scale;
		rect.h *= scale;

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (pivotX != INT_MAX && pivotY != INT_MAX)
		{
			pivot.x = pivotX;
			pivot.y = pivotY;
			p = &pivot;
		}

		if (SDL_RenderCopyEx(renderer, texture, &rect, &it->data->GetViewport(), angle, p, SDL_FLIP_NONE) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_Rect cam, bool filled, bool useCamera) const
{
	bool ret = true;

	for (ListItem<Camera*>* it = cameras.start; it != nullptr; it = it->next)
	{
		SDL_Rect cam = it->data->GetViewport();
		SDL_Rect rec(rect);
		if (useCamera)
		{
			// TODO 5: Calculate the position of the texture, with the x and y that you receive in the function
			// and the x and y of the camera, and also you will have to use the position of the viewport
			rec.w *= scale;
			rec.h *= scale;
		}

		// TODO 6: Check if the rect is inside of the viewport or not. If not, you don't want to draw it
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(renderer, r, g, b, a);

		int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

		if (result != 0)
		{
			LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
			ret = false;
		}
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer,x1 * scale, y1 * scale, x2 * scale, y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

//bool Render::DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint)
//{
//	bool ret = true;
//
//	int length = strlen(text);
//	int posX = x;
//
//	float scale = (float)size / font->GetCharRec(32).h;
//
//	SDL_SetTextureColorMod(font->GetTextureAtlas(), tint.r, tint.g, tint.b);
//
//	for (int i = 0; i < length; i++)
//	{
//		SDL_Rect recGlyph = font->GetCharRec(text[i]);
//		SDL_Rect recDest = { posX, y, (scale * recGlyph.w), size };
//
//		SDL_RenderCopyEx(camera->GetRenderer(), font->GetTextureAtlas(), &recGlyph, &recDest, 0.0, { 0 }, SDL_RendererFlip::SDL_FLIP_NONE);
//
//		posX += ((float)recGlyph.w * scale + spacing);
//	}
//
//	return ret;
//}