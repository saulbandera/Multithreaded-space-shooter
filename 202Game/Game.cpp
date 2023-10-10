#include "Game.h"
typedef std::chrono::steady_clock the_clock;
DynArray<Enemy, 500> enemies;

Game::Game(RenderWindow *window){
	this->window = window;
	this->window->setFramerateLimit(300);
	this->fullscreen = false;
	this->dtMultiplier = 82.5f;
	this->scoreMultiplier = 1;
	this->score = 0;
	this->scoreTimer.restart();
	this->scoreTime = 0;
	this->killStreakAdderMax = 10;
	this->killStreakAdder = 0;
	this->killStreakTimerMax = 200.f;
	this->killStreakTimer = this->killStreakTimerMax;
	this->bestScoreSecond = 0.0;
	this->difficulty = 0;
	this->difficultyTimer = 0.f;
	this->fps = 0.0f;
	this->enemyStep = 0;
	this->bg = nullptr;
	this->useThreads = false;
	this->godMode = false;
	this->paused = true;
	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;
	this->initialize();
}

Game::~Game(){
	delete this->bg;
}

void Game::toggleFullscreen(){
	if (Keyboard::isKeyPressed(Keyboard::F11) && this->keyTime >= this->keyTimeMax){
		this->keyTime = 0.f;
		if (fullscreen){
			this->fullscreen = false;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "202", Style::Default);
		}
		else{
			this->fullscreen = true;
			this->window->close();
			this->window->create(sf::VideoMode(1920, 1080), "202", Style::Fullscreen);
		}
	}
}

void Game::changeEnemySpawnTime(){
	if (Keyboard::isKeyPressed(Keyboard::Num1)){
		this->enemySpawnTimerMax = 50.f; // low amount of enemies
	}

	if (Keyboard::isKeyPressed(Keyboard::Num2)){
		this->enemySpawnTimerMax = 40.f; // higher amount of enemies
	}

	if (Keyboard::isKeyPressed(Keyboard::Num3)) {
		this->enemySpawnTimerMax = 30.f; // higher amount of enemies
	}


	if (Keyboard::isKeyPressed(Keyboard::Num4)){
		this->enemySpawnTimerMax = 20.f; // way too many enemies
	}
	if (Keyboard::isKeyPressed(Keyboard::Num5)) {
		this->enemySpawnTimerMax = 10.f; // higher amount of enemies
	}


	if (Keyboard::isKeyPressed(Keyboard::Num6)) {
		this->enemySpawnTimerMax = 1.f; // way too many enemies
	}
	if (Keyboard::isKeyPressed(Keyboard::Num7)) {
		this->enemySpawnTimerMax = 0.1f; // higher amount of enemies
	}


	if (Keyboard::isKeyPressed(Keyboard::Num8)) {
		this->enemySpawnTimerMax = 0.01f; // way too many enemies
	}


	if (Keyboard::isKeyPressed(Keyboard::Num9)) {
		this->enemySpawnTimerMax = 0.001f; // way too many enemies
	}

	if (Keyboard::isKeyPressed(Keyboard::Num0)) {
		this->enemySpawnTimerMax = 0.0000000000000000001f; // way too many enemies
	}
}

void Game::toggleThreading(){
	if (Keyboard::isKeyPressed(Keyboard::F5)) {
		this->useThreads = true; 
	}
	if (Keyboard::isKeyPressed(Keyboard::F6)) {
		this->useThreads = false;
	}
}

void Game::toggleGodMode(){
	this->godMode = true;

}

void Game::pauseGame(){
	



	if (Keyboard::isKeyPressed(Keyboard::P) && this->keyTime >= this->keyTimeMax){
		if (this->paused) {
			this->paused = false;
			scoreTimer.restart();

		}
	
		else
			this->paused = true;
		this->keyTime = 0.f;
	}
}

void Game::initView(){
	this->mainView.setSize(Vector2f(this->window->getSize()));
	this->mainView.setCenter(Vector2f(this->window->getSize().x / 2, this->window->getSize().y / 2));
}

void Game::initTextures(){
	Projectiles::initTextures();
	Player::initTextures();
	Enemy::initTextures();
}

