#pragma once

#include <memory>
#include <unordered_map>
#include <cmath> 

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
	const int CHUNK_SIZE;

	const int CHUNK_RENDER_DISTANCE;

	shared_ptr<Player> player;

	unordered_map<tuple<int, int>, unique_ptr<Chunk>, ChunkHash> chunks;

	tuple<int, int> getChunkCoordsFromPlayerPos(const vec3& pos);

	bool isChunkInRenderDistance(const tuple<int, int>& chunkCoord, const tuple<int, int>& playerCoord);

	void loadChunk(const tuple<int, int>& chunkCoord);

	void unloadChunk(const tuple<int, int>& chunkCoord);
public:
	ChunkManager(shared_ptr<Player>);

	void init() override;

	void update(float) override;

	void render(float) override;

	void destroy() override;
};

