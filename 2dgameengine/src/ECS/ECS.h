#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <typeindex>
#include <unordered_map>
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

		Entity CreateEntity();

		void Update();

		void AddEntityToSystem();

		// KillEntity()
		// AddComponent(Entity entity)
		// RemoveComponent(Entity entity)
		// HasComponent(Entity entity)
		//
		// AddSystem()
		// RemoveSystem()
		// HasSystem()
		// GetSystem()
		
};


template <typename TComponent> 
void System::RequireComponent() {
	const auto componentId = Component<TComponent>::GetId();
	componentSignature.set(componentId);
}

#endif