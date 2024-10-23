#include "ChunkManager.h"

ChunkManager::ChunkManager(shared_ptr<Player> player):
	player(player),
	CHUNK_SIZE(32),
	CHUNK_RENDER_DISTANCE(4)
{
}

void ChunkManager::init()
{

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
}

void ChunkManager::render(float deltaTime)
{
	std::lock_guard<std::mutex> lock(chunksMutex);

	for (auto& chunk : chunks)
	{
		chunk.second->render(deltaTime);
	}
}

void ChunkManager::destroy()
{
	for (auto& chunk : chunks)
	{
		chunk.second->destroy();
	}

	chunks.clear();
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

	chunk->init();

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