void Game::initUI(){
	Text tempText;

	// Follow text init
	this->playerTxt.setFont(font);
	this->playerTxt.setCharacterSize(25);
	this->playerTxt.setColor(Color::White);

	// Static text init
	this->staticPlayerTxt.setFont(font);
	this->staticPlayerTxt.setCharacterSize(25);
	this->staticPlayerTxt.setColor(Color::White);
	
	this->useThreadTxt.setFont(font);
	this->useThreadTxt.setCharacterSize(25);
	this->useThreadTxt.setColor(Color::Red);
	this->useThreadTxt.setPosition(10, 300);
	// XP bar
	this->playerXPBar.setSize(Vector2f(100.f, 10.f));
	this->playerXPBar.setFillColor(Color(0, 230, 255, 200));

	// Enemy text init 
	this->enemyTxt.setFont(this->font);
	this->enemyTxt.setCharacterSize(25);
	this->enemyTxt.setFillColor(Color::White);

	this->gameOverTxt.setFont(this->font);
	this->gameOverTxt.setFillColor(Color::Red);
	this->gameOverTxt.setCharacterSize(40);
	this->gameOverTxt.setString("GAME OVER!");
	this->gameOverTxt.setPosition(this->window->getSize().x/2 - 100.f, this->window->getSize().y / 2);

	this->scoreTxt.setFont(this->font);
	this->scoreTxt.setFillColor(Color(200,200,200,150));
	this->scoreTxt.setCharacterSize(32);
	this->scoreTxt.setString("Score: 0");
	this->scoreTxt.setPosition(10.f, 10.f);

	this->controlsTxt.setFont(this->font);
	this->gameOverTxt.setFillColor(Color::White);
	this->controlsTxt.setCharacterSize(26);
	this->controlsTxt.setString(
		"ARROW KEYS TO MOVE\nSPACE: SHOOT\nP: PAUSE/CONTROLS (START GAME)\nESC: QUIT\nF2, F3, F4: CHANGE ENEMY SPAWN RATE\nF5 TOGGLE ON THREADING\nF6 TOGGLE OFF THREADING\nF7 TOGGLE ON GOD MODE\n F8 TOGGLE OFF GOD MODE\nF11: FULLSCREEN\nTOP RIGHT SHIP: health\nBOTTOM SHIP: XP\n"
	);
	this->controlsTxt.setPosition(50.f, 400.f);

}

void Game::initialize(){
	//Init fonts
	this->font.loadFromFile("Fonts/Sansation-Bold.ttf");

	fpsCounter.setFont(font);
	fpsCounter.setFillColor(Color::White);
	fpsCounter.setPosition(1780,20);
	fpsCounter.setCharacterSize(34);
	fpsCounter.setOutlineThickness(5);
	// Init textures
	this->initTextures();

	// Init view
	this->initView();

	// Init map
	this->bg = new Background(10, 10);

	// Init players
	this->players.add(Player());

	this->playersAlive = this->players.size();

	// Init enemies
	this->enemySpawnTimerMax = 0.000001f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->initUI();
}


void Game::setEndingScoreboard(){ // Game over
	this->scoreTime = (int)this->scoreTimer.getElapsedTime().asSeconds();
	if (this->scoreTime == 0)
		this->scoreTime = 1;

	this->gameOverTxt.setString(
		std::string("GAME OVER\nScore: " +
			std::to_string(this->score) +
			"\n" +
			"Time: " +
			std::to_string(this->scoreTime) +
			"\n" +
			"Score/Second: " +
			std::to_string((int)round((double)this->score / (double)this->scoreTime))) +
		"\nF1 to RESTART"
	);

	if ((double)this->score / (double)this->scoreTime > this->bestScoreSecond)
		this->bestScoreSecond = (double)this->score / (double)this->scoreTime;
}	


///////   UPDATE FUNCTIONS  ///////  

//// RESTART ////  
void Game::restartUpdate(){
	if (Keyboard::isKeyPressed(Keyboard::F1)){
		for (size_t i = 0; i < this->players.size(); i++){
			this->players[i].reset();
		}

		this->playersAlive = this->players.size();
		this->score = 0;
		this->scoreMultiplier = 1;
		this->killStreakAdder = 0;
		this->scoreTime = 0;
		this->difficulty = 0;
		this->enemySpawnTimerMax = 25.f; 
		this->scoreTimer.restart();
		
		// Reset stage
		this->mainView.setCenter(Vector2f(
			this->window->getSize().x / 2,
			this->window->getSize().y / 2));
		this->bg->reset(this->mainView);

		// Clear arrays
		enemies.clear();
		this->textTags.clear();
	}
}

//// TIMERS ////  
void Game::updateTimers(const float &dt){
	if (this->keyTime < this->keyTimeMax)
		this->keyTime += 1.f * dt * this->dtMultiplier;
}

//// TIMERS UNPAUSED ////  
void Game::updateTimersUnpaused(const float &dt){
	
	// Enemy spawn 
	if (this->enemySpawnTimer < this->enemySpawnTimerMax)
		this->enemySpawnTimer += 1.f * dt * this->dtMultiplier;

	// Difficulty 
	this->difficultyTimer += 1.f * dt * this->dtMultiplier;

	// Score and multipliers
	if (this->killStreakTimer > 0.f){
		this->killStreakTimer -= 1.f * dt * this->dtMultiplier;

		if (this->killStreakTimer <= 0.f){
			this->killStreakTimer = 0.f;
			this->killStreakAdder = 0;
			this->scoreMultiplier = 1;
		}
	}
}

//// SCORE ////  
void Game::updateScore(){
	if (this->killStreakAdder >= this->killStreakAdderMax){
		this->killStreakAdder = 0;
		this->scoreMultiplier++;
	}
}

//// DIFFICULTY ////  
void Game::updateDifficulty(){
	if ((int)this->difficultyTimer % 1000 == 0){

		this->difficulty++;
		this->difficultyTimer = 1.f;
	}
}

//// PLAYER UI ////  
void Game::updateUIPlayer(int index){
	
	this->playerTxt.setPosition(this->players[index].getPosition().x - 25.f, this->players[index].getPosition().y - 68.f);
    this->playerTxt.setString(this->players[index].getHpAsString()+ "\n\n\n\n\n\n\n\n\n\n"+ std::to_string(this->players[index].getLevel()));

	// XP bar
	this->playerXPBar.setPosition(this->players[index].getPosition().x + 10.f,this->players[index].getPosition().y + 217.f);
	this->playerXPBar.setScale(((static_cast<float>(this->players[index].getXp()) / this->players[index].getXpNext())),2.f);
	
}

//// PLAYER ////  
void Game::updatePlayer(const float &dt){
	for (int i = 0; i < this->players.size(); i++){
		if (this->players[i].isAlive()){

			// Update players
			this->players[i].update(this->mainView, dt);

			// Bullets update
			this->updatePlayerProjectiles(dt, i);

			// Update score
			this->score = 0;
			
			for (size_t k = 0; k < this->players.size(); k++){
				this->score += players[k].getScore();
			}

			this->scoreTxt.setString(
				"Score: " +
				std::to_string(this->score) +
				"\nScore Multiplier:" +
				std::to_string(this->scoreMultiplier) + "x" +
				"\nKill Streak Timer:" +
				std::to_string((int)this->killStreakTimer) +
				"\nNew Multiplier: " +
				std::to_string(this->killStreakAdder) +
				" / " +
				std::to_string(this->killStreakAdderMax) +
				"\nGame time: " +
				std::to_string((int)this->scoreTimer.getElapsedTime().asSeconds()) +
				"\nDifficulty: " +
				std::to_string(this->difficulty) +
				"\nBest Score/Second: " +
				std::to_string((int)round(this->bestScoreSecond))
			);
		}
	}
}

//// PLAYER PROJECTILES ////  
void Game::updatePlayerProjectiles(const float &dt, const int i){
	bool enemyRemoved = false;
	bool prjRemoved = false;

	for (size_t k = 0; k < this->players[i].getProjectileSize() && !prjRemoved; k++){
		this->players[i].getProjectile(k).update(dt);

		// Enemy collision check
		for (size_t j = 0; j < enemies.size() && !enemyRemoved; j++){
			if (this->players[i].getProjectile(k).getGlobalBounds().intersects(enemies[j].getGlobalBounds())){
				
				// Enemy take damage
				int damage = this->players[i].getProjectile(k).getDamage();
				if (enemies[j].getHP() > 0){
					enemies[j].takeDamage(damage);
				}

				// Enemy dead
				if (enemies[j].getHP() <= 0){
					// Add to Score and reset 
					this->killStreakTimer = this->killStreakTimerMax;
					int score = enemies[j].getHPMax() * this->scoreMultiplier;
					this->killStreakAdder++;
					this->players[i].gainScore(score);

					// gain xp
					int xp = enemies[j].getHPMax() + (rand() % enemies[j].getHPMax() + 1) * this->scoreMultiplier;

					// Score
					this->textTags.add(
						TextTag(&this->font,
							"+ " + std::to_string(score) +
							"	( x" + std::to_string(this->scoreMultiplier) + " )",
							Color::White,
							Vector2f(100.f, 10.f),
							Vector2f(1.f, 0.f),
							30,
							40.f,
							true
						)
					);

					// Level up
					if (this->players[i].gainXp(xp)){
						this->textTags.add(
							TextTag(&this->font,
								"LEVEL UP!",
								Color::Cyan,
								Vector2f(this->players[i].getPosition().x + 20.f,
									this->players[i].getPosition().y - 20.f),
								Vector2f(0.f, 1.f),
								32,
								40.f,
								true
							)
						);
					}
					enemyRemoved = true;
				}

			}
			if (enemyRemoved)
				enemies.remove(j);
		}

		// Window bounds check
		if (this->players[i].getProjectile(k).getPosition().x > this->mainView.getCenter().x + this->mainView.getSize().x / 2)	{
			prjRemoved = true;
		}

		if (prjRemoved)
			this->players[i].removeBullet(k);
	}
}

//// ENEMY ////  
void Game::updateEnemies(const float &dt){
	// Spawn enemies
	this->updateEnemySpawn(dt);

	// Update enemies
	bool enemyRemoved = false;
	bool playerKilled = false;

#pragma region mltThreading
	if (useThreads) {
		enemyStep = enemies.size() / 4;
		enemyStep = std::max(enemyStep, 1);

		thread_data data; // Specify data to be used by thread function
		data.dt = dt;
		data.playerPosition = players[0].getPosition();

		for (int i = 0; i < enemies.size(); i += enemyStep) { // Get groups of enemies and update them on each thread
			if (enemyStep > 1 && (i + enemyStep) >= (enemies.maxSize() - 1))
				enemyStep = (enemies.maxSize() - 1) - i;
			data.start = i;
			data.count = enemyStep;
			threads.pushData(data);
		}
		while (!threads.isFinished()) {}
	}
#pragma endregion

	for (size_t i = 0; i < enemies.size() && !enemyRemoved; i++){
		
		if (!useThreads) {
			// update normally without multithreading 
			//std::cout << "no threads\n";
			enemies[i].update(dt, this->players[enemies[i].getPlayerFollowNum()].getPosition());
		}

		for (size_t k = 0; k < this->players.size() && !playerKilled; k++){
			if (this->players[k].isAlive()){
				if (this->players[k].getBounds().intersects(enemies[i].getGlobalBounds()) && !this->players[k].isDamageCooldown()){
					int damage = enemies[i].getDamage();

					// Player takes damage
					if (!this->godMode) {
						this->players[k].takeDamage(damage);
					}
		
					enemies[i].collision();

					// Player death
					if (!this->players[k].isAlive())
						playerKilled = true;
				}
			}
			if (playerKilled)
				this->playersAlive--;
		}
		//Remove enemies out of bounds
		if (enemies[i].getPosition().x < this->mainView.getCenter().x - this->mainView.getSize().x/2 - enemies[i].getGlobalBounds().width){
	       enemyRemoved = true;
		}

		if (enemyRemoved)
			enemies.remove(i);
	}

}

//// ENEMY SPAWN ////  
void Game::updateEnemySpawn(const float &dt){
	if (this->enemySpawnTimer >= this->enemySpawnTimerMax){

		//std::cout << "SPAWN: " << this->scoreTimer.getElapsedTime().asSeconds() << endl;
			enemies.add(Enemy( // Spawn an enemy
				this->mainView,
				true,
				Vector2f(0.f, 1.f),
				Vector2f(0.f, 1.f),
				-400,
				rand() % Enemy::numTypes,
				this->players[(rand() % playersAlive)].getLevel(),
				rand() % this->playersAlive)
			);

			//enemies.add(Enemy( // Spawn an enemy
			//	this->mainView,
			//	true,
			//	Vector2f(0.f, 1.f),
			//	Vector2f(0.f, 1.f),
			//	-400,
			//	rand() % Enemy::numTypes,
			//	this->players[(rand() % playersAlive)].getLevel(),
			//	rand() % this->playersAlive)
			//);
			//enemies.add(Enemy( // Spawn an enemy
			//	this->mainView,
			//	true,
			//	Vector2f(0.f, 1.f),
			//	Vector2f(0.f, 1.f),
			//	-400,
			//	rand() % Enemy::numTypes,
			//	this->players[(rand() % playersAlive)].getLevel(),
			//	rand() % this->playersAlive)
			//);
			this->enemySpawnTimer = 0; // Reset timer
	}
}

