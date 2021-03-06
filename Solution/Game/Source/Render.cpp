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
	Camera* camera = new Camera(bounds, viewport);
	
	cameras.Add(camera);
}

void Render::ClearCameras()
{
	cameras.Clear();
}

Camera* Render::GetCamera()
{
	ListItem<Camera*>* item = cameras.start;
	for (; item != nullptr; item = item->next)
	{
		if (item->data->assigned == false)
		{
			item->data->assigned = true;
			return item->data;
		}
	}
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
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, SDL_Rect cam, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	SDL_Rect rect = {0,0,0,0};
	ListItem<Camera*>* it = cameras.start;
	for (; it != nullptr; it = it->next)
	{
		rect.x = (int)((-it->data->GetPos().x + it->data->GetViewport().x) * speed) + x * scale;
		rect.y = (int)((-it->data->GetPos().y + it->data->GetViewport().y) * speed) + y * scale;
		SDL_Rect cam = it->data->GetViewport();
		if (section != NULL)
		{
			rect.w = section->w;
			rect.h = section->h;
		}
		else
		{
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
		}
			
		if (rect.x + rect.w > cam.x && rect.x < cam.x + cam.w &&
			rect.y + rect.h > cam.y && rect.y < cam.y + cam.h)
		{
			if (rect.x < cam.x)
				rect.x = cam.x;
			if (rect.y < cam.y)
				rect.y = cam.y;
			if (rect.x + rect.w > cam.x + cam.w)
				rect.w = (cam.x + cam.w) - rect.x;
			if (rect.y + rect.h > cam.y + cam.h)
				rect.h = (cam.y + cam.h) - rect.y;

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
			rec.x = (int)((rect.x + (it->data->GetViewport().x - it->data->GetPos().x)) * scale);
			rec.y = (int)((rect.y + (it->data->GetViewport().y - it->data->GetPos().y)) * scale);
			rec.w *= scale;
			rec.h *= scale;
		}
		if (rec.x + rec.w > cam.x && rec.x < cam.x + cam.w &&
			rec.y + rec.h > cam.y && rec.y < cam.y + cam.h)
		{
			if (rec.x < cam.x)
				rec.x = cam.x;
			if (rec.y < cam.y)
				rec.y = cam.y;
			if (rec.x + rec.w > cam.x + cam.w)
				rec.w = (cam.x + cam.w) - rec.x;
			if (rec.y + rec.h > cam.y + cam.h)
				rec.h = (cam.y + cam.h) - rec.y;

			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(renderer, r, g, b, a);

			int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

			if (result != 0)
			{
				LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
				ret = false;
			}
		}
	}

	return ret;
}

bool Render::DrawLine(Camera* camera, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera->GetPos().x + x1 * scale, camera->GetPos().y + y1 * scale, camera->GetPos().x + x2 * scale, camera->GetPos().y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(Camera* camera, int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
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