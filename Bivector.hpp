#ifndef PCMB_HPP
#define PCMB_HPP

#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <array>
#include <cmath>

constexpr std::size_t distinctPairs(const std::size_t N) {
	return (N * (N-1)) >> 1;
}

template <class Derived, class Scalar, std::size_t N>
struct _Bivector : public std::array<Scalar, distinctPairs(N)> {
	using Base = std::array<Scalar, distinctPairs(N)>;

	Scalar get(const std::size_t index1, const std::size_t index2) const {
		if(index1 == index2) {
			return Scalar{0};
		}
		if(index1 > index2) {
			return -1 * (*this).get(index2, index1);
		}
		return (*this)[index1 + index2 - 1];
	};

	Scalar getSymmetric(const std::size_t index1, const std::size_t index2) const {
		if(index1 == index2) {
			return Scalar{0};
		}
		return (*this)[index1 + index2 - 1];
	}

	void set(const std::size_t index1, const std::size_t index2, const Scalar value) {
		if(index1 == index2) {
			return ;
		}
		if(index1 > index2) {
			return (*this).set(index2, index1, -value);
		}
		(*this)[index1 + index2 - 1] = value;
	};

	void setSymmetric(const std::size_t index1, const std::size_t index2, const Scalar value) {
		if(index1 == index2) {
			return ;
		}
		(*this)[index1 + index2 - 1] = value;
	};

	Derived operator+(const _Bivector& B) const {
		Derived result;
		std::size_t M = distinctPairs(N);
		for (std::size_t i = 0; i != M; ++i) {
			result[i] = (*this)[i] + B[i];
		}
		return result;
	};
};
template <class Scalar, std::size_t N> class Bivector: public _Bivector<Bivector<Scalar, N>, Scalar, N> {
public:
	using Base = _Bivector<Bivector, Scalar, N>;
};

template <class Scalar, std::size_t N> Bivector<Scalar, N> operator^(const Vector<Scalar, N>& V1, const Vector<Scalar, N>& V2) {
	Bivector<Scalar, N> result;
	for (std::size_t i = 0; i != N - 1; ++i) {
		for(std::size_t j = i + 1; j != N; ++j) {
			result.set(i, j, V1[i] * V2[j] - V2[i] * V1[j]);
		}
	}
	return result;
};



template <class Scalar, std::size_t N>
class Rotor {
	Bivector<Scalar, N> angles, cosines, sines;

	public:

	void set(const std::size_t index1, const std::size_t index2, const Scalar value) {
		angles.set(index1, index2, value);
		cosines.setSymmetric(index1, index2, cos(value));
		sines.set(index1, index2, sin(value));
	}

	Rotor<Scalar, N> (const Bivector<Scalar, N> angles) {
		for(std::size_t i = 0; i != N; ++i) {
			for(std::size_t j = 0; j < i; ++j) {
				set(i, j, angles.get(i, j));
			}
		}
	}

	const Vector<Scalar, N> rotateSingleAxis(const Vector<Scalar, N> vector, const std::size_t index1, const std::size_t index2) const {
		Vector<Scalar, N> result = vector;
		result[index1] = vector[index1] * cosines.getSymmetric(index1, index2) + vector[index2] * sines.get(index2, index1);
		result[index2] = vector[index2] * cosines.getSymmetric(index2, index1) + vector[index1] * sines.get(index1, index2);
		return result;
	}

	const Vector<Scalar, N> rotate(const Vector<Scalar, N> vector) const {
		Vector<Scalar, N> result = vector;
		// In 3D, it is possible to simplify the rotation by converting into a single bivector orientation.
		// This should be handled in an if constexpr.
		for(std::size_t i = 0; i != N; ++i) {
			for(std::size_t j = i + 1; j != N; ++j) {
				result = rotateSingleAxis(result, i, j);
			}
		}
		return result;
	}
};

#endif