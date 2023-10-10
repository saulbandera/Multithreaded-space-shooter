#pragma once
#include "DynArray.h"
#include <condition_variable>
#include "Includes.h"

class Enemy;
extern DynArray<Enemy, 500> enemies; 

struct thread_data { // data needed to update enemies
	uint32_t start;
	uint32_t count;
	sf::Vector2f playerPosition;
	float dt;
};


class threadQueue {
public:
	threadQueue(size_t numThreads);
	~threadQueue();
	void pushData(const thread_data& data);
	void threadQueue::updateEnemyRange(thread_data data);
	bool isFinished();

private:
	void pushThread();
	std::queue<thread_data> threadData;
	std::vector<std::thread> threadVec;
	std::vector<std::thread::id> threadIDs;

	std::atomic<int> tasksLeft = 0;
	std::mutex threadsMutex;
	std::mutex queueMutex;

	std::condition_variable condition;
	bool stopped = false;
};

inline threadQueue::threadQueue(size_t n_threads) {
	for (size_t i = 0; i < n_threads; i++)
		pushThread();
}

inline threadQueue::~threadQueue() {
	std::unique_lock<std::mutex> lock(queueMutex);
	std::cout << "task?\n";
	stopped = true;
	// turn on all threads
	condition.notify_all();
	for (std::thread& thread1 : threadVec)
		thread1.join();
}

inline void threadQueue::pushData(const thread_data& data) {
	std::unique_lock<std::mutex> lock(queueMutex);
	threadData.push(data);
	tasksLeft++;
	condition.notify_one();
}

inline bool threadQueue::isFinished() {
	if (tasksLeft == 0) {
		return true;
	}
	else {
		return false;
	}

}


inline void threadQueue::pushThread() {
	std::cout << threadVec.size() <<"\n";
	threadVec.emplace_back( // add thread
		[this]() {
		while(true) { // infinite loop 
			std::cout << "waiting?\n";
			thread_data data;
			std::unique_lock<std::mutex> lock(queueMutex); // lock threadData queue so that it can't be accessed until task is completed o it is empty
			
			this->condition.wait(lock, [this]() { return this->stopped || !this->isFinished(); }); // wait until task is completed
			if (this->threadData.empty()) {
				continue;
			}
			data = this->threadData.front(); 
			this->threadData.pop(); // remove pushed data
		     

			// update enemies
			updateEnemyRange(data); // update enemies
			std::cout << " TASKS TASKS TASKS : " << tasksLeft << endl;
			--tasksLeft; 
		}
	}
	);
}
