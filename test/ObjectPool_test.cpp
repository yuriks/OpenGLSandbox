#include <boost/test/unit_test.hpp>

#include "ObjectPool.hpp"

BOOST_AUTO_TEST_CASE(ObjectPoolTest) {
	using hw::ObjectPool;

	ObjectPool<int> pool(10);

	typedef ObjectPool<int>::Handle Handle;

	Handle a = pool.allocate();
	pool.set(a, 123);

	Handle b = pool.allocate();
	pool.set(b, 456);

	BOOST_CHECK_EQUAL(123, pool.get(a));
	BOOST_CHECK_EQUAL(456, pool.get(b));

	// Edge case: De-alloc when capacity full
	Handle tmp_h2[8];
	for (auto& h : tmp_h2) {
		h = pool.allocate();
	}
	for (auto h : tmp_h2) {
		pool.free(h);
	}

	Handle tmp_h[12];
	for (auto& h : tmp_h) {
		h = pool.allocate();
	}

	Handle c = pool.allocate();
	pool.set(c, 789);

	BOOST_CHECK_EQUAL(789, pool.get(c));

	pool.free(a);
	pool.free(b);

	a = pool.allocate();
	b = pool.allocate();

	pool.set(a, 111);
	pool.set(b, 222);

	for (auto h : tmp_h) {
		pool.free(h);
	}

	BOOST_CHECK_EQUAL(111, pool.get(a));
	BOOST_CHECK_EQUAL(222, pool.get(b));
	BOOST_CHECK_EQUAL(789, pool.get(c));

	auto i = pool.begin();
	BOOST_CHECK_EQUAL(222, *i);
	BOOST_CHECK_EQUAL(*i, pool.get(i.handle())); ++i;
	BOOST_CHECK_EQUAL(111, *i); ++i;
	BOOST_CHECK_EQUAL(789, *i); ++i;
	BOOST_CHECK(pool.end() == i);
}