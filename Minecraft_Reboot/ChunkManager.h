#pragma once

#include <memory>
#include <unordered_map>
#include <queue>
#include <cmath> 
#include <mutex>
#include <thread>

#include "GameObject.h"
#include "Chunk.h"
#include "Player.h"

using namespace std;

struct ChunkHash {
	size_t operator()(const tuple<int, int>& chunkCoord) const {
		return hash<int>()(get<0>(chunkCoord)) ^ hash<int>()(get<1>(chunkCoord));
	};
};

class ChunkManager : public GameObject
{
private:
	const vec3 CHUNK_SIZE;

	const int CHUNK_RENDER_DISTANCE;

	shared_ptr<Player> player;

	unordered_map<tuple<int, int>, shared_ptr<Chunk>, ChunkHash> chunks;

	std::queue<shared_ptr<Chunk>> chunksToDelete; // Queue for chunks to be deleted

	std::queue<tuple<int, int>> chunksToCreate;

	std::mutex chunksMutex, queueMutex;

	std::condition_variable queueCondition;

	std::atomic<bool> hasChunksInCreationQueue, done;

	std::vector<unique_ptr<std::thread>> chunkCreationThreads;

	tuple<int, int> getChunkCoordsFromPlayerPos(const vec3& pos);

	bool isChunkInRenderDistance(const tuple<int, int>& chunkCoord, const tuple<int, int>& playerCoord);

	void queueChunkForDelete(const tuple<int, int>& chunkCoord);

	void queueChunkForCreation(const tuple<int, int>& chunkCoord);

	void loadChunk(const tuple<int, int>& chunkCoord);

	void unloadChunk(const tuple<int, int>& chunkCoord);

	void updateChunk(const tuple<int, int>& chunkCoord, float deltaTime);

	void deletePendingChunks();
	
	void createPendingChunks();
public:
	ChunkManager(shared_ptr<Player>);

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

