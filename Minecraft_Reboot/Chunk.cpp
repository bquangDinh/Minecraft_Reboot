#include "Chunk.h"

const float Chunk::VOXEL_UNIT = 1.0f;

Chunk::Chunk(const vec3 position, const vec3 dimensions) :
	position(position), 
	dimensions(dimensions),
	initialized(false)
{
	model = glm::translate(mat4(1.0f), position);

	meshBuilder = new MeshBuilder();
}

Chunk::~Chunk()
{
	delete voxelsData;

	delete meshBuilder;

	voxelsData = nullptr;

	meshBuilder = nullptr;
}

void Chunk::init()
{
	if (initialized) return;

	// Make sure dimensions are integers
	dimensions = vec3(
		(int)dimensions.x,
		(int)dimensions.y,
		(int)dimensions.z
	);

	//voxelsData = MeshGenerator::GenerateShape(dimensions, SHAPE_GENERATION_METHODS::SPHERE);

	voxelsData = MeshGenerator::GenerateTerrain(dimensions, position, TERRAIN_GENERATION_METHODS::PERLIN);

	cout << "About to do meshing" << endl;

	// Perform meshing on voxels data
	greedyMeshing();

	cout << "Done meshing" << endl;

	// We no longer need to voxels data since the data has been transferred to VBO
	delete voxelsData;

	voxelsData = nullptr;

	initialized = true;
}

void Chunk::update(float deltaTime)
{
	//
}

void Chunk::render(float deltaTime)
{
	if (!initialized) return;

	// Generate VBO
	meshBuilder->generateVBO();

	meshBuilder->render();
}

void Chunk::destroy()
{
	meshBuilder->cleanUp();

	delete meshBuilder;

	meshBuilder = nullptr;

	//delete voxelsData;
	
	//voxelsData = nullptr;
}

int Chunk::getActualFaceIndex(int direction, bool backface)
{
	if (direction == 0) {
		// YZ Plane
		return backface ? SIDES::LEFT : SIDES::RIGHT;
	}
	else if (direction == 1) {
		// XZ Plane
		return backface ? SIDES::BOTTOM : SIDES::TOP;
	}
	else if (direction == 2) {
		// XY Plane
		return backface ? SIDES::BACK : SIDES::FRONT;
	}

	throw std::runtime_error("Invalid direction");
}

bool Chunk::isFaceBlocking(vec3 pos, int direction, bool backface) {
	// Get the voxel in front of the current voxel
	pos[direction] += backface ? -1 : 1;

	// If the considered voxel is outside the chunk, then it is invibible,
	// Thus the face is visible
	if (pos[direction] < 0 || pos[direction] >= dimensions[direction]) {
		return false;
	}

	int consideredIndex = MathUtil::flattenIndex(pos, dimensions);

	assert(consideredIndex >= 0 && consideredIndex < dimensions.x * dimensions.y * dimensions.z);

	// If the considered voxel is transparent, then the face is visible
	if (voxelsData[consideredIndex].transparent) {
		return false;
	}

	return true;
}

bool Chunk::shouldMergeTwoVoxel(vec3 currentPos, vec3 comparingPos, int direction, bool backface) {
	Voxel v1 = voxelsData[MathUtil::flattenIndex(currentPos, dimensions)];

	Voxel v2 = voxelsData[MathUtil::flattenIndex(comparingPos, dimensions)];

	// Only merge two voxels if they are the same type and they are solid
	// And the comparing voxel is not blocked by another voxel
	return v1 == v2 && v2.isSolid() && !isFaceBlocking(comparingPos, direction, backface);
}

void Chunk::stupidMeshing()
{
	if (voxelsData == nullptr) {
		throw std::runtime_error("Voxels data is null. You may forgot to call MeshGenerator before this call");
	}

	int direction, workAxis1, workAxis2, face;
	vec3 startPos, quadSize, m, n, offsetPos;
	vec4 p1, p2, p3, p4;
	bool backface;

	// Iterate through each face and generate the mesh for each face of the quads
	for (face = 0; face < NUM_VOX_SIDES; ++face) {
		backface = face % 2 != 0;

		// direction could be front-back, left-right, top-bottom
		direction = face % 3;

		// workAxis1 and workAxis2 are the other two axis
		workAxis1 = (direction + 1) % 3;
		workAxis2 = (direction + 2) % 3;

		startPos = vec3();

		// Iterate through each voxel in the chunk
		// start from 0 to the end of each chunk in that direction
		for (startPos[direction] = 0; startPos[direction] < dimensions[direction]; ++startPos[direction]) {
			for (startPos[workAxis1] = 0; startPos[workAxis1] < dimensions[workAxis1]; ++startPos[workAxis1]) {
				for (startPos[workAxis2] = 0; startPos[workAxis2] < dimensions[workAxis2]; ++startPos[workAxis2]) {
					// Get the voxel at the current position
					Voxel voxel = voxelsData[MathUtil::flattenIndex(startPos, dimensions)];

					// If the voxel is not visible, skip it
					if (voxel.transparent) continue;

					// Get the size of the quad
					quadSize[workAxis1] = VOXEL_UNIT;

					quadSize[workAxis2] = VOXEL_UNIT;

					m = vec3();

					n = vec3();

					m[workAxis1] = quadSize[workAxis1];

					n[workAxis2] = quadSize[workAxis2];

					offsetPos = startPos;

					offsetPos[direction] += backface ? 0 : VOXEL_UNIT;

					// Get the quad vertices
					p1 = model * vec4(offsetPos, 1.0f);

					p2 = model * vec4(offsetPos + m, 1.0f);

					p3 = model * vec4(offsetPos + m + n, 1.0f);

					p4 = model * vec4(offsetPos + n, 1.0f);

					int actualFace = getActualFaceIndex(direction, backface);

					Quad q(
						vec3(p1),
						vec3(p2), 
						vec3(p3), 
						vec3(p4), 
						voxel.type,
						actualFace,
						voxel.transparent
					);

					if (direction == 0) {
						q.rotateQuadOrder90();
					}

					meshBuilder->addQuad(q, quadSize[workAxis1], quadSize[workAxis2], backface);
				}
			}
		}
	}
}

