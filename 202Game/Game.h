#pragma once
#include "Includes.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include <array>
#include "threadQueue.h"
#include <chrono>


extern DynArray<Enemy, 500> enemies;



using std::mutex;
using std::thread;
using std::vector;
using std::queue;
class Game{
private:

	// MULTITHREADING
	int enemyStep;
	threadQueue threads{std::thread::hardware_concurrency()}; // thread pool with num of threads on computer
	bool useThreads;

	//Game
	RenderWindow *window;
	View mainView;

	float dtMultiplier;
	bool paused;
	float keyTimeMax, keyTime;
	bool fullscreen;
	int mode;
	

	Clock clock;
	Text fpsCounter; 

	//Score
	Clock scoreTimer;
	unsigned score, scoreMultiplier;
	int scoreTime;
	int killStreakAdder, killStreakAdderMax;
	float killStreakTimerMax, killStreakTimer;
	float difficultyTimer;
	int difficulty;
	double bestScoreSecond;

	//Text 
	Font font;
	Text playerTxt;
	Text staticPlayerTxt;
	Text enemyTxt;
	Text gameOverTxt;
	Text scoreTxt;
	Text controlsTxt;
	Text useThreadTxt;
	RectangleShape playerXPBar;

	//Texttags
	dArr<TextTag> textTags;

	Background *bg;

	//Players
	dArr<Player> players;
	int playersAlive;
	bool godMode;
	float enemySpawnTimer, enemySpawnTimerMax;

	//Enemy textures
	dArr<Texture> enemyTextures;



public:

	float fps;
	Game(RenderWindow *window);
	virtual ~Game();

	//Getters and Setters
	inline RenderWindow& getWindow() { return *this->window; }
	inline Player getPlayer(int indx) { return players[indx]; };

	//Functions
	void toggleFullscreen();
	void changeEnemySpawnTime();


	void toggleThreading();
	void toggleGodMode();


	void pauseGame();

	void initView();
	void initTextures();
	void initUI();
	void initialize();

	void restartUpdate();
	void setEndingScoreboard();

	void updateTimers(const float &dt);
	void updateTimersUnpaused(const float &dt);
	void updateScore();
	void updateDifficulty();
	void updateUIPlayer(int index);
	void updatePlayer(const float &dt);
	void updatePlayerProjectiles(const float &dt, const int i);
	void updateEnemies(const float &dt);
	void updateEnemySpawn(const float &dt);
	void updateTextTags(const float &dt);
	void UPDATE(const float &dt);

	void renderUI();
	void renderPlayers();
	void renderEnemies();
	void renderTextTags();
	void RENDER();

};

