#include "Player.h"

unsigned Player::players = 0;
dArr<Texture> Player::bodyTextures;


void Player::initTextures(){
	// Loading player textures
	Texture temp;
	temp.loadFromFile("Lunar Lander Upload/ships/spaceshipSpritesS.png");
	Player::bodyTextures.add(temp);
}

Player::Player(int UP,int DOWN,int LEFT,int RIGHT,int SHOOT,int STATS)
	:level(1), xp(0),
	hp(10), hpMax(10),hpAdded(10),
	statPoints(0), levelMultiplier(0),
	damage(1), damageMax(2),
	score(0){
	//// ANIMATION
	jet.addFrame(sf::IntRect(0, 0, 32, 50));
	jet.addFrame(sf::IntRect(32, 0, 32, 50));
	jet.addFrame(sf::IntRect(64, 0, 32, 50));
	jet.addFrame(sf::IntRect(96, 0, 32, 50));
	jet.addFrame(sf::IntRect(128, 0, 32, 50));
	jet.addFrame(sf::IntRect(160, 0, 32, 50));
	jet.addFrame(sf::IntRect(192, 0, 32, 50));
	jet.addFrame(sf::IntRect(224, 0, 32, 50));
	jet.addFrame(sf::IntRect(256, 0, 32, 50));
	jet.setFrameSpeed(0.10f);

	shoot.addFrame(sf::IntRect(0, 50, 32, 50));
	shoot.addFrame(sf::IntRect(32, 50, 32, 50));
	shoot.addFrame(sf::IntRect(64, 50, 32, 50));
	shoot.addFrame(sf::IntRect(96, 50, 32, 50));
	shoot.addFrame(sf::IntRect(128, 50, 32, 50));
	shoot.addFrame(sf::IntRect(160, 50, 32, 50));
	shoot.setFrameSpeed(0.07f);

	// Dt
	this->dtMultiplier = 62.5f;

	// Multiplayer counter
	this->playerNum = Player::players;
	Player::players++;

	// Time
	this->keyTimeMax = 10.f;
	this->keyTime = this->keyTimeMax;

	// Timers
	this->shootTimerMax = 1.f;
	this->shootTimer = this->shootTimerMax;
	this->damageTimerMax = 40.f;
	this->damageTimer = this->damageTimerMax;

	// Stats
	this->xpNext = 20 + static_cast<int>((50 / 3)*((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));
	 
	// Sprite
	this->sprite.setTexture(bodyTextures[0]);
	this->sprite.setTextureRect(jet.getCurrentFrame());
	this->sprite.setScale(4.f, 4.f);

	// Set player center 
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width/2; 
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height/2;

	// Movement
	this->maxVelocity = 25.f;
	this->acceleration = 0.8f;
	this->deacceleration = 0.3f;

	this->shooting = false;

	// Controls
	this->controls.add(int(UP));
	this->controls.add(int(DOWN));
	this->controls.add(int(LEFT));
	this->controls.add(int(RIGHT));
	this->controls.add(int(SHOOT));
	this->controls.add(int(STATS));

}

Player::~Player(){}

///////   GETTERS AND SETTERS   ///////  
#pragma region Getters&Setters

Projectiles& Player::getProjectile(unsigned index){
	if (index < 0 || index > this->projectiles.size())
		throw"out of bounds";
	return this->projectiles[index];
}

void Player::removeBullet(unsigned index){
	if (index < 0 || index > this->projectiles.size())
		throw"out of bounds";
	this->projectiles.remove(index);
}

int Player::getDamage()const{ 
	int pDamage = 0;
		pDamage = rand() % this->damageMax + this->damage;
	return pDamage;
}

void Player::takeDamage(int damage){ 
	this->hp -= damage; 
	this->damageTimer = 0; 
	// stun player
	this->currentVelocity.x += -this->normDir.x*10.f;
	this->currentVelocity.y += -this->normDir.y*10.f;
}

std::string Player::getStatsAsString()const{
	return
		"Level: " + std::to_string(this->level) +
		"\nExp: " + std::to_string(this->xp) + " / " + std::to_string(this->xpNext) +
		"\nStatpoints: " + std::to_string(this->statPoints) +
		"\nHP: " + std::to_string(this->hp) + " / " + std::to_string(this->hpMax) + " ( +" + std::to_string(this->hpAdded) + ") "
		"\nDamage: " + std::to_string(this->damage) + " - " + std::to_string(this->damageMax) +
		"\n\nScore: " + std::to_string(this->score) +
		"\n\Level Multiplier: " + std::to_string(this->levelMultiplier);
}


void Player::setPos(float x, float y){
	this->sprite.setPosition(Vector2f(x, y));
	////Update player center
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2;
}


bool Player::gainXp(int exp){
	this->xp += exp;
	return this->updateLeveling();
}
#pragma endregion


///////   COMBAT   ///////    
void Player::combat(const float &dt){
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::SHOOT])) && this->shootTimer >= this->shootTimerMax){
		this->shooting = true;
		// Create projectile
		this->projectiles.add(
			Projectiles(
				0,
				Vector2f(this->playerCenter.x, this->playerCenter.y - 100.f),
				Vector2f(7.f, 7.f),
				Vector2f(0.f, -1.f),
				20.f, 
				60.f, 
				5.f, 
				this->getDamage()
				));
		this->shootTimer = 0; // Reset timer
		shoot.animate(dt);
		sprite.setTextureRect(jet.getCurrentFrame());
	}

	// Player is hit
	if (this->isDamageCooldown()){
		if ((int)this->damageTimer % 2 == 0){
			this->sprite.setColor(Color::Red); // Flash red
		}
		else{
			this->sprite.setColor(Color::White);
		}
	}
	else{
		this->sprite.setColor(Color::White);
	}
}

///////   MOVEMENT   ///////  
void Player::movement(View& view, const float& dt){
	//Update normalized direction
	this->normDir = normalize(this->currentVelocity, vectorLength(this->currentVelocity));

	// UP
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::UP]))){
		this->direction.x = 0.f;
		this->direction.y = -1.f;
		if (this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0)
			this->currentVelocity.y += this->direction.y * this->acceleration* dt * this->dtMultiplier;
	}
	// DOWN
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::DOWN]))){
		this->direction.x = 0.f;
		this->direction.y = 1.f;
		if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0)
			this->currentVelocity.y += this->direction.y * this->acceleration* dt * this->dtMultiplier;
	}
	// LEFT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::LEFT]))){
		this->direction.x = -1.f;
		this->direction.y = 0.f;
		if (this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0)
			this->currentVelocity.x += this->direction.x * this->acceleration* dt * this->dtMultiplier;

	}
	// RIGHT
	if (Keyboard::isKeyPressed(Keyboard::Key(this->controls[controls::RIGHT]))){
		this->direction.x = 1.f;
		this->direction.y = 0.f;
		if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0)
			this->currentVelocity.x += this->direction.x * this->acceleration* dt * this->dtMultiplier;
	}

	// Deacceleration 
	if (this->currentVelocity.x > 0){
		this->currentVelocity.x -= this->deacceleration * dt * this->dtMultiplier;
        if (this->currentVelocity.x < 0)
			this->currentVelocity.x = 0;
	}

	else if (this->currentVelocity.x < 0){
		this->currentVelocity.x += this->deacceleration * dt * this->dtMultiplier;
		if (this->currentVelocity.x > 0)
			this->currentVelocity.x = 0;
	}

	if (this->currentVelocity.y > 0){
		this->currentVelocity.y -= this->deacceleration * dt * this->dtMultiplier;
		if (this->currentVelocity.y < 0)
			this->currentVelocity.y = 0;
	}

	else if (this->currentVelocity.y < 0){
		this->currentVelocity.y += this->deacceleration * dt * this->dtMultiplier;
		if (this->currentVelocity.y > 0)
			this->currentVelocity.y = 0;
	}

	// Movement
	this->sprite.move(this->currentVelocity.x * dt * this->dtMultiplier, this->currentVelocity.y * dt * this->dtMultiplier);

	// Update player center
	this->playerCenter.x = this->sprite.getPosition().x + this->sprite.getGlobalBounds().width / 2;
	this->playerCenter.y = this->sprite.getPosition().y + this->sprite.getGlobalBounds().height / 2;


	// Window bounds collision
	//Left
	if (this->sprite.getPosition().x <= view.getCenter().x - view.getSize().x / 2){
		this->sprite.setPosition(view.getCenter().x - view.getSize().x / 2 + 10.f, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}
	//Right
	else if (this->sprite.getPosition().x + this->getBounds().width >= view.getCenter().x + view.getSize().x / 2){
		this->sprite.setPosition(view.getCenter().x + view.getSize().x / 2 - this->getBounds().width, this->sprite.getPosition().y);
		this->currentVelocity.x = 0.f;
	}
	//Top
	if (this->sprite.getPosition().y <= view.getCenter().y - view.getSize().y / 2){
		this->sprite.setPosition(this->sprite.getPosition().x, view.getCenter().y - view.getSize().y / 2);
		this->currentVelocity.y = 0.f;
	}
	//Bottom
	else if (this->sprite.getPosition().y + this->getBounds().height >= view.getCenter().y + view.getSize().y / 2){
		this->sprite.setPosition(this->sprite.getPosition().x, view.getCenter().y + view.getSize().y / 2 - this->getBounds().height);
		this->currentVelocity.y = 0.f;
	}
}

///////   LEVEL UP   ///////  
bool Player::updateLeveling(){
	if (this->xp >= this->xpNext){
		this->level++; // Level up
		this->statPoints++;
		this->xp -= this->xpNext;
		this->xpNext = static_cast<int>((50 / 3) * ((pow(level, 3) - 6 * pow(level, 2)) + 17 * level - 12));

		// Level up player ship 
		this->levelMultiplier++; 

		this->updateStats();
		this->hp = hpMax; // set health to max
		return true;
	}
	return false;
}

///////   STATS   ///////  
void Player::updateStats(){
	this->hpMax = this->hpAdded + this->levelMultiplier * 5;
	this->damageMax = 2 + this->levelMultiplier * 2;
	this->damage = 1 + this->levelMultiplier;
}

///////   UPDATE   ///////  
void Player::update(View &view, const float &dt){

	// Keep timers up to date
	this->shootTimerMax = 1.f;
	if (this->shootTimer < this->shootTimerMax) {
		shoot.animate(dt);
		sprite.setTextureRect(shoot.getCurrentFrame());
		this->shootTimer += 1.f * dt * this->dtMultiplier;
	}
	else {
		jet.animate(dt);
		sprite.setTextureRect(jet.getCurrentFrame());
	}


	if (this->damageTimer < this->damageTimerMax)
		this->damageTimer += 1.f * dt * this->dtMultiplier;

	this->movement(view, dt);
	this->combat(dt);
}

///////   RENDER   ///////  
void Player::render(RenderTarget &target){
	for (size_t i = 0; i < this->projectiles.size(); i++){
		this->projectiles[i].render(target);
	}
	target.draw(this->sprite);
}

///////   RESET   ///////  
void Player::reset(){
	this->sprite.setPosition(Vector2f(1000.f, 2500.f + Player::playerNum * 150.f));
	this->hpMax = 10;
	this->hpAdded = 10;
	this->hp = this->hpMax;
	this->levelMultiplier = 0;
	this->level = 1;
	this->xp = 0;
	this->xpNext = 0;
	this->statPoints = 0;
	this->score = 0;
	this->updateStats();
	this->currentVelocity.x = 0;
	this->currentVelocity.y = 0;
	this->projectiles.clear();
	this->shootTimer = this->shootTimerMax;
	this->damageTimer = this->damageTimerMax;
}
