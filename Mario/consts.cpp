#include <SFML\Graphics.hpp>
#include <vector>
#include "consts.h"
using namespace sf;
using namespace std;
float offsetX=0, offsetY=0;

const int H = 25;
const int W = 150;

vector<String> TileMap(H, String());

const int tilesize=16;



const int windowW = 600;
const int windowH = 400;

const int tileinanimplayer = 7;
const int tileinanimenemy = 2;

const float gravitystrength = 0.0005;
const float movespeedX = 0.1;
const float movespeedY = 0.4;
const float movespeedenemy = 0.05;
const float animationspeed = 0.005;

/*struct sprite {
	int x;
	int y;
	int width;
	int height;
};*/

sprite player = {100,64,55,57};
sprite npc = {18,58,48,21};
sprite block = {0, 25, 16, 16};
sprite cloud = {27,23,64,32};
sprite pipe = {0,50,32,49};
sprite shroom = {0,0,16,16};
sprite banner = {49,66,12,36};
