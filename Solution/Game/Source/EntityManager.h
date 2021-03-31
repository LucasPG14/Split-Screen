#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Entity.h"

#include "Point.h"
#include "List.h"

class EntityManager
{
public:
	EntityManager();

	virtual ~EntityManager();

	bool Update(float dt, Physics* physics);

	void Draw(Render* render, bool showColliders);

	bool UnLoad(Textures* tex);

	Entity* CreateEntity(EntityType type, iPoint position);

	void DeleteEntity(Entity* entity);

	bool LoadState();

	bool SaveState() const;

public:
	List<Entity*> entities;
	List<Entity*> dynamicEntities;
};

#endif //__ENTITYMANAGER_H__