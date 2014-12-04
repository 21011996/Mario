#include <SFML/Graphics.hpp>


class Player{
public:

float dx,dy;
sf::FloatRect rect;
bool onGround;
sf::Sprite sprite;
float currentFrame;
bool endofgame;

	Player(sf::Texture &image);                // constructor

	void update(float time);               // movements + collisions
	
	void Collision(sf::String dir);
	
};