void Chunk::cullingMeshing() {
	if (voxelsData == nullptr) {
		throw std::runtime_error("Voxels data is null. You may forgot to call MeshGenerator before this call");
	}

	int direction, workAxis1, workAxis2, face;
	vec3 startPos, quadSize, m, n, offsetPos;
	vec4 p1, p2, p3, p4;
	bool backface;

	// Iterate through each face and generate the mesh for each face of the quads
	for (face = 0; face < NUM_VOX_SIDES; ++face) {
		backface = face % 2 != 0;

		// direction could be front-back, left-right, top-bottom
		direction = face % 3;

		// workAxis1 and workAxis2 are the other two axis
		workAxis1 = (direction + 1) % 3;
		workAxis2 = (direction + 2) % 3;

		startPos = vec3();

		// Iterate through each voxel in the chunk
		// start from 0 to the end of each chunk in that direction
		for (startPos[direction] = 0; startPos[direction] < dimensions[direction]; ++startPos[direction]) {
			for (startPos[workAxis1] = 0; startPos[workAxis1] < dimensions[workAxis1]; ++startPos[workAxis1]) {
				for (startPos[workAxis2] = 0; startPos[workAxis2] < dimensions[workAxis2]; ++startPos[workAxis2]) {
					// Get the voxel at the current position
					Voxel voxel = voxelsData[MathUtil::flattenIndex(startPos, dimensions)];

					// If the voxel is not visible, skip it
					if (voxel.transparent || isFaceBlocking(startPos, direction, backface)) continue;

					// Get the size of the quad
					quadSize[workAxis1] = VOXEL_UNIT;

					quadSize[workAxis2] = VOXEL_UNIT;

					m = vec3();

					n = vec3();

					m[workAxis1] = quadSize[workAxis1];

					n[workAxis2] = quadSize[workAxis2];

					offsetPos = startPos;

					offsetPos[direction] += backface ? 0 : VOXEL_UNIT;

					// Get the quad vertices
					p1 = model * vec4(offsetPos, 1.0f);

					p2 = model * vec4(offsetPos + m, 1.0f);

					p3 = model * vec4(offsetPos + m + n, 1.0f);

					p4 = model * vec4(offsetPos + n, 1.0f);

					int actualFace = getActualFaceIndex(direction, backface);

					Quad q(
						vec3(p1),
						vec3(p2),
						vec3(p3),
						vec3(p4),
						voxel.type,
						actualFace,
						voxel.transparent
					);

					meshBuilder->addQuad(q, quadSize[workAxis1], quadSize[workAxis2], backface);
				}
			}
		}
	}
}

