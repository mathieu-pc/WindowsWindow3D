#ifndef PCMV_HPP
#define PCMV_HPP

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <array>
#include <cmath>


template <class Derived, class Scalar, std::size_t N>
struct _Vector : public std::array<Scalar, N> {
	using Base = std::array<Scalar, N>;
	
	Derived& asDerived() {
		return static_cast<Derived&>(*this);
	}
	
	const Derived& asDerived() const {
		return static_cast<const Derived&>(*this);
	}

	Scalar dot(const _Vector& V) const {
		Scalar result = 0;
		for (size_t i = 0; i != N; ++i) {
			result += (*this)[i] * V[i];
		}
		return result;
	};
	Scalar norm() const {
		Scalar result = 0;
		for (std::size_t i = 0; i != N; ++i) {
			result += (*this)[i]*(*this)[i];
		}
		return sqrt((*this).dot((*this)));
	};
	Derived unitVector() const {
		return (*this)/(*this).norm();
	};

	Derived operator/(const Scalar scalar) const {
		Derived result;
		for (std::size_t i = 0; i != N; ++i) {
			result[i] = (*this)[i] / scalar;
		}
		return result;
	}

	Derived operator+(const _Vector& V) const {
		Derived result;
		for (std::size_t i = 0; i != N; ++i) {
			result[i] = (*this)[i] + V[i];
		}
		return result;
	};
	Derived operator-(const _Vector& V) const {
		Derived result;
		for (std::size_t i = 0; i != N; ++i) {
			result[i] = (*this)[i] - V[i];
		}
		return result;
	};
	Derived operator-() const {
		Derived result;
		for(std::size_t i = 0; i != N; ++i) {
			result[i] = -(*this)[i];
		}
		return result;
	};
	Derived& operator+=(const Derived& vector){
		auto& self = asDerived();
		for(std::size_t i = 0; i != N; ++i) {
			self[i] += vector[i];
		}
		return self;
	}
	Derived& operator-=(const Derived& vector){
		auto& self = asDerived();
		for(std::size_t i = 0; i != N; ++i) {
			self[i] -= vector[i];
		}
		return self;
	}
	friend std::ostream& operator<<(std::ostream& os, const _Vector& V) {
		os << '(' << V[0];
		for (std::size_t i = 1; i != N; ++i) {
			os << ',' << V[i];
		}
		os <<')';
		return os;
	};
};

template <class Scalar, std::size_t N> class Vector: public _Vector<Vector<Scalar, N>, Scalar, N> {
public:
	using Base = _Vector<Vector, Scalar, N>;
};

using Vector3D = Vector<float, 3>;
struct Simplex {
	Vector3D a;
	Vector3D b;
	Vector3D c;
	const Simplex operator-(const Vector3D& vector) const {
		return Simplex{
			this -> a - vector,
			this -> b - vector,
			this -> c - vector
		};
	};
	Simplex& operator+=(const Vector3D& vector){
		this -> a += vector;
		this -> b += vector;
		this -> c += vector;
		return *this;
	};
	Simplex& operator-=(const Vector3D& vector){
		this -> a -= vector;
		this -> b -= vector;
		this -> c -= vector;
		return *this;
	};
};

#endif