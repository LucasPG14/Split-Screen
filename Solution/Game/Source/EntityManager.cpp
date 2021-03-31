#include "EntityManager.h"

#include "Player.h"
#include "Log.h"

EntityManager::EntityManager()
{
	
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Update(float dt, Physics* physics)
{
	bool ret = true;

	ListItem<Entity*>* item = entities.start;

	// Update Logic
	while (item)
	{
		if (item->data->type != EntityType::PLAYER) item->data->Update(dt);
		item = item->next;
	}

	return ret;
}

void EntityManager::Draw(Render* render, bool showColliders)
{
	ListItem<Entity*>* item = entities.start;

	while (item)
	{
		item->data->Draw(render, showColliders);
		item = item->next;
	}
}

bool EntityManager::UnLoad(Textures* tex)
{
	LOG("Unloading Entities");
	bool ret = true;

	ListItem<Entity*>* item = entities.start;

	while (item && ret)
	{
		ret = item->data->UnLoad(tex);
		item = item->next;
	}
	
	return false;
}

Entity* EntityManager::CreateEntity(EntityType type, iPoint pos)
{
	LOG("Creating %s", type);
	Entity* entity = nullptr;

	switch (type)
	{
	/*case EntityType::PLAYER: break;*/
	case EntityType::ITEM: break;
	case EntityType::ENEMY: break;
	}

	// Created entities are added to the list
	if (entity != nullptr) entities.Add(entity); // Should player be on the list?

	return entity;
}

void EntityManager::DeleteEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.start;

	while (item)
	{
		if (item->data == entity)
		{
			entities.Del(item);
			break;
		}
		item = item->next;
	}
}