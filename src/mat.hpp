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

	inline float& operator ()(unsigned int i, unsigned int j) {
		ASSERT(i < R && j < C);
		return data[i][j];
	}

	inline const float& operator [](unsigned int i) const {
		ASSERT(i < R && j < C);
		return data[i][j];
	}

#define DEFINE_OP(op) \
	inline mat<R,C>& operator op=(const mat<R,C>& o) { \
		for (unsigned int i = 0; i < R; ++i) { \
			data[i] op= o.data[i]; \
		} \
		return *this; \
	} \
	inline mat<R,C> operator op(const mat<R,C>& o) { \
		mat<R,N> r = *this; \
		return r op= o; \
	}

	DEFINE_OP(+)
	DEFINE_OP(-)

#undef DEFINE_OP

#define DEFINE_SCALAR_OP(op) \
	inline mat<R,C>& operator op=(const float a) { \
		for (unsigned int i = 0; i < N; ++i) { \
			data[i] op= a; \
		} \
		return *this; \
	} \
	inline mat<R,C> operator op(const float a) { \
		mat<R,N> r = *this; \
		return r op= a; \
	}

	DEFINE_SCALAR_OP(*)
	DEFINE_SCALAR_OP(/)

#undef DEFINE_SCALAR_OP

	inline mat<R,C> operator -() {
		mat<R,C> r = *this;
		for (unsigned int i = 0; i < N; ++i) {
			r.data[i] = -r.data[i];
		}
		return r;
	}
};

typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

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

}
}	