#pragma once
#include "Includes.h"

class TextTag{
private:
	float dtMultiplier;
	Text text;

	float timerMax, timer;
	float speed;
	Vector2f direction;
	bool accelerate;

public:
	TextTag(Font *font, std::string text, 
		const Color color,
		Vector2f position, Vector2f direction,
		unsigned int size, float timerMax, 
		bool accelerate);
	virtual ~TextTag();

	// Getters Setters
	inline const float& getTimer()const { return this->timer; }

	// Functions
	void update(const float &dt);
	void render(RenderTarget &target);
};

