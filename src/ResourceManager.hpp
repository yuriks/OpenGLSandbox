#pragma once

#include <boost/noncopyable.hpp>
#include "Texture.hpp"

namespace hw {
namespace rsrc {

template <typename T>
class ResourceManager;

template <typename T>
class ResourceHandle {
public:
	ResourceHandle()
		: manager(nullptr)
	{
	}

	ResourceHandle(const ResourceHandle& o)
		: index(o.index), manager(o.manager)
	{
		manager->increaseResourceRefcount(index);
	}

	ResourceHandle(ResourceHandle&& o)
		: index(o.index), manager(o.manager)
	{
		o.manager = nullptr;
	}

	ResourceHandle& operator =(const ResourceHandle& o) {
		o.manager->increaseResourceRefcount(o.index);
		if (manager)
			manager->decreaseResourceRefcount(index);

		index = o.index;
		manager = o.manager;
	}

	ResourceHandle& operator =(ResourceHandle&& o) {
		if (manager)
			manager->decreaseResourceRefcount(index);

		index = o.index;
		manager = o.manager;
		o.manager = nullptr;
	}

	~ResourceHandle() {
		if (manager)
			manager->decreaseResourceRefcount(index);
	}

	T& operator *() {
		return manager->getResource(index);
	}

	T* operator ->() {
		return &(operator*());
	}

	bool valid() {
		return manager != nullptr;
	}

	ResourceHandle(ResourceManager<T>* manager, size_t index)
		: index(index), manager(manager)
	{
		manager->increaseResourceRefcount(index);
	}

private:
	size_t index;
	ResourceManager<T>* manager;
};

template <typename T>
class ResourceManager : private boost::noncopyable {
public:
	ResourceManager()
		: first_free_resource(0)
	{
		for (size_t i = 0; i < resources.size()-1; ++i) {
			resources[i].next_free = i+1;
		}
		resources.back().next_free = -1;
	}

	~ResourceManager() {
		collectGarbage();
		assert(resource_names.empty());
	}

	// Frees resources with no more references.
	void ResourceManager::collectGarbage() {
		for (auto i = resource_names.begin(), end = resource_names.end(); i != end;) {
			size_t index = i->second;
			assert(refcounts[index] >= 0);

			if (refcounts[index] == 0) {
				T& tex = getResource(index);
				tex.~T();

				resources[index].next_free = first_free_resource;
				first_free_resource = index;

				resource_names.erase(i++);
			} else {
				++i;
			}
		}
	}

	void increaseResourceRefcount(int index) {
		++refcounts[index];
	}

	void decreaseResourceRefcount(int index) {
		--refcounts[index];
	}

	T& getResource(size_t index) {
		return *reinterpret_cast<T*>(resources[index].data);
	}

	ResourceHandle<T> getLoadedResource(const std::string& name) {
		// Check if resource has already been loaded.
		auto existing = resource_names.find(name);
		if (existing != resource_names.end()) {
			return ResourceHandle<T>(this, existing->second);
		} else {
			return ResourceHandle<T>();
		}
	}

protected:
	static const int MAX_RESOURCES = T::MAX_RESOURCES;

	union ResourceNode {
		char data[sizeof(T)];
		size_t next_free;
	};

	ResourceHandle<T> constructResource(const std::string& name, T&& new_resource) {
		size_t resource_index = first_free_resource;
		assert(resource_index != -1);
		first_free_resource = resources[resource_index].next_free;

		refcounts[resource_index] = 0;
		T* shader = new(resources[resource_index].data) T(std::move(new_resource));

		resource_names.emplace(name, resource_index);
		return ResourceHandle<T>(this, resource_index);
	}

	std::unordered_map<std::string, size_t> resource_names;
	std::array<ResourceNode, MAX_RESOURCES> resources;
	std::array<int, MAX_RESOURCES> refcounts;
	size_t first_free_resource;
};

} // namespace rsrc
} // namespace hw