void Chunk::greedyMeshing() {
	if (voxelsData == nullptr) {
		throw std::runtime_error("Voxels data is null. You may forgot to call MeshGenerator before this call");
	}

	if (position.x == -3 && position.z == 5) {
		cout << "Greedy meshing for chunk at position: " << position.x << ", " << position.z << endl;
	}

	int direction, workAxis1, workAxis2, face, index;
	vec3 startPos, quadSize, m, n, offsetPos, currentPos;
	vec4 p1, p2, p3, p4;
	bool backface;

	// Iterate through each face and generate the mesh for each face of the quads
	for (face = 0; face < NUM_VOX_SIDES; ++face) {
		backface = face % 2 != 0;

		// direction could be front-back, left-right, top-bottom
		direction = face % 3;

		// workAxis1 and workAxis2 are the other two axis
		workAxis1 = (direction + 1) % 3;
		workAxis2 = (direction + 2) % 3;

		startPos = vec3();

		// Iterate through each voxel in the chunk
		// start from 0 to the end of each chunk in that direction
		for (startPos[direction] = 0; startPos[direction] < dimensions[direction]; ++startPos[direction]) {
			
			// Create a merge 2D array
			// Merge array will be used to keep track of the merged voxels
			// If a voxel is merged, then it will be marked as true
			bool** merged = (bool**)malloc(dimensions[workAxis1] * sizeof(bool*));

			assert(merged != nullptr);

			// Populate the merged array
			for (int i = 0; i < dimensions[workAxis1]; ++i) {
				merged[i] = (bool*)malloc(dimensions[workAxis2] * sizeof(bool));
				
				assert(merged[i] != nullptr);

				// Initialize the array with false elements
				memset(merged[i], false, (int)dimensions[workAxis2] * sizeof(bool));
			}
			
			for (startPos[workAxis1] = 0; startPos[workAxis1] < dimensions[workAxis1]; ++startPos[workAxis1]) {
				for (startPos[workAxis2] = 0; startPos[workAxis2] < dimensions[workAxis2]; ++startPos[workAxis2]) {
					quadSize = vec3();
					
					// Get the voxel at the current position
					index = MathUtil::flattenIndex(startPos, dimensions);

					assert(index >= 0 && index < dimensions.x * dimensions.y * dimensions.z);

					Voxel voxel = voxelsData[index];
					
					// If this voxel is already merged to some other voxel, skip it
					assert((int)startPos[workAxis1] >= 0 && (int)startPos[workAxis1] < dimensions[workAxis1] && (int)startPos[workAxis2] >= 0 && (int)startPos[workAxis2] < dimensions[workAxis2]);

					if (merged[(int)startPos[workAxis1]][(int)startPos[workAxis2]]) continue;

					// If the voxel is not visible or it is blocked by another voxel, skip it
					if (voxel.transparent || isFaceBlocking(startPos, direction, backface)) continue;

					// Figure out the size of the quad
					for (
						currentPos = startPos, currentPos[workAxis2]++; 
						currentPos[workAxis2] < dimensions[workAxis2] && shouldMergeTwoVoxel(startPos, currentPos, direction, backface) && !merged[(int)currentPos[workAxis1]][(int)currentPos[workAxis2]];
						currentPos[workAxis2]++
					) {}

					quadSize[workAxis2] = currentPos[workAxis2] - startPos[workAxis2];

					// Stretch as far as possible in workAxis1 direction
					for (
						currentPos = startPos, currentPos[workAxis1]++;
						currentPos[workAxis1] < dimensions[workAxis1] && shouldMergeTwoVoxel(startPos, currentPos, direction, backface) && !merged[(int)currentPos[workAxis1]][(int)currentPos[workAxis2]];
						currentPos[workAxis1]++
					) {
						// Stretch as far as possible in workAxis2 direction
						// Note that we've already figure out the size of the quad in workAxis2 direction
						for (
							currentPos[workAxis2] = startPos[workAxis2];
							currentPos[workAxis2] < dimensions[workAxis2] && shouldMergeTwoVoxel(startPos, currentPos, direction, backface) && !merged[(int)currentPos[workAxis1]][(int)currentPos[workAxis2]];
							currentPos[workAxis2]++
						) {}

						// Is the size of the quad same as the size of the quad we have figured out before?
						if (currentPos[workAxis2] - startPos[workAxis2] < quadSize[workAxis2]) {
							// If not, then we need to break the loop
							// Since we can't stretch the quad in workAxis2 direction anymore
							break;
						}
						else {
							// If the size of the quad is the same as the size of the quad we have figured out before
							// Then we can keep continue to the next "row" of voxels in workAxis2 direction
							currentPos[workAxis2] = startPos[workAxis2];
						}
					}

					quadSize[workAxis1] = currentPos[workAxis1] - startPos[workAxis1];

					m = vec3();

					n = vec3();

					m[workAxis1] = quadSize[workAxis1];

					n[workAxis2] = quadSize[workAxis2];

					offsetPos = startPos;

					offsetPos[direction] += backface ? 0 : VOXEL_UNIT;

					// Get the quad vertices
					p1 = model * vec4(offsetPos, 1.0f);

					p2 = model * vec4(offsetPos + m, 1.0f);

					p3 = model * vec4(offsetPos + m + n, 1.0f);

					p4 = model * vec4(offsetPos + n, 1.0f);

					int actualFace = getActualFaceIndex(direction, backface);

					Quad q(
						vec3(p1),
						vec3(p2),
						vec3(p3),
						vec3(p4),
						voxel.type,
						actualFace,
						voxel.transparent
					);

					if (direction == 0) {
						q.rotateQuadOrder90();
						meshBuilder->addQuad(q, quadSize[workAxis2], quadSize[workAxis1], backface);
					}
					else {
						meshBuilder->addQuad(q, quadSize[workAxis1], quadSize[workAxis2], backface);

					}

					// Update merged array
					for (int i = 0; i < quadSize[workAxis1]; ++i) {
						for (int j = 0; j < quadSize[workAxis2]; ++j) {
							merged[(int)startPos[workAxis1] + i][(int)startPos[workAxis2] + j] = true;
						}
					}
				}
			}

			// Free up memory
			for (int i = 0; i < dimensions[workAxis1]; ++i) {
				free(merged[i]);

				merged[i] = nullptr;
			}

			free(merged);

			merged = nullptr;
		}
	}
}