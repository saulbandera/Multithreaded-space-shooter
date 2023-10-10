#include "Projectiles.h"

dArr<Texture> Projectiles::textures;
int Projectiles::nrOfTextures;

void Projectiles::initTextures(){
	// Load projectile textures
	Texture tex;
	tex.loadFromFile("Lunar Lander Upload/Effects/Fx_01.png");
	Projectiles::textures.add(tex);
	Projectiles::nrOfTextures = Projectiles::textures.size();
}

Projectiles::Projectiles(int type,
	Vector2f position, Vector2f scale,
	Vector2f direction, float initialVelocity,
	float maxVelocity, float acceleration,
	int damage){

	this->dtMultiplier = 62.5f;
	this->damage = damage;

	this->projectileType = type;

	this->sprite.setTexture(Projectiles::textures[this->projectileType]);

	this->initialVelocity = initialVelocity;
	this->maxVelocity = maxVelocity;
	this->acceleration = acceleration;
	this->direction = direction;
	this->currentVelocity = Vector2f(initialVelocity * this->direction.x, initialVelocity * this->direction.y);
	
	this->sprite.setOrigin(this->sprite.getGlobalBounds().width/2,this->sprite.getGlobalBounds().height/2);
	this->sprite.setScale(scale);
	this->sprite.setPosition(position);
	this->sprite.setRotation(atan2(this->direction.y, this->direction.x) * 180/3.14159265359);
}

Projectiles::~Projectiles(){}

///////   UPDATE   ///////   
void Projectiles::update(const float &dt){
	// Projectile movement
	if (this->acceleration > 0.f){
		if (this->currentVelocity.x < this->maxVelocity && this->direction.x > 0.f 
			|| this->currentVelocity.x > -this->maxVelocity && this->direction.x < 0.f)
			this->currentVelocity.x += this->acceleration * this->direction.x * dt * this->dtMultiplier;

		if (this->currentVelocity.y < this->maxVelocity && this->direction.y > 0.f
			|| this->currentVelocity.y > -this->maxVelocity && this->direction.y < 0.f)
			this->currentVelocity.y += this->acceleration * this->direction.y * dt * this->dtMultiplier;
	}
	else{
		this->currentVelocity = Vector2f(this->maxVelocity * this->direction.x, this->maxVelocity * this->direction.y);
	}
	this->sprite.move(this->currentVelocity * dt * this->dtMultiplier);
}


///////   RENDER   ///////   
void Projectiles::render(RenderTarget &target){
	target.draw(this->sprite);
}