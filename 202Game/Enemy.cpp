#include "Enemy.h"
// Static define
dArr<Texture> Enemy::textures;
int Enemy::numTextures;

void Enemy::initTextures(){
	// Load enemy ship textures
	Texture tex;
	tex.loadFromFile("Lunar Lander Upload/Ships/enemy2.png");
	Enemy::textures.add(Texture(tex));
	tex.loadFromFile("Lunar Lander Upload/Ships/enemy4.png");
	Enemy::textures.add(Texture(tex));
	Enemy::numTextures = Enemy::textures.size();
}

int Enemy::numTypes = 2;

Enemy::Enemy(
	View& view,
	bool randomPos,
	Vector2f position, 
	Vector2f moveDirection,
	float maxVelocity,
	int type,
	int scalar,
	int playerFollowNr){

	this->dtMultiplier = 92.5f;
	this->enemyType = type;

	this->sprite.setTexture(Enemy::textures[this->enemyType]);
	this->sprite.setOrigin(this->sprite.getGlobalBounds().width/2, this->sprite.getGlobalBounds().height/2);
	
	this->color = Color::White;
	this->damageColor = Color::Red;

	this->damageTimerMax = 4.f;
	this->damageTimer = 0;

	this->isKilled = false;
	this->moveDirection = moveDirection;

	this->hpMax = hpMax;
	this->hp = this->hpMax;
	this->playerFollowNum = playerFollowNr;

	// Enemy selection
	switch (this->enemyType){
		case MOVE: // Moves downwards
			this->sprite.setScale(Vector2f(3.f, 3.f));
			this->hpMax = (rand()% 5 + 1) * scalar;
			this->hp = this->hpMax;
			this->damageMax = (rand() % 3 + 1) * scalar;
			this->damageMin = (rand() % 1 + 1) * scalar;
			if (maxVelocity < 0)
				this->maxVelocity = rand() % 20 + 5;
			else
				this->maxVelocity = maxVelocity;
				break;

		case FOLLOW: // Moves downwards following player
			this->sprite.setScale(Vector2f(3.f, 3.f));
			this->hpMax = (rand() % 3 + 1) * scalar;
			this->hp = this->hpMax;
			this->damageMax = (rand() % 2 + 1) * scalar;
			this->damageMin = (rand() % 1 + 1) * scalar;
			if (maxVelocity < 0)
				this->maxVelocity = rand() % 6 + 3;
			else
				this->maxVelocity = maxVelocity;
				break;
		
		default:
			this->hpMax = (rand() % 2 + 1) * scalar;
			this->hp = this->hpMax;
			this->damageMax = (rand() % 2 + 1) * scalar;
			this->damageMin = (rand() % 1 + 1) * scalar;
			if (maxVelocity < 0)
				this->maxVelocity = rand() % 6 + 3;
			else
				this->maxVelocity = maxVelocity;
				break;
	}

	if (randomPos){
		this->sprite.setPosition( 
			rand() % static_cast<int>((view.getCenter().x + view.getSize().x)) + static_cast<int>((view.getCenter().x - view.getSize().x)),
			0);
	}
	else{
		this->sprite.setPosition(position);
	}
}

Enemy::~Enemy(){}

///////   COLLISION   ///////   
void Enemy::collision(){
	this->damageTimer = this->damageTimerMax;
}

///////   TAKE DAMAGE   ///////   
void Enemy::takeDamage(int damage){
	this->hp -= damage;
	this->damageTimer = this->damageTimerMax;
	if (this->hp <= 0){
		this->hp = 0;
	}
}

///////   UPDATE   ///////   
void Enemy::update(const float &dt, Vector2f playerPosition){
	switch (this->enemyType){

	case MOVE:
		this->sprite.move(
			this->moveDirection.x * this->maxVelocity * dt * this->dtMultiplier,
			this->moveDirection.y * this->maxVelocity * dt * this->dtMultiplier
		);
		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));
		break;

	case FOLLOW:
		if (this->sprite.getPosition().y < playerPosition.y){
			this->moveDirection.x = playerPosition.x - this->sprite.getPosition().x;
			this->moveDirection.y = playerPosition.y - this->sprite.getPosition().y;
		}
		this->normalizedMoveDir = normalize(this->moveDirection, vectorLength(this->moveDirection));
		if (this->normalizedMoveDir.x > 0.3)
			this->normalizedMoveDir.x = 0.3;

		else if(this->normalizedMoveDir.x > -0.3)
			this->normalizedMoveDir.x = -0.3;

		if (this->normalizedMoveDir.y > -0.7)
			this->normalizedMoveDir.y = 0.7;

		// Face player
		this->sprite.setRotation(atan2(this->normalizedMoveDir.y,this->normalizedMoveDir.x) * 180/3.14159265359-90);

		this->sprite.move(
			this->normalizedMoveDir.x * this->maxVelocity * dt * this->dtMultiplier,
			this->normalizedMoveDir.y * this->maxVelocity * dt * this->dtMultiplier
		);
			break;

	default:
		break;
	}

	// Enemy hits player
	if (this->damageTimer > 0.f){
		this->damageTimer -= 1.f * dt * dtMultiplier;
		this->sprite.setColor(damageColor);
		this->sprite.move(
			10.f * -this->normalizedMoveDir.x * this->damageTimer * dt * dtMultiplier,
			10.f * -this->normalizedMoveDir.y * this->damageTimer * dt * dtMultiplier);
	}
	else
		this->sprite.setColor(color);
}

///////   RENDER   ///////   
void Enemy::render(RenderTarget &target){
	target.draw(this->sprite);
}