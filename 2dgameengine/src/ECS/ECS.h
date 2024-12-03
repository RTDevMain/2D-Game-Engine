#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
#include <set>

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

		// Testing operator overloading
		Entity& operator = (const Entity& other) = default;
		bool operator == (const Entity& other) const {return id == other.id;}
		bool operator != (const Entity& other) const {return id != other.id;}
		bool operator > (const Entity& other) const {return id > other.id;}	
		bool operator < (const Entity& other) const {return id < other.id;}	
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
// Pool: A vector (contiguous data) of objects of type T
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class IPool {
	public:
		virtual ~IPool() {}
};


template <typename T>
class Pool: IPool {
	private:
		std::vector<T> data;


	public:
		Pool(int size = 100) {
			data.resize(size);
		}

		virtual ~Pool() = default;	

		bool isEmpty() const {
			return data.empty();
		}

		int GetSize() const {
			return data.size();
		}

		void Resize(int n) {
			data.resize(n);
		}

		void Clear() {
			data.clear();
		}

		void Add(T object) {
			data.push_back(object);
		}

		void Set(int index, T object) {
			data[index] = object;
		}

		T& Get(int index) {
			return static_cast<T&>(data[index]);
		}

		T& operator [](unsigned int index) {
			return data[index];
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Registry: Manages the creation/destruction of Entities.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Registry {
	private:
		int numEntities = 0;

		// Each pool contains all data for a certain component type
		// [Vector index = component type id]
		// [Pool index = entity id]
		std::vector<IPool*> componentPools;

		// Per entity component signatures, implies which components are on/off
		// [Vector index = entity id]
		std::vector<Signature> entityComponentSignatures;

		std::unordered_map<std::type_index, System*> systems;

		// Set of entities that are flagged to be added/removed in next registry update
		std::set<Entity> entitiesToBeAdded;
		std::set<Entity> entitiesToBeRemoved;

	public:
		Registry() = default;

		// TODO:
		void Update();

		Entity CreateEntity();


		void AddEntityToSystem();

		template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

		template <typename T> void RemoveComponent(Entity entity);

		template <typename T> bool HasComponent(Entity entity);

		
		
		template <typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
		template <typename TSystem> void RemoveSystem();
		template <typename TSystem> bool HasSystem() const;
		template <typename TSystem> TSystem& GetSystem() const;

		// Checks the component signature of an entity and adds it to systems that are interested
		void AddEntityToSystems(Entity entity);
		
};


template <typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
	TSystem* newSystem(new TSystem(std::forward<TArgs>(args)...));
	systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
}

template <typename TSystem>
void Registry::RemoveSystem(){
	auto system = systems.find(std::type_index(typeid(TSystem)));
	systems.erase(system);
}

template <typename TSystem>
bool Registry::HasSystem() const {
	return systems.find(std::type_index(typeid(TSystem)))!= systems.end();
}

template <typename TSystem>
TSystem& Registry::GetSystem() const {
	auto system = systems.find(std::type_index(typeid(TSystem)));

	// Don't use '.' when working with pointer use -> instead //
	return *(std::static_pointer_cast<TSystem>(system->second));
}


template <typename TComponent> 
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}



template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
	const auto componentId = Component<TComponent>::GetId();
	const auto entityId = entity.GetId();

	// If 

	if (componentId >= componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}

	if (!componentPools[componentId]) {
		Pool<TComponent>* newComponentPool = new Pool<TComponent>();
		componentPools[componentId] = newComponentPool;
	}

	Pool<TComponent>* componentPool = componentPools[componentId];

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(numEntities);
	}

	TComponent newComponent(std::forward<TArgs>(args)...);
	componentPool->Set(entityId, newComponent);
	entityComponentSignatures[entityId].set(componentId);

}

template <typename T>
void Registry::RemoveComponent(Entity entity) {
	const auto componentId = Component<T>::GetId();
	const auto entityId = entity.GetId();

	entityComponentSignatures[entityId].set(componentId, false);
}

template <typename T>
bool Registry::HasComponent(Entity entity) {
	const auto componentId = Component<T>::GetId();
	const auto entityId = entity.GetId();

	return entityComponentSignatures[entityId].test(componentId);
}

#endif