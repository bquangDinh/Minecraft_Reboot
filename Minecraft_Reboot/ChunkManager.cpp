#include "ChunkManager.h"

ChunkManager::ChunkManager(shared_ptr<Player> player):
	player(player),
	CHUNK_SIZE(32),
	CHUNK_RENDER_DISTANCE(2),
	verticesBuffer(nullptr),
	indicesBuffer(nullptr),
	verticesBufferID(0),
	indicesBufferID(0)
{
	numberOfChunks = 25;

	cout << "Number of chunks in render distance: " << numberOfChunks << endl;

	// Calculate the number of vertices and indices

	// Each chunk has CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE voxels
	// Each voxel has 6 faces
	// Each face has 4 vertices (indices will be used)
	// Each vertex has 6 floats (3 for position, 2 for texture coordinates, 1 for texture index)
	vertexCount = numberOfChunks * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 4 * 6;

	cout << "Each chunk has " << CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 4 * 6 << " vertices" << endl;

	// Each voxels has 6 faces
	// Each face has 2 triangles
	// Each triangle has 3 vertices
	indicesCount = numberOfChunks * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6;

	cout << "Each chunk has " << CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6 << " indices" << endl;
}

void ChunkManager::init()
{
	// Will be called on main thread
	initShareBuffers();
}

void ChunkManager::initShareBuffers() {
	int numberOfChunks = numberOfChunksInRenderDistance();

	cout << "Number of chunks in render distance: " << numberOfChunks << endl;

	// Generate VAO
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	// Init vertices buffer
	glGenBuffers(1, &verticesBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferID);

	glBufferStorage(GL_ARRAY_BUFFER, vertexCount * sizeof(float), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

	verticesBuffer = (float*)glMapBufferRange(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

	// Assigned default values
	// -1.0f is for tex index
	// Will be used to ignore every vertex that has -1.0f as tex index
	memset(verticesBuffer, -1.0f, vertexCount * sizeof(float));

	// Init indices buffer
	glGenBuffers(1, &indicesBufferID);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferID);

	glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, indicesCount * sizeof(unsigned int), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

	indicesBuffer = (unsigned int*)glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indicesCount * sizeof(unsigned int), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

	// Assigned default values
	memset(indicesBuffer, 0, indicesCount * sizeof(unsigned int));

	// Set vertex attribute pointers
	// For position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	// For texture coordinates attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	// For texture index attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	// Unbind VAO
	glBindVertexArray(0);
}

void ChunkManager::update(float deltaTime)
{
	std::lock_guard<std::mutex> lock(chunksMutex);

	tuple<int, int> playerCoord = getChunkCoordsFromPlayerPos(player->playerCamera->transform.position);

	for (auto it = chunks.begin(); it != chunks.end();)
	{
		if (!isChunkInRenderDistance(it->first, playerCoord))
		{
			unloadChunk(it->first);

			cout << "Unloaded chunk: " << get<0>(it->first) << ", " << get<1>(it->first) << endl;

			// Erase the element and get the next ite
			it = chunks.erase(it);
		}
		else
		{
			it++;
		}
	}

	for (int x = get<0>(playerCoord) - CHUNK_RENDER_DISTANCE; x <= get<0>(playerCoord) + CHUNK_RENDER_DISTANCE; x++)
	{
		for (int z = get<1>(playerCoord) - CHUNK_RENDER_DISTANCE; z <= get<1>(playerCoord) + CHUNK_RENDER_DISTANCE; z++)
		{
			if (x + CHUNK_RENDER_DISTANCE < 0 || z + CHUNK_RENDER_DISTANCE < 0) continue;

			if (chunks.find(make_tuple(x, z)) == chunks.end())
			{
				cout << "About to load chunk: " << x << ", " << z << endl;

				loadChunk(make_tuple(x, z));

				cout << "Loaded chunk: " << x << ", " << z << endl;
			}
			else {
				updateChunk(make_tuple(x, z), deltaTime);
			}
		}
	}

	dataUpdated = true;
}

void ChunkManager::render(float deltaTime)
{
	/*std::lock_guard<std::mutex> lock(chunksMutex);

	for (auto& chunk : chunks)
	{
		chunk.second->render(deltaTime);
	}*/

	std::lock_guard<std::mutex> lock(chunksMutex);

	if (dataUpdated) {
		glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(float));

		glFlushMappedBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, indicesCount * sizeof(unsigned int));

		dataUpdated = false;
	}

	// Render the data inside VBO and EBO
	TextureManager::getInstance()->getTextureArray(MAIN_TEXTURE_ARRAY)->Use();

	shaderManager->getShaderProgram(MAIN_SHADER_PROGRAM)->Use();

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void ChunkManager::destroy()
{
	for (auto& chunk : chunks)
	{
		chunk.second->destroy();
	}

	chunks.clear();

	glDeleteVertexArrays(1, &VAO);

	glDeleteBuffers(1, &verticesBufferID);

	glDeleteBuffers(1, &indicesBufferID);

	//delete[] verticesBuffer;

	//delete[] indicesBuffer;

	verticesBuffer = nullptr;

	indicesBuffer = nullptr;
}

