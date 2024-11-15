#include "ECS.h"
#include "../Logger/Logger.h"
#include <string>


int Entity::GetId() const {
	return id;
}

void System::AddEntityToSystem(Entity entity){
	entities.push_back(entity);
}

void System::RemoveEntityFromSystem(Entity entity){
	// Using c++ iterators
	entities.erase(std::__remove_if(entities.begin(), entities.end(),
		[&entity](Entity other){
			return entity == other;
		}), entities.end());
}

std::vector<Entity> System::GetSystemEntities() const{
	return entities;
}


const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity() {
	int entityId;
	
	entityId = numEntities++;

	Entity entity(entityId);
	entitiesToBeAdded.insert(entity);
	return entity;

	Logger::Log("Entity created with id = " + std::to_string(entityId));
}

void Registry::Update() {
	
}