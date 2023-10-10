#pragma once
#include "dArr.h"
#include "Player.h"
#include "Includes.h"


class Enemy{
private:
	float dtMultiplier;
	Sprite sprite;
	Color color, damageColor;

	Vector2f moveDirection, normalizedMoveDir;
	Vector2f lookDirection, normalizedLookDir;
	float maxVelocity;

	float damageTimer, damageTimerMax;

	int enemyType;
	int hp, hpMax;
	int damageMin, damageMax;

	int playerFollowNum;

	bool isKilled;

public:
	Enemy() = default;
	Enemy(
		View& view,
		bool randomPos,
		Vector2f position, 
		Vector2f direction, 
		float maxVelocity,
		int type, 
		int scalar,
		int playerFollowNum);

	virtual ~Enemy();
	// Getters and Setters
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline Vector2f getPosition()const { return this->sprite.getPosition(); }
	inline const int getDamage()const { return rand() % this->damageMax + this->damageMin; }
	inline const int getHP()const { return this->hp; }
	inline const int getHPMax()const { return this->hpMax; }
	inline const int& getPlayerFollowNum()const { return this->playerFollowNum; }
	inline void setKilled(bool k) {isKilled = k; }
	inline bool getKilled() { return isKilled; }


	// Functions
	void collision();
	void takeDamage(int damage);
	void update(const float &dt, Vector2f playerPosition);
	void render(RenderTarget &target);

	float vectorLength(Vector2f v){
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}
	Vector2f normalize(Vector2f v, float length){
		if (length == 0)
			return Vector2f(0.f, 0.f);
		else
			return v / length;
	}


	// Static
	static dArr<Texture> textures;
	static int numTextures;
	static void initTextures();
	static int numTypes;

	enum eTypes { 
		MOVE = 0, 
		FOLLOW};
};

