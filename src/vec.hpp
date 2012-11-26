#pragma once

#include <cassert>
#include <cmath>
#include <ostream>

namespace hw {
namespace math {

template<unsigned int N>
struct vec {
	float data[N == 3 ? 4 : N];

	inline float& operator [](unsigned int i) {
		assert(i < N);
		return data[i];
	}

	inline const float& operator [](unsigned int i) const {
		assert(i < N);
		return data[i];
	}

	inline bool operator ==(const vec<N> o) const {
		bool eq = true;
		for (unsigned int i = 0; i < N; ++i) {
			eq = eq && data[i] == o[i];
		}
		return eq;
	}

	inline bool operator !=(const vec<N> o) const {
		return !(*this == o);
	}

#define DEFINE_OP(op) \
	inline vec<N>& operator op##=(const vec<N> o) { \
		for (unsigned int i = 0; i < N; ++i) { \
			data[i] op##= o[i]; \
		} \
		return *this; \
	} \
	inline vec<N> operator op(const vec<N> o) const { \
		vec<N> r = *this; \
		return r op##= o; \
	}

	DEFINE_OP(+)
	DEFINE_OP(-)
	DEFINE_OP(*)
	DEFINE_OP(/)

#undef DEFINE_OP

	inline vec<N>& operator *=(const float a) {
		for (unsigned int i = 0; i < N; ++i) {
			data[i] *= a;
		}
		return *this;
	}

	inline vec<N> operator *(const float a) const {
		vec<N> r = *this;
		return r *= a;
	}

	inline vec<N> operator -() const {
		vec<N> r = *this;
		for (unsigned int i = 0; i < N; ++i) {
			r[i] = -r[i];
		}
		return r;
	}
};

template<unsigned int N>
inline vec<N> operator *(const float a, const vec<N> v) {
	return v * a;
}

// Convenience typedefs for the most popular kinds of vector.
typedef vec<2> vec2;
typedef vec<3> vec3;
typedef vec<4> vec4;

// Convenience functions for constructing vectors inline.
inline vec2 mvec(float x, float y) {
	vec2 v = {x, y};
	return v;
}

inline vec3 mvec(float x, float y, float z) {
	vec3 v = {x, y, z};
	return v;
}

inline vec4 mvec(float x, float y, float z, float w) {
	vec4 v = {x, y, z, w};
	return v;
}

// Convenient constants for vectors
static const vec3 vec3_x = {1.0f, 0.0f, 0.0f};
static const vec3 vec3_y = {0.0f, 1.0f, 0.0f};
static const vec3 vec3_z = {0.0f, 0.0f, 1.0f};
static const vec3 vec3_1 = {1.0f, 1.0f, 1.0f};
static const vec3 vec3_0 = {0.0f, 0.0f, 0.0f};

// Computes dot-product of vectors a and b.
template<unsigned int N>
inline float dot(const vec<N> a, const vec<N> b) {
	float r = 0.0f;
	for (unsigned int i = 0; i < N; ++i) {
		r += a[i] * b[i];
	}
	return r;
}

// Computes cross-product of vectors a and b.
inline vec3 cross(const vec3 a, const vec3 b) {
	vec3 r = {a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]};
	return r;
}

// Computes euclidean length of vector v.
template<unsigned int N>
inline float length(const vec<N> v) {
	return std::sqrt(dot(v, v));
}

// Normalizes vector v.
template<unsigned int N>
inline vec<N> normalized(const vec<N> v) {
	return v * (1.0f / length(v));
}

// Prints vector v to stream in the format "<x y z>".
template<unsigned int N>
std::ostream& operator <<(std::ostream& s, const vec<N> v) {
	s << '<';
	for (unsigned int i = 0; i < N-1; ++i) {
		s << v[i] << ' ';
	}
	return s << v[N-1] << '>';
}

}
}