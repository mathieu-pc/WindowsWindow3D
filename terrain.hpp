#include "Vector.hpp"
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

Vector<float, 1600> makeTerrainHeight() {
	std::srand(0);
	Vector<float, 1600> result;

	for(int i = 0; i < 40; i++) {
		for(int j = 0; j < 40; j++) {
			result[j * 40 + i] = (6 - 12 * exp(- ((i-20.)*(i-20.) + (j-24.)*(j-24.))/16) - 20 * exp(- ((i-20.)*(i-20.) + (j-12.)*(j-12.))/16)) / 2;
		}
	}
	return result;
};

void makePyramid(std::vector<Simplex> & terrain) {
	terrain.push_back(Simplex({
		{0, 0, 10},
		{2, -2, 9},
		{2, 0, 8}
	}));
	terrain.push_back(Simplex({
		{4, 0, 10},
		{2, -2, 9},
		{2, 0, 8}
	}));
}


void makeTerrain(std::vector<Simplex> & terrain) {
	Vector<float, 1600> terrainHeight = makeTerrainHeight();
	for(int i = 0; i < 39; i++) {
		for(int j = 0; j < 39; j++) {
			terrain.push_back(
				Simplex({
				{float(j-19), terrainHeight[j*40+i], float(50 - i)},
				{float(j-19), terrainHeight[j*40+i+1], float(50 - (i+1))},
				{float(j+1-19), terrainHeight[(j+1)*40 + i], float(50 - i)}
				})
			);
		}
		for(int j = 0; j < 39; j++) {
			terrain.push_back(
				Simplex({
				{float(j+1-19), terrainHeight[(j+1) * 40 + i + 1], float(50 - (i + 1))},
				{float(j-19), terrainHeight[ j * 40 + i + 1], float(50 - (i+1))},
				{float(j+1-19), terrainHeight[(j+1) * 40 + i], float(50 - i)}
				})
			);
		}
	}
}