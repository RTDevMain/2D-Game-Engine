#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fields & Types
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const unsigned int MAX_COMPONENTS = 32;

// Signature: A bitset to keep track of which components an entity has.
// Also used by systems to keep track of which entities a system is interested in.

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
	protected:
		static int nextId;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Entity: 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Entity {
	private:
		int id;

	public:
		Entity(int id): id(id) {}; // Syntax used to automatically initialize constructor
		int GetId() const;	
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Component: Used to assign a unique id to a component type.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Component: public IComponent {
	// Returns the unique id of Component<T>
	static int GetId(){
		static auto id = nextId++;
		return id;
	}
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System: The system processes entities that contain a specific signature.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class System {
	private:
		Signature componentSignature;
		std::vector<Entity> entities;

	public:
		System() = default;
		~System() = default;

		void AddEntityToSystem(Entity entity);
		void RemoveEntityFromSystem(Entity entity);	
		std::vector<Entity> GetSystemEntities() const;
		const Signature& GetComponentSignature() const;

        // Define the components an entity must have
		template <typename TComponent> void RequireComponent();

};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registry: Manages the creation/destruction of Entities.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Registry {
	//TODO:
};


template <typename TComponent> 
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif