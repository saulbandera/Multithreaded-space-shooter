#pragma once
#include"TextTag.h"


class Projectiles{
private:
	float dtMultiplier;
	Sprite sprite;
	int projectileType;
	Vector2f currentVelocity;
	Vector2f direction;
	float initialVelocity, maxVelocity;
	float acceleration;
	int damage;
public:
	Projectiles(int type,
		Vector2f position, Vector2f scale,
		Vector2f direction, float initialVelocity,
		float maxVelocity, float acceleration,
		int damage);
	virtual ~Projectiles();

	// Getters and Setters
	inline FloatRect getGlobalBounds()const { return this->sprite.getGlobalBounds(); }
	inline const Vector2f& getPosition()const { return this->sprite.getPosition(); }
	inline Vector2f getVel()const { return this->currentVelocity; }
	inline int getDamage()const { return this->damage; }
	inline void setPosition(Vector2f position) { this->sprite.setPosition(position); }

	// Functions
	void update(const float &dt);
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

	// Textures
	static dArr<Texture> textures;
	static int nrOfTextures;
	static void initTextures();
};

