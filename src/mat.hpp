#pragma once

#include <cassert>
#include <cmath>
#include "vec.hpp"

namespace hw {
namespace math {

template<unsigned int R, unsigned int C = R>
struct mat {
	// Rows
	vec<C> data[R];

	// Basic element acessors
	inline float& operator ()(unsigned int i, unsigned int j) {
		return data[i][j];
	}

	inline const float& operator ()(unsigned int i, unsigned int j) const {
		return data[i][j];
	}

	inline bool operator ==(const mat<R,C>& o) const {
		bool eq = true;
		for (unsigned int i = 0; i < R; ++i) {
			eq = eq && data[i] == o.data[i];
		}
		return eq;
	}

	inline bool operator !=(const mat<R,C>& o) const {
		return !(*this == o);
	}

	// Component-wise matrix arithmetic
#define DEFINE_OP(op) \
	inline mat<R,C>& operator op##=(const mat<R,C>& o) { \
		for (unsigned int i = 0; i < R; ++i) { \
			data[i] op##= o.data[i]; \
		} \
		return *this; \
	} \
	inline mat<R,C> operator op(const mat<R,C>& o) const { \
		mat<R,C> r = *this; \
		return r op##= o; \
	}

	DEFINE_OP(+)
	DEFINE_OP(-)

#undef DEFINE_OP

	// Scalar multiplication
	inline mat<R,C>& operator *=(const float a) {
		for (unsigned int i = 0; i < R; ++i) {
			data[i] *= a;
		}
		return *this;
	}

	inline mat<R,C> operator *(const float a) const {
		mat<R,C> r = *this;
		return r *= a;
	}

	inline mat<R,C> operator -() const {
		mat<R,C> r = *this;
		for (unsigned int i = 0; i < R; ++i) {
			r.data[i] = -r.data[i];
		}
		return r;
	}
};

template<unsigned int R, unsigned int C>
inline mat<R,C> operator *(const float a, const mat<R,C>& m) {
	return m * a;
}

// Convenience typedefs for the most common kinds of matrices.
typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

// Convenient constants for matrices.
static const mat2 mat2_identity = {{
	{1.0f, 0.0f},
	{0.0f, 1.0f}
}};
static const mat3 mat3_identity = {{
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}
}};
static const mat4 mat4_identity = {{
	{1.0f, 0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 0.0f, 1.0f}
}};

template<unsigned int M, unsigned int N, unsigned int P>
mat<M,P> operator *(const mat<M,N>& a, const mat<N,P>& b) {
	mat<M,P> r;

	for (unsigned int i = 0; i < M; ++i) {
		for (unsigned int j = 0; j < N; ++j) {
			float s = 0.0f;
			for (unsigned int k = 0; k < P; ++k) {
				s += a(i,k) * b(k,j);
			}
			r(i,j) = s;
		}
	}

	return r;
}

template<unsigned int R, unsigned int C>
mat<C,R> transpose(const mat<R,C>& m) {
	mat<C,R> t;

	for (unsigned int i = 0; i < R; ++i) {
		for (unsigned int j = 0; j < C; ++j) {
			t(j, i) = m(i, j);
		}
	}

	return t;
}

// Prints matrix m to stream.
template<unsigned int R, unsigned int C>
std::ostream& operator <<(std::ostream& s, const mat<R,C>& m) {
	for (unsigned int i = 0; i < R; ++i) {
		s << '|';
		for (unsigned int j = 0; j < C-1; ++j) {
			s << m(i, j) << ' ';
		}
		s << m(i, C-1) << '|';
		if (i < R-1)
			s << '\n';
	}

	return s;
}

}
}	