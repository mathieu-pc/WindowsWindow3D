#include <cmath>
#include <windows.h>
#include "Vector.hpp"

struct Plane {
	Vector3D a, b;
	Plane (Vector3D A, Vector3D B) {
		a = A; b = B;
	}
	Vector3D normal() const {
		return Vector3D {
			a[1] * b[2] - a[2] * b[1],
			a[2] * b[0] - a[0] * b[2],
			a[0] * b[1] - a[1] * b[0]
		};
	}
};

LONG shaderF(Vector3D normal, Vector3D light) {
	return LONG ( (1. + normal.unitVector().dot(light) ) / 2 * 255);
};

LONG shaderV(Vector3D normal, Vector3D light, Vector3D pos) {
	if(normal.dot(pos) < 0) {
		return shaderF(-normal, light);
	}
	return shaderF(normal, light);
};

LONG shaderPlane(Plane plane, Vector3D light, Vector3D pos) {
	return shaderV(plane.normal(), light, pos);  // cross product of plane a by plane b
};

LONG shader(Simplex triangle, Vector3D light) {
	return shaderPlane(Plane(triangle.a - triangle.b, triangle.a - triangle.c), light, triangle.a + triangle.b + triangle.c);
};



