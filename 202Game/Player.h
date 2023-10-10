#pragma once
#include "Includes.h"
#include"Projectiles.h"
#include "Animation.h"


class Player{
private:
	// Animations
	Animation jet;
	Animation shoot;

	Sprite sprite;
	unsigned playerNum;
	float dtMultiplier;
    bool shooting;
	// Positions
	Vector2f playerCenter;

	// Time
	float keyTimeMax, keyTime;
	float shootTimer, shootTimerMax;
	float damageTimer, damageTimerMax;


	dArr<Projectiles> projectiles;
	dArr<int> controls;


	// Movement
	float maxVelocity;
	float acceleration, deacceleration;
	Vector2f currentVelocity;
	Vector2f direction;
	Vector2f normDir;
	int levelMultiplier;

	// Progression 
	int level;
	int statPoints;
	int score;
	int xp, xpNext;


	// Health and damage
	int hp, hpMax, hpAdded;
	int damage, damageMax;

public:
	Player(
		int UP = Keyboard::Up, 
		int DOWN = Keyboard::Down,
		int LEFT = Keyboard::Left, 
		int RIGHT = Keyboard::Right,
		int SHOOT = Keyboard::Space,
		int STATS = Keyboard::Tab
	);
	virtual ~Player();

	//Getters and Setters
	Projectiles& getProjectile(unsigned index);
	void removeBullet(unsigned index);


	inline Vector2f getPosition() const { return this->sprite.getPosition(); }
	inline FloatRect getBounds()const { return this->sprite.getGlobalBounds(); }
	inline const int getProjectileSize() const { return this->projectiles.size(); }
	inline const int getScore()const { return this->score; }
    inline const String getHpAsString()const { return std::to_string(this->hp) + "/" + std::to_string(this->hpMax); }
	inline const int& getLevel()const { return this->level; }
	inline const int& getXp()const { return this->xp; }
	inline const int& getXpNext()const { return this->xpNext; }
	inline const bool intersects(FloatRect rect)const { return this->sprite.getGlobalBounds().intersects(rect); }
	inline bool isAlive()const { return this->hp > 0; }
	inline bool isDamageCooldown()const { return this->damageTimer < this->damageTimerMax; }
	inline void gainScore(int score) { this->score += score; }
	
	int getDamage()const;
	void takeDamage(int damage);
	std::string getStatsAsString()const;
	void setPos(float x, float y);




	//Functions
	bool gainXp(int xp);

	void combat(const float &dt);

	void movement(View &view, const float &dtconst);
	float vectorLength(Vector2f v){
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	Vector2f normalize(Vector2f v, float length){
		if (length == 0)
			return Vector2f(0.f, 0.f);
		else
			return v / length;
	}

	bool updateLeveling();
	void updateStats();
	void update(View &view, const float &dt);
	void render(RenderTarget &target);
	void reset();

	//Statics
	static unsigned players;

	//Static Textures
	static dArr<Texture> bodyTextures;
	static void initTextures();
	enum controls { UP = 0, DOWN, LEFT, RIGHT, SHOOT, STATS };
};

