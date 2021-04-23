#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"
#include "Camera.h"
#include "Point.h"
#include "Font.h"
#include "List.h"

#include "SDL/include/SDL.h"

class Window;

class Render : public Module
{
public:

	Render(Window* window);

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawSectionTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, SDL_Rect cam = { 0,0,0,0 }, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawTexture(SDL_Texture* texture, int x, int y, SDL_Rect cam = { 0,0,0,0 }, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX);
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, SDL_Rect cam = {0,0,0,0}, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(Font* font, const char* text, int x, int y, int size, int spacing, SDL_Color tint);

	// Set background color
	void SetBackgroundColor(SDL_Color color);
	void AddCamera(iPoint bounds, SDL_Rect viewport);
	void ClearCameras();
	Camera* GetCamera();

public:
	SDL_Renderer* renderer;
	SDL_Color background;
	uint scale;
	List<Camera*> cameras;
private:

	Window* win;
};

#endif // __RENDER_H__