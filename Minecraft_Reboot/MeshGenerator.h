#pragma once

namespace siv {
	class PerlinNoise;
}

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <array>
#include <random>
#include <iostream>
#include <memory>

#include "Voxel.h"
#include "VoxelUtil.h"
#include "MathUtil.h"
#include "Octree.h"

#define DB_PERLIN_IMPL
#include "db_perlin.hpp"

using namespace glm;

enum TERRAIN_GENERATION_METHODS {
	SIMPLEX,
	PERLIN
};

enum SHAPE_GENERATION_METHODS {
	CUBE,
	SPHERE
};

class MeshGenerator
{
public:
	static Octree* GenerateTerrain(const vec3& dimensions, const vec3& position, const TERRAIN_GENERATION_METHODS method);
	static Voxel* GenerateShape(const vec3& dimensions, const vec3& position, SHAPE_GENERATION_METHODS method);

private:
	static Octree* GeneratePerlinTerrain(const vec3& dimensions, const vec3& position);

	static Voxel* ShapeGenerator(const vec3 pos, const vec3, bool(*checkFunc)(const vec3, const vec3));

	static bool perlinNoise3DF(vec3 pos, vec3 dimensions, float& height);

	static bool cubeCheckFunc(const vec3 pos, const vec3 dimensions);

	static bool sphereCheckFunc(const vec3 pos, const vec3 dimensions);

	static float perlinNoise2D(float x, float y, float z, float octaves = 4, float persistence = 0.5f, float lacunarity = 2.0f, float baseFrequency = 1.0f, float baseAmplitude = 1.0f);
};

