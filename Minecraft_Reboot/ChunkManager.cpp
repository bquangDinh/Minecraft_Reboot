#include "ChunkManager.h"

ChunkManager::ChunkManager(shared_ptr<Player> player):
	player(player),
	CHUNK_SIZE(32, 32, 32),
	CHUNK_RENDER_DISTANCE(4)
{
}

void ChunkManager::init()
{
	// Create a thread pool
	unsigned int threadCount = std::thread::hardware_concurrency();

	printf("Creating %d threads for chunk creation\n", threadCount);

	for (unsigned int i = 0; i < threadCount; i++)
	{
		chunkCreationThreads.emplace_back(make_unique<std::thread>(&ChunkManager::createPendingChunks, this));
	}
}

void ChunkManager::update(float deltaTime)
{
	// Lock mutex
	std::lock_guard<std::mutex> lock(chunksMutex);

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
				//loadChunk(make_tuple(x, z));

				//cout << "Loaded chunk: " << x << ", " << z << endl;

				queueChunkForCreation(make_tuple(x, z));
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
	printf("Destroying chunk manager...\n");

	// Clear the chunks map
	while (!chunksToDelete.empty())
	{
		//chunksToDelete.front()->destroy();

		chunksToDelete.pop();
	}

	printf("Emptied chunks to delete queue\n");

	for (auto& chunk : chunks)
	{
		chunk.second->destroy();
	}

	chunks.clear();

	printf("Destroyed all chunks\n");

	// Stop all workers
	done = true;

	queueCondition.notify_all();

	for (auto& thread : chunkCreationThreads)
	{
		if (thread->joinable())
		{
			thread->join();
		}
	}

	printf("Stopped all chunk creation threads\n");

	chunkCreationThreads.clear();

	printf("Cleared chunk creation threads\n");

	// Clear the chunks to create queue
	while (!chunksToCreate.empty())
	{
		chunksToCreate.pop();
	}

	printf("Emptied chunks to create queue\n");
}

void ChunkManager::createPendingChunks() {
	while (gameStates->isRunning)
	{
		tuple<int, int> chunkCoord;

		{
			std::unique_lock<std::mutex> lock(queueMutex);

			// Wait until there are chunks to create
			queueCondition.wait(lock, [this] { return !chunksToCreate.empty() || done; });

			if (!chunksToCreate.empty()) {
				chunkCoord = chunksToCreate.front();

				chunksToCreate.pop();
			}
			else {
				continue;
			}
		}

		loadChunk(chunkCoord);
	}
}

void ChunkManager::deletePendingChunks() {
	int deleteLimit = 10; // Limit the number of chunks deleted per frame

	while (!chunksToDelete.empty() && deleteLimit > 0)
	{
		auto chunk = chunksToDelete.front();

		chunk->destroy();

		chunks.erase(*chunk->chunkCoord);

		chunksToDelete.pop();

		deleteLimit--;
	}

	//cout << "Pending chunks left: " << chunksToDelete.size() << endl;
}

tuple<int, int> ChunkManager::getChunkCoordsFromPlayerPos(const vec3& pos)
{
	return make_tuple((int)floor(pos.x / CHUNK_SIZE.x), (int)floor(pos.z / CHUNK_SIZE.z));
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

void ChunkManager::queueChunkForCreation(const tuple<int, int>& chunkCoord) {
	{
		std::lock_guard<std::mutex> lock(queueMutex);

		chunksToCreate.push(chunkCoord);
	}

	queueCondition.notify_all();
}

void ChunkManager::loadChunk(const tuple<int, int>& chunkCoord)
{
	// Lock mutex
	std::lock_guard<std::mutex> lock(chunksMutex);

	shared_ptr<Chunk> chunk = make_shared<Chunk>(vec3(get<0>(chunkCoord) * CHUNK_SIZE.x, 0.0f, get<1>(chunkCoord) * CHUNK_SIZE.z), CHUNK_SIZE);

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