#include "Vector.hpp"
#include "Bivector.hpp"
#include "draw3D.hpp"
#include "terrain.hpp"

#include <windows.h>
#include <gdiplus.h>
#include <uxtheme.h>
#include <ctime>

#include <functional>
#include <math.h>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>

struct GameState {
	std::vector<Simplex> planes;
	RECT screen;
	Vector3D cameraPosition = {0,0,0};
	Rotor<float, 3> rotorUp {{0, 0, 0.01}};
	Rotor<float, 3> rotorDown {{0, 0, -0.01}};
	Rotor<float, 3> rotorLeft {{0, -0.01, 0}};
	Rotor<float, 3> rotorRight {{0, 0.01, 0}};
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	const char CLASS_NAME[] = "Sample Window Class";
	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		"Main",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (hwnd == NULL) {
		return 0;
	}
	std::srand(0);

	GameState* pState = new GameState;

	makeTerrain(pState -> planes);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pState);
	ShowWindow(hwnd, nCmdShow);

	SetTimer(hwnd, 1, 25, NULL);

	GetClientRect(hwnd, & pState -> screen);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	delete pState;
	return 0;
}

void applyRotationToTriangles(std::vector<Simplex> & triangles, const Rotor<float, 3> rotor) {
	for(Simplex &triangle : triangles) {
		triangle.a = rotor.rotate(triangle.a);
		triangle.b = rotor.rotate(triangle.b);
		triangle.c = rotor.rotate(triangle.c);
	}
}

void processKeydown(GameState * pointerState, const WPARAM windowParam) {
	switch (windowParam) {
		case VK_UP: {
			applyRotationToTriangles(pointerState -> planes, pointerState -> rotorUp);
			return ;
		}
		case VK_DOWN: {
			applyRotationToTriangles(pointerState -> planes, pointerState -> rotorDown);
			return ;
		}
		case VK_LEFT: {
			applyRotationToTriangles(pointerState -> planes, pointerState -> rotorLeft);
			return ;
		}
		case VK_RIGHT: {
			applyRotationToTriangles(pointerState -> planes, pointerState -> rotorRight);
			return ;
		}
	}
}

// Window procedure: handles messages sent to the window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	GameState* pState = (GameState*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	switch (uMsg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			return 0;
		}
		case WM_KEYDOWN: {
			processKeydown(pState, wParam);
			return 0;
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc0 = BeginPaint(hwnd, &ps);
			HDC memDC = CreateCompatibleDC( hdc0 );
			RECT rect;
			GetClientRect(hwnd, &rect);
			HBITMAP bmp = CreateCompatibleBitmap(
				hdc0,
				rect.right - rect.left,
				rect.bottom - rect.top
			);
			HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);
			FillRect(memDC, &rect, (HBRUSH)(COLOR_WINDOW+1));
			Vector3D light = {1,0,0};
			for(const Simplex triangle : pState -> planes) {
				drawTriangle(memDC, triangle - pState->cameraPosition, ScreenParameters {0.8, rect.right - rect.left, rect.bottom - rect.top}, light);
			}
			BitBlt( hdc0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
			SelectObject( memDC, oldBmp);
			DeleteObject( bmp );
			DeleteDC (memDC );
			EndPaint(hwnd, &ps);
			return 0;
		}
		case WM_TIMER: {
			for(Simplex &triangle : pState -> planes) {
				triangle.a[2] -= 0.1;
				triangle.b[2] -= 0.1;
				triangle.c[2] -= 0.1;
			}
			InvalidateRect(hwnd, NULL, TRUE);
			return 0;
		}
		default: {
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}
}
