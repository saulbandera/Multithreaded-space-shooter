#include "Background.h"

Background::Background(unsigned long sizeX, unsigned long sizeY){
	this->dtMultiplier = 62.5f;
	this->scrollSpeed = 2.f;
	temp.loadFromFile("Lunar Lander Upload/Space Background/stars2.png");
	this->background.setSize(Vector2f(2000, 1100));
	this->background.setTextureRect(IntRect(0, 0, 2000, 1100));
	this->background.setTexture(pTexture);
	this->backgrounds.add(background);
}

Background::~Background(){}

///////   UPDATE   ///////   
void Background::update(const float &dt, View &view){
	// Loop the backgrounds so that they scroll
	bool bgRemoved = false;
	for (size_t i = 0; i < this->backgrounds.size() && !bgRemoved; i++){

		this->backgrounds[i].move(0.f, this->scrollSpeed * dt * this->dtMultiplier * 0.8f);
		
		if (this->backgrounds.size() < 3 && this->backgrounds[i].getPosition().y >= 0){
			this->background.setPosition(this->background.getPosition().x,
				this->backgrounds[i].getPosition().y - this->backgrounds[i].getGlobalBounds().height);
			this->backgrounds.add(background); // Add background on top 
		}

		else if (this->backgrounds[i].getPosition().y >= view.getCenter().y + view.getSize().y / 2){
			// Remove background once it disappears from view
			bgRemoved = true;
		}

		if (bgRemoved)
			this->backgrounds.remove(i);
	}
}

///////   RENDER   ///////   
void Background::render(RenderTarget &target, View &view, Font &font){
	for (size_t i = 0; i < this->backgrounds.size(); i++){
			target.draw(this->backgrounds[i]);
     }
}

///////   RESET   ///////   
void Background::reset(View& view){
	//Reset background
	this->backgrounds.clear();
	this->background.setPosition(Vector2f(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/2));
	this->backgrounds.add(this->background);
}