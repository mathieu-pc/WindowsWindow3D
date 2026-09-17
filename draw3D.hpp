#include <windows.h>
#include <map>
#include <cmath>
#include "Vector.hpp"
#include "Shader.hpp"

LONG projectPos(const float x, const float dist, const float fovRatio, const int screenWidth, const int screenLength) {
	return LONG(float(screenWidth) * atan(x/dist) * fovRatio + float(screenLength)/2) ;
};

struct ScreenParameters {
	float fov;
	int width;
	int height;
};

void drawTriangle(HDC hdc, const Simplex triangle, const ScreenParameters screen, const Vector3D light) {
	if(triangle.a[2] < 0 || triangle.b[2] < 0 || triangle.c[2] < 0) {
		return ;
	}
	HPEN hNullPen = (HPEN)GetStockObject(NULL_PEN);
	HPEN oldPen = (HPEN)SelectObject(hdc, hNullPen);
	POINT points[] = {
		{
			projectPos(triangle.a[0], triangle.a[2], screen.fov, screen.width, screen.width),
			projectPos(triangle.a[1], triangle.a[2], screen.fov, screen.width, screen.height)},
		{
			projectPos(triangle.b[0], triangle.b[2], screen.fov, screen.width, screen.width),
			projectPos(triangle.b[1], triangle.b[2], screen.fov, screen.width, screen.height)},
		{
			projectPos(triangle.c[0], triangle.c[2], screen.fov, screen.width, screen.width),
			projectPos(triangle.c[1], triangle.c[2], screen.fov, screen.width, screen.height)}
	};
	LONG lightLevel = shader(triangle, light);
	HBRUSH hBrush = CreateSolidBrush(RGB(shader(triangle, {1,0,0}), shader(triangle, {1,0,0}), shader(triangle, {1,0,0})));
	HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	Polygon(hdc, points, 3);
	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	DeleteObject(hBrush);
};