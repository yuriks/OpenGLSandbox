#pragma once

#include <cassert>
#include <cmath>
#include "vec.hpp"

namespace hw {
namespace math {

template<unsigned int R, unsigned int C = R, typename T = float>
struct mat {
	// Rows
	vec<C, T> data[R];

	// Basic element acessors
	inline T& operator ()(unsigned int i, unsigned int j) {
		return data[i][j];
	}

	inline const T& operator ()(unsigned int i, unsigned int j) const {
		return data[i][j];
	}

	inline bool operator ==(const mat& o) const {
		bool eq = true;
		for (unsigned int i = 0; i < R; ++i) {
			eq = eq && data[i] == o.data[i];
		}
		return eq;
	}

	inline bool operator !=(const mat& o) const {
		return !(*this == o);
	}

	// Component-wise matrix arithmetic
#define DEFINE_OP(op) \
	inline mat& operator op##=(const mat& o) { \
		for (unsigned int i = 0; i < R; ++i) { \
			data[i] op##= o.data[i]; \
		} \
		return *this; \
	} \
	inline mat operator op(const mat& o) const { \
		mat r = *this; \
		return r op##= o; \
	}

	DEFINE_OP(+)
	DEFINE_OP(-)

#undef DEFINE_OP

	// Scalar multiplication
	inline mat& operator *=(const T a) {
		for (unsigned int i = 0; i < R; ++i) {
			data[i] *= a;
		}
		return *this;
	}

	inline mat operator *(const T a) const {
		mat r = *this;
		return r *= a;
	}

	inline mat operator -() const {
		mat r = *this;
		for (unsigned int i = 0; i < R; ++i) {
			r.data[i] = -r.data[i];
		}
		return r;
	}

	template<typename T2>
	inline mat<R,C,T2> typecast() const {
		mat<R,C,T2> r;
		for (unsigned int i = 0; i < R; ++i) {
			r.data[i] = data[i].typecast<T2>();
		}
		return r;
	}
};

template<unsigned int R, unsigned int C, typename T>
inline mat<R,C,T> operator *(const T a, const mat<R,C,T>& m) {
	return m * a;
}

// Convenience typedefs for the most common kinds of matrices.
typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

// Convenient constants for matrices.
static const mat2 mat2_identity = {{
	{1, 0},
	{0, 1}
}};
static const mat3 mat3_identity = {{
	{1, 0, 0},
	{0, 1, 0},
	{0, 0, 1}
}};
static const mat4 mat4_identity = {{
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{0, 0, 0, 1}
}};

template<unsigned int M, unsigned int N, unsigned int P, typename T>
mat<M,P,T> operator *(const mat<M,N,T>& a, const mat<N,P,T>& b) {
	mat<M,P> r;

	for (unsigned int i = 0; i < M; ++i) {
		for (unsigned int j = 0; j < N; ++j) {
			T s = 0;
			for (unsigned int k = 0; k < P; ++k) {
				s += a(i,k) * b(k,j);
			}
			r(i,j) = s;
		}
	}

	return r;
}

// Column vector version
template<unsigned int N, typename T>
inline vec<N,T> vec_from_mat(const mat<N,1,T>& m) {
	vec<N> v;
	
	for (unsigned int i = 0; i < N; ++i) {
		v[i] = m(i, 0);
	}

	return v;
}

// Row vector version
template<unsigned int N, typename T>
inline vec<N,T> vec_from_mat(const mat<1,N,T>& m) {
	return m.data[0];
}

template<unsigned int R, unsigned int C, typename T>
mat<C,R,T> transpose(const mat<R,C,T>& m) {
	mat<C,R,T> t;

	for (unsigned int i = 0; i < R; ++i) {
		for (unsigned int j = 0; j < C; ++j) {
			t(j, i) = m(i, j);
		}
	}

	return t;
}

// Prints matrix m to stream.
template<unsigned int R, unsigned int C, typename T>
std::ostream& operator <<(std::ostream& s, const mat<R,C,T>& m) {
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