int ChunkManager::numberOfChunksInRenderDistance()
{
	// Calculate the circle area of the render distance R
	float area = M_PI * pow(CHUNK_RENDER_DISTANCE, 2);

	// TODO: change this to chunk Voxel unit
	float VOXEL_UNIT = 1.0f;

	// Calculate the area of a single chunk with voxel unit
	float chunkArea = pow(CHUNK_SIZE, 2) * pow(VOXEL_UNIT, 2);

	int numberOfChunks = ceil(area / chunkArea);

	return numberOfChunks;
}

tuple<int, int> ChunkManager::getChunkCoordsFromPlayerPos(const vec3& pos)
{
	return make_tuple((int)floor(pos.x / CHUNK_SIZE), (int)floor(pos.z / CHUNK_SIZE));
}

bool ChunkManager::isChunkInRenderDistance(const tuple<int, int>& chunkCoord, const tuple<int, int>& playerCoord)
{
	return abs(get<0>(chunkCoord) - get<0>(playerCoord)) <= CHUNK_RENDER_DISTANCE && abs(get<1>(chunkCoord) - get<1>(playerCoord)) <= CHUNK_RENDER_DISTANCE;
}

void ChunkManager::loadChunk(const tuple<int, int>& chunkCoord)
{
	unique_ptr<Chunk> chunk = make_unique<Chunk>(vec3(get<0>(chunkCoord) * CHUNK_SIZE, 0.0f, get<1>(chunkCoord) * CHUNK_SIZE), vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE));

	// Genrate array index from chunk's coord so that the top-left chunk (from top-down view) will be at 0
	int arrayIndex = (get<0>(chunkCoord) + CHUNK_RENDER_DISTANCE) * (CHUNK_RENDER_DISTANCE * 2 + 1) + (get<1>(chunkCoord) + CHUNK_RENDER_DISTANCE);

	int startVerticesIndex = arrayIndex * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 4 * 6;

	int startIndicesIndex = arrayIndex * CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * 6 * 6;

	cout << "Chunk at " << get<0>(chunkCoord) << ", " << get<1>(chunkCoord) << " has array index " << arrayIndex << endl;

	cout << "Start vertices index " << startVerticesIndex << " Start indices index " << startIndicesIndex << endl;

	chunk->init();

	chunk->useSharedBuffers(&verticesBuffer, startVerticesIndex, &indicesBuffer, startIndicesIndex);

	chunks[chunkCoord] = move(chunk);
}

void ChunkManager::unloadChunk(const tuple<int, int>& chunkCoord)
{
	chunks[chunkCoord]->destroy();
}

void ChunkManager::updateChunk(const tuple<int, int>& chunkCoord, float deltaTime)
{
	chunks[chunkCoord]->update(deltaTime);
}