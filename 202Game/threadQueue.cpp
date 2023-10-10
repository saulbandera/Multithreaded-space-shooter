#include "threadQueue.h"
#include "Enemy.h"

void threadQueue::updateEnemyRange(thread_data data) {
	uint32_t max = data.start + data.count; // update a range of enemies
	for (uint32_t i = data.start; i < max; ++i) {
		enemies[i].update(data.dt, data.playerPosition); 
	}
}
