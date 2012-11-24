#include <boost/test/unit_test.hpp>

#include "vec.hpp"

BOOST_AUTO_TEST_CASE(VectorClassTest) {
	using namespace hw::math;

	const vec3 a  = {2.0f, 3.0f, 4.0f};

	// Test basic acessors
	BOOST_CHECK_EQUAL(a[0], 2.0f);
	BOOST_CHECK_EQUAL(a[1], 3.0f);
	BOOST_CHECK_EQUAL(a[2], 4.0f);

	// Test equality, assignment and copying
	BOOST_CHECK(a == vec3(a));
	BOOST_CHECK(vec3_1 != vec3_0);

	{
		vec3 b = vec3_1;
		b = vec3_0;
		BOOST_CHECK(b != vec3_1);
		BOOST_CHECK_EQUAL(b, vec3_0);
	}

	// Test vector ops
	{
		vec3 b = {3.0f, 4.0f, 5.0f};
		BOOST_CHECK_EQUAL(a + vec3_1, b);
		BOOST_CHECK_EQUAL(b - vec3_1, a);

		BOOST_CHECK_EQUAL(a * a, mvec(4.0f, 9.0f, 16.0f));
		BOOST_CHECK_EQUAL(a / a, vec3_1);
	}

	// Test scalar ops
	{
		vec3 b = {4.0f, 6.0f, 8.0f};
		BOOST_CHECK_EQUAL(a * 2.0f, b);
		BOOST_CHECK_EQUAL(2.0f * a, b);

		BOOST_CHECK_EQUAL(-a, mvec(-2.0f, -3.0f, -4.0f));
	}
}

BOOST_AUTO_TEST_CASE(VectorFuncsTest) {
	using namespace hw::math;

	const vec3 a  = {2.0f, 3.0f, 4.0f};

	// Test dot product
	BOOST_CHECK_EQUAL(dot(vec3_1, vec3_0), 0.0f);
	BOOST_CHECK_EQUAL(dot(vec3_1, vec3_1), 3.0f);
	BOOST_CHECK_EQUAL(dot(vec3_1, a),  9.0f);
	BOOST_CHECK_EQUAL(dot(a,  a), 29.0f);

	// Test cross product
	BOOST_CHECK_EQUAL(cross(vec3_0, vec3_0), vec3_0);
	BOOST_CHECK_EQUAL(cross(vec3_x, vec3_y), vec3_z);

	// Test length
	BOOST_CHECK_EQUAL(length(vec3_0), 0.0f);
	BOOST_CHECK_EQUAL(length(vec3_x), 1.0f);
	BOOST_CHECK_EQUAL(length(vec3_1), std::sqrtf(3.0f));

	// Test normalized
	BOOST_CHECK_EQUAL(normalized(vec3_x), vec3_x);
	//BOOST_CHECK_EQUAL(normalized(mvec(2.0f, 4.0f), );
}