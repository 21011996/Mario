#include <vector>
#include <SFML\Graphics.hpp>

extern float offsetX, offsetY;

extern const int H;
extern const int W;

extern std::vector<sf::String> TileMap;

extern const int tilesize;



extern const int windowW;
extern const int windowH;

extern const int tileinanimplayer;
extern const int tileinanimenemy;

extern const float gravitystrength;
extern const float movespeedX;
extern const float movespeedY;
extern const float movespeedenemy;
extern const float animationspeed;

struct sprite {
	int x;
	int y;
	int width;
	int height;
};

extern sprite player;
extern sprite npc;
extern sprite block;
extern sprite cloud;
extern sprite pipe;
extern sprite shroom;
extern sprite banner;