//// TEXT TAGS ////  
void Game::updateTextTags(const float &dt){
	bool textTagRemoved = false;
	for (size_t i = 0; i < this->textTags.size() && !textTagRemoved; i++){
		this->textTags[i].update(dt);
		if (this->textTags[i].getTimer() <= 0.f){
			textTagRemoved = true;
		}
		if (textTagRemoved)
			this->textTags.remove(i);
	}

	if (useThreads) {
		useThreadTxt.setString("MULTITHREADING: ON");
		useThreadTxt.setFillColor(Color::Green);
	}
	else {
		useThreadTxt.setString("MULTITHREADING: OFF");
		useThreadTxt.setFillColor(Color::Red);
	}




	
}

//// UPDATE ////  
void Game::UPDATE(const float &dt){	
	int Framerate = 1.f / clock.getElapsedTime().asSeconds();
	clock.restart();

// 	std::cout << this->scoreTimer.getElapsedTime().asSeconds() << endl;

	/*if (this->scoreTimer.getElapsedTime().asSeconds() >= 10) {
		std::cout << std::to_string(Framerate) << " , " << enemySpawnTimerMax << endl; 
		window->close();
	}*/
	// Fps

	fpsCounter.setString(std::to_string(Framerate));
	
	this->changeEnemySpawnTime();

	// Timers
	this->updateTimers(dt);

	// Fullscreen
	this->toggleFullscreen();

	//check for multithreading
	this->toggleThreading();

	// god mode
	this->toggleGodMode();
	// Pause 
	this->pauseGame();

	// Game start
	if (this->playersAlive > 0 && !this->paused){

	

		
		// Timers
		this->updateTimersUnpaused(dt);
		// Progression
		this->updateDifficulty();
		// Score timer and multipliers
		this->updateScore();
		// Background 
		this->bg->update(dt, this->mainView);
		// players
		this->updatePlayer(dt);
		// enemies
		this->updateEnemies(dt);
		// Text
		this->updateTextTags(dt);
		//Restart
		this->restartUpdate();
	}
	else if(this->playersAlive <= 0 && this->scoreTime == 0){
		this->setEndingScoreboard(); 
	}
	
	// Restart if no players alive
	if (this->playersAlive <= 0){
		this->restartUpdate();
	}
}





///////   RENDER FUNCTIONS   ///////   

//// UI //// 
void Game::renderUI(){
	// GAME OVER
	if (this->playersAlive <= 0){
		this->window->draw(this->gameOverTxt);
	}

	// Score 
	this->window->draw(this->scoreTxt);

	// Controls 
	if (this->paused)
		this->window->draw(this->controlsTxt);
}

//// PLAYERS //// 
void Game::renderPlayers(){
	for (size_t i = 0; i < this->players.size(); i++){
		if (this->players[i].isAlive()){
			this->players[i].render(*this->window);

			// UI
			this->updateUIPlayer(i);
			this->window->draw(this->playerTxt);
			this->window->draw(this->playerXPBar);
		}
	}
}

//// ENEMIES //// 
void Game::renderEnemies(){

	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i].render(*this->window);
		this->window->draw(this->enemyTxt);
	}
}

//// TEXT TAGS //// 
void Game::renderTextTags(){
	//Draw texttags
	for (size_t i = 0; i < this->textTags.size(); i++){
		this->textTags[i].render(*this->window);
	}

	window->draw(useThreadTxt);
}

//// RENDER ////  
void Game::RENDER(){
	this->window->clear();
	this->window->setView(this->mainView);

	// Background
	bg->render(*this->window, this->mainView, this->font);
	
	// Players
	this->renderPlayers();
	// Enemies
	this->renderEnemies();
	// Text
	this->renderTextTags();
	// Set view
	this->window->setView(this->window->getDefaultView());
	// UI
	this->renderUI();
	this->window->draw(fpsCounter);
	//FINISH DRAW
	this->window->display();
}
#pragma endregion RENDER