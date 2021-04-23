#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

class Render;
class Textures;
class Physics;

enum class EntityType
{
	PLAYER,
	ENEMY,
	ITEM,
	MAP,
	UNKNOWN
};

class Entity
{
public:
	Entity(EntityType type) : type(type) {}

	virtual bool Load(Textures* tex) { return true; }
	
	virtual bool Update(float dt) { return true; }

	virtual bool CheckCollisions(Physics* physics) { return true; }
	
	virtual void Draw(Render* render, bool showColliders) { return; }

	virtual bool UnLoad(Textures* tex) { return true; }

	virtual bool LoadState(pugi::xml_node&) { return true; }

	virtual bool SaveState(pugi::xml_node&) const { return true; }

public:
	SString name;
	EntityType type;
	SDL_Rect bounds;
};

#endif //__ENTITY_H__