#pragma once

#include <vector>
#include <cassert>

namespace hw {

template<typename T>
class ObjectPool;

template<typename T>
class ObjectPoolIterator {
public:
	T operator *() {
		assert(h != -1);
		return pool->get(h);
	}

	typename ObjectPool<T>::Handle handle() const {
		return h;
	}

	ObjectPoolIterator& operator ++() {
		assert(h != -1);
		h = pool->object_pool[h].next;

		return *this;
	}

	ObjectPoolIterator& operator --() {
		assert(h != -1);
		h = pool->object_pool[h].prev;

		return *this;
	}

	bool operator ==(const ObjectPoolIterator& o) const {
		return pool == o.pool && h == o.h;
	}

	bool operator !=(const ObjectPoolIterator& o) const {
		return !(*this == o);
	}

private:
	ObjectPoolIterator(const ObjectPool<T>* pool, typename ObjectPool<T>::Handle h)
		: pool(pool), h(h)
	{ }

	const ObjectPool<T>* pool;
	typename ObjectPool<T>::Handle h;

	friend class ObjectPool<T>;
};

template<typename T>
class ObjectPool {
public:
	typedef size_t Handle;
	typedef ObjectPoolIterator<T> iterator;

	ObjectPool(size_t initial_capacity)
		: first_free_index(-1), first_alloc_index(-1)
	{
		expand_list(initial_capacity);
	}

	Handle allocate() {
		if (first_free_index == -1)
			expand_list(object_pool.size() + 1);

		size_t new_obj = first_free_index;
		first_free_index = object_pool[new_obj].next;

		object_pool[new_obj].next = first_alloc_index;
		object_pool[new_obj].prev = -1;
		
		if (first_alloc_index != -1)
			object_pool[first_alloc_index].prev = new_obj;
		first_alloc_index = new_obj;

		return new_obj;
	}

	void free(Handle handle) {
		size_t next = object_pool[handle].next;
		size_t prev = object_pool[handle].prev;

		if (next != -1) {
			object_pool[next].prev = prev;
		}

		if (prev != -1) {
			object_pool[prev].next = next;
		} else {
			first_alloc_index = next;
		}

		object_pool[handle].next = first_free_index;
		first_free_index = handle;
	}

	T get(Handle handle) const {
		return object_pool[handle].object;
	}

	void set(Handle handle, const T& object) {
		object_pool[handle].object = object;
	}

	iterator begin() {
		return iterator(this, first_alloc_index);
	}

	iterator end() {
		return iterator(this, -1);
	}

private:
	struct ObjectNode {
		T object;
		size_t next;
		size_t prev;
	};

	void expand_list(size_t new_capacity) {
		if (new_capacity <= object_pool.size())
			return;

		size_t old_first_free = first_free_index;

		first_free_index = object_pool.size();

		object_pool.resize(new_capacity);
		object_pool.resize(object_pool.capacity());

		for (size_t i = first_free_index; i < object_pool.size()-1; ++i) {
			object_pool[i].next = i+1;
		}
		object_pool.back().next = old_first_free;
	}

	std::vector<ObjectNode> object_pool;
	size_t first_free_index;
	size_t first_alloc_index;

	friend class ObjectPoolIterator<T>;
};

} // namespace hw
