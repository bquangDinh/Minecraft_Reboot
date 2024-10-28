#include "MeshGenerator.h"

Octree* MeshGenerator::GenerateTerrain(const vec3& dimensions, const vec3& position, const TERRAIN_GENERATION_METHODS method) {
	if (method == TERRAIN_GENERATION_METHODS::PERLIN) {
		return GeneratePerlinTerrain(dimensions, position);
	}

	return nullptr;
}

Octree* MeshGenerator::GeneratePerlinTerrain(const vec3& dimensions, const vec3& position) {
	// Allocate memory for the voxel array
	//Voxel* voxels = new Voxel[dimensions.x * dimensions.y * dimensions.z];
	
	// Check if dimenions is power of 2
	assert(MathUtil::isPowerOf2(dimensions.x) && MathUtil::isPowerOf2(dimensions.y) && MathUtil::isPowerOf2(dimensions.z));

	// Create an Octree
	Octree* octree = new Octree(BoundingBox(0.5f * dimensions, dimensions));

	float height, heightOffset;

	vec4 globalPos;

	mat4 model = glm::translate(mat4(1.0f), position);

	for (int x = 0; x < dimensions.x; x++) {
		for (int y = 0; y < dimensions.y; y++) {
			for (int z = 0; z < dimensions.z; z++) {
				globalPos = model * vec4(vec3(x, y, z), 1.0f);
				
				if (y == 0) {
					octree->insert(vec3(x, y, z), std::make_unique<Voxel>(VOXEL_TYPE::WATER, false, vec3(x, y, z)));

					continue;
				}

				if (perlinNoise3DF(vec3(globalPos), dimensions, height)) {
					if ((int)height == 1) height += 1;

					heightOffset = rand() % ((int)height - 1) + 1;
					
					if (y == 1) {
						octree->insert(vec3(x, y, z), std::make_unique<Voxel>(VOXEL_TYPE::SAND, false, vec3(x, y, z)));
					} else if ((y < height - heightOffset)) {
						octree->insert(vec3(x, y, z), std::make_unique<Voxel>(VOXEL_TYPE::ROCK, false, vec3(x, y, z)));
					}
					else if (y >= height - 1) {
						octree->insert(vec3(x, y, z), std::make_unique<Voxel>(VOXEL_TYPE::GRASS, false, vec3(x, y, z)));
					}
					else {
						octree->insert(vec3(x, y, z), std::make_unique<Voxel>(VOXEL_TYPE::DIRT, false, vec3(x, y, z)));
					}
				}
				else {
					// AIR
					// SKIP
				}
			}
		}
	}

	return octree;
}

Voxel* MeshGenerator::GenerateShape(const vec3& dimensions, const vec3& position, SHAPE_GENERATION_METHODS method) {
	if (method == SHAPE_GENERATION_METHODS::SPHERE) {
		return ShapeGenerator(vec3(0.0f), dimensions, sphereCheckFunc);

	}
	else if (method == SHAPE_GENERATION_METHODS::CUBE) {
		return ShapeGenerator(vec3(0.0f), dimensions, cubeCheckFunc);
	}

	return nullptr;
}

Voxel* MeshGenerator::ShapeGenerator(const vec3 pos, const vec3 dimensions, bool(*checkFunc)(const vec3, const vec3)) {
	// Allocate memory for the voxel array
	Voxel* voxels = new Voxel[dimensions.x * dimensions.y * dimensions.z];

	int index;

	for (int x = 0; x < dimensions.x; x++) {
		for (int y = 0; y < dimensions.y; y++) {
			for (int z = 0; z < dimensions.z; z++) {
				index = MathUtil::flattenIndex(vec3(x, y, z), dimensions);

				if (checkFunc(vec3(x, y, z), dimensions)) {
					voxels[index] = Voxel{ VOXEL_TYPE::GRASS, false, vec3(0) };
				}
				else {
					voxels[index] = Voxel{ VOXEL_TYPE::AIR, true, vec3(0) };
				}
			}
		}
	}

	return voxels;
}

bool MeshGenerator::perlinNoise3DF(vec3 pos, vec3 dimensions, float& height) {
	float scale = 50;

	vec3 normalizedPos = pos / vec3(scale);

	normalizedPos.x = normalizedPos.x * cos(0.5f) - normalizedPos.z * sin(0.5f);
	normalizedPos.z = normalizedPos.x * sin(0.5f) + normalizedPos.z * cos(0.5f);

	float lowFreq = 0.005f;
	float midFreq = 0.05f;
	float highFreq = 0.25f;

	float lowNoise = perlinNoise2D(normalizedPos.x, normalizedPos.y, normalizedPos.z, 6, 0.5f, 2.2f, lowFreq, dimensions.y);
	float midNoise = perlinNoise2D(normalizedPos.x, normalizedPos.y, normalizedPos.z, 6, 0.5f, 2.2f, midFreq, dimensions.y);
	float highNoise = perlinNoise2D(normalizedPos.x, normalizedPos.y, normalizedPos.z, 6, 0.5f, 2.2f, highFreq, dimensions.y);

	float noise = (lowNoise + midNoise + highNoise) * scale;

	height = noise;

	return pos.y < noise;
}

bool MeshGenerator::cubeCheckFunc(const vec3 pos, const vec3 dimensions) {
	return true;
}

bool MeshGenerator::sphereCheckFunc(const vec3 pos, const vec3 dimensions) {
	vec3 center = dimensions / 2.0f;
	float radius = dimensions.x / 2.0f;

	return glm::distance(pos, center) <= radius;
}

float MeshGenerator::perlinNoise2D(
	float x, 
	float y,
	float z,
	float octaves, 
	float persistence,
	float lacunarity,
	float baseFrequency,
	float baseAmplitude
) {
	float frequency = baseFrequency;

	float amplitude = baseAmplitude;

	float totalNoiseValue = 0.0f; // Total noise value accumulated

	float maxNoiseValue = 0.0f; // Maximum noise value, used for normalization

	float noise;

	for (int i = 0; i < octaves; i++) {
		// Sample noise value
		noise = db::perlin(x * frequency, y * frequency, z * frequency) * amplitude;

		totalNoiseValue += noise;

		maxNoiseValue += amplitude;

		// Update frequency and amplitude

		// Frequency increases by a factor of lacunarity
		frequency *= lacunarity;

		// Amplitude decreases by a factor of persistence
		amplitude *= persistence;
	}

	// Normalize the noise value
	return totalNoiseValue / maxNoiseValue;
}