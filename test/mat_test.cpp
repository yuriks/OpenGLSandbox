#include <boost/test/unit_test.hpp>
#include <sstream>
#include <string>

#include "mat.hpp"

BOOST_AUTO_TEST_CASE(MatrixClassTest) {
	using namespace hw::math;

	const mat2 a = {{
		{1.0f, 2.0f},
		{3.0f, 4.0f}
	}};

	const mat2 b = {{
		{10.0f, 5.0f},
		{ 2.0f, 2.0f}
	}};

	// Test basic acessors.
	BOOST_CHECK_EQUAL(a(0,0), 1.0f);
	BOOST_CHECK_EQUAL(a(0,1), 2.0f);
	BOOST_CHECK_EQUAL(a(1,0), 3.0f);
	BOOST_CHECK_EQUAL(a(1,1), 4.0f);

	// Test equality, assignment and copying.
	BOOST_CHECK(a == mat2(a));
	BOOST_CHECK(a != b);
	{
		mat2 c = mat2_identity;
		c = a;
		BOOST_CHECK(c != mat2_identity);
		BOOST_CHECK_EQUAL(c, a);
	}

	// Test component-wise arithmetic
	const mat2 a_plus_b = {{
		{11.0f, 7.0f},
		{ 5.0f, 6.0f}
	}};

	BOOST_CHECK_EQUAL(a + b, a_plus_b);
	BOOST_CHECK_EQUAL(a_plus_b - b, a);

	// Test scaling
	{
		const mat2 b2 = {{
			{20.0f, 10.0f},
			{ 4.0f,  4.0f}
		}};

		BOOST_CHECK_EQUAL(b * 2.0f, b2);
		BOOST_CHECK_EQUAL(2.0f * b, b2);
	}

	// Test component-wise negation
	{
		const mat2 minus_a = {{
			{-1.0f, -2.0f},
			{-3.0f, -4.0f}
		}};

		BOOST_CHECK_EQUAL(-a, minus_a);
	}
}

BOOST_AUTO_TEST_CASE(MatrixFuncsTest) {
	using namespace hw::math;

	const mat2 a = {{
		{1.0f, 2.0f},
		{3.0f, 4.0f}
	}};

	const mat2 a_times_a = {{
		{ 7.0f, 10.0f},
		{15.0f, 22.0f}
	}};

	// Test matrix multiplication
	BOOST_CHECK_EQUAL(a * mat2_identity, a);
	BOOST_CHECK_EQUAL(a * a, a_times_a);

	// Test transpose()
	const mat2 a_transposed = {{
		{1.0f, 3.0f},
		{2.0f, 4.0f}
	}};

	BOOST_CHECK_EQUAL(transpose(a), a_transposed);

	// Test printing
	std::ostringstream ss;
	ss << a;

	const std::string a_string =
		"|1 2|\n"
		"|3 4|";
	BOOST_CHECK_EQUAL(ss.str(), a_string);
}
