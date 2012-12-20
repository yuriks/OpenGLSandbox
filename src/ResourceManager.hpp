#pragma once

#include <boost/noncopyable.hpp>
#include "Texture.hpp"

namespace hw {
namespace rsrc {

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

protected:
	static const int MAX_RESOURCES = T::MAX_RESOURCES;

	union ResourceNode {
		char data[sizeof(T)];
		size_t next_free;
	};

	std::unordered_map<std::string, size_t> resource_names;
	std::array<ResourceNode, MAX_RESOURCES> resources;
	std::array<int, MAX_RESOURCES> refcounts;
	size_t first_free_resource;
};

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

	ResourceHandle(ResourceManager<T>* manager, size_t index)
		: index(index), manager(manager)
	{
		manager->increaseResourceRefcount(index);
	}

private:
	size_t index;
	ResourceManager<T>* manager;
};

} // namespace rsrc
} // namespace hw
