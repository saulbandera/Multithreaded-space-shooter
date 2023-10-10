#pragma once
#include "Includes.h"
class Background{
private:
	float dtMultiplier;
	float scrollSpeed;
	RectangleShape background;
	dArr<RectangleShape> backgrounds;

public:
	Background(unsigned long sizeX, unsigned long sizeY);
	virtual ~Background();

	//Functions
	void update(const float &dt, View &view);
	void render(RenderTarget &target, View &view, Font &font);
	void reset(View& view);

	// Textures
	Texture temp;
	const sf::Texture* pTexture = &temp;
};

