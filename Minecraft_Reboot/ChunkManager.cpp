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
	tuple<int, int> playerCoord = getChunkCoordsFromPlayerPos(player->playerCamera->transform.position);

	for (auto it = chunks.begin(); it != chunks.end();)
	{
		if (!isChunkInRenderDistance(it->first, playerCoord))
		{
			queueChunkForDelete(it->first);
		}

		++it;
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
				//updateChunk(make_tuple(x, z), deltaTime);
			}
		}
	}
}

void ChunkManager::render(float deltaTime)
{
	for (auto& chunk : chunks)
	{
		if (!chunk.second->pendingDeletion)
		{
			chunk.second->render(deltaTime);
		}
	}

	deletePendingChunks();
}

void ChunkManager::destroy()
{
	for (auto& chunk : chunks)
	{
		chunk.second->destroy();
	}

	chunks.clear();

	// Clear the chunks map
	while (!chunksToDelete.empty())
	{
		chunksToDelete.front()->destroy();

		chunksToDelete.pop();
	}
}

void ChunkManager::deletePendingChunks() {
	int deleteLimit = 5; // Limit the number of chunks deleted per frame

	while (!chunksToDelete.empty() && deleteLimit > 0)
	{
		auto chunk = chunksToDelete.front();

		cout << "Deleting chunk" << endl;

		chunk->destroy();

		chunks.erase(*chunk->chunkCoord);

		chunksToDelete.pop();

		deleteLimit--;
	}
}

tuple<int, int> ChunkManager::getChunkCoordsFromPlayerPos(const vec3& pos)
{
	return make_tuple((int)floor(pos.x / CHUNK_SIZE), (int)floor(pos.z / CHUNK_SIZE));
}

bool ChunkManager::isChunkInRenderDistance(const tuple<int, int>& chunkCoord, const tuple<int, int>& playerCoord)
{
	return abs(get<0>(chunkCoord) - get<0>(playerCoord)) <= CHUNK_RENDER_DISTANCE && abs(get<1>(chunkCoord) - get<1>(playerCoord)) <= CHUNK_RENDER_DISTANCE;
}

void ChunkManager::queueChunkForDelete(const tuple<int, int>& chunkCoord)
{
	// Mark this chunk is pending for deletion
	if (chunks.find(chunkCoord) == chunks.end())
	{
		// already deleted
		return;
	}

	cout << "Queueing chunk for deletion: " << get<0>(chunkCoord) << ", " << get<1>(chunkCoord) << endl;

	chunks[chunkCoord]->pendingDeletion = true;

	chunksToDelete.push(chunks[chunkCoord]);
}

void ChunkManager::loadChunk(const tuple<int, int>& chunkCoord)
{
	shared_ptr<Chunk> chunk = make_shared<Chunk>(vec3(get<0>(chunkCoord) * CHUNK_SIZE, 0.0f, get<1>(chunkCoord) * CHUNK_SIZE), vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE));

	chunk->chunkCoord = new tuple<int, int>(chunkCoord);

	chunk->init();

	chunks[chunkCoord] = chunk;
}

void ChunkManager::unloadChunk(const tuple<int, int>& chunkCoord)
{
	//
}

void ChunkManager::updateChunk(const tuple<int, int>& chunkCoord, float deltaTime)
{
	chunks[chunkCoord]->update(deltaTime);
}