#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <string>

using namespace sf;
using namespace std;

float offsetX=0, offsetY=0;
int tilesize=16;

const int verybigNUMBER = INT_MAX/1000;

const int H = 25;
const int W = 150;

const int windowW = 600;
const int windowH = 400;

const float gravitystrength = 0.0005;
const float movespeedX = 0.1;
const float movespeedY = 0.4;
const float movespeedenemy = 0.05;
const float animationspeed = 0.005;

int tileinanimplayer = 7;
int tileinanimenemy = 2;

int playersize[4] = {100,64,55,57};
int npcsize[4] = {18,58,48,21};
int enviromentconsts[5][4] = {{0,25,16,16},{27,23,64,32},{0,50,32,49},{0,0,16,16},{49,66,12,36}};

String TileMap[H];


class Player{
public:

float dx,dy;
FloatRect rect;
bool onGround;
Sprite sprite;
float currentFrame;
bool endofgame;

	Player(Texture &image)                // constructor
	{
		sprite.setTexture(image);
		rect = FloatRect(playersize[0],playersize[1],playersize[2],playersize[3]);
		dx=dy=0;
		currentFrame=0;
		endofgame=false;
	}

	void update(float time)               // movements + collisions
	{
		rect.left +=dx*time;
		Collision("x");                     // collision|X
		
		if (!onGround) dy=dy+gravitystrength*time; // gravity apply
		rect.top +=dy*time;
		onGround=false;
		Collision("y");                     // collision|Y
		
		currentFrame += animationspeed*time;       // animation
		if (currentFrame > tileinanimplayer) currentFrame -=tileinanimplayer;
		if (dx>0) sprite.setTextureRect(IntRect(playersize[2]*int(currentFrame),0,playersize[2],playersize[3]));
		if (dx<0) sprite.setTextureRect(IntRect(playersize[2]*int(currentFrame)+playersize[2],0,-playersize[2],playersize[3]));

		sprite.setPosition(rect.left - offsetX,rect.top - offsetY); // moving sprite in window
		dx=0;
	}


	void Collision(String dir)
	{
		for (int i=rect.top/tilesize; i<(rect.top+rect.height)/tilesize; i++)         //checking closest tiles
			for (int j=rect.left/tilesize; j<(rect.left+rect.width)/tilesize;j++)
			{
				if ((TileMap[i][j]=='P') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='T'))
				{
					if ((dx>0) && (dir=="x")) rect.left = j*tilesize-rect.width; //X collisions
					if ((dx<0) && (dir=="x")) rect.left = j*tilesize+tilesize;
					if ((dy>0) && (dir=="y"))                              //Y collisions
						{
							rect.top = i*tilesize-rect.height;
							dy=0;
							onGround=true;
						}
					if ((dy<0) && (dir=="y"))
						{
							rect.top = i*tilesize+tilesize;
							dy=0;
						}
				}
				if (TileMap[i][j]=='G') endofgame=true;

			}	
	}
};

class Enemy{
public:

float dx,dy;
FloatRect rect;
bool onGround;
Sprite sprite;
float currentFrame;
bool life;

	void set(Texture &image, int x, int y)
   {
	sprite.setTexture(image);
	rect = FloatRect(x,y,tilesize,tilesize);

    dx=movespeedenemy;
	currentFrame = 0;
	life=true;
   }

   void update(float time)
   {	
	 rect.left += dx * time;
	
     Collision();
  

     currentFrame += time * animationspeed;
     if (currentFrame > tileinanimenemy) currentFrame -= tileinanimenemy;

    sprite.setTextureRect(IntRect(npcsize[0]*int(currentFrame),0,tilesize,tilesize));
    if (!life) sprite.setTextureRect(IntRect(npcsize[1],0,tilesize,tilesize));


	sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	  
   }


   void Collision()
  {
	for (int i = rect.top/tilesize ; i<(rect.top+rect.height)/tilesize; i++)
		for (int j = rect.left/tilesize; j<(rect.left+rect.width)/tilesize; j++)
			 if ((TileMap[i][j]=='P') || (TileMap[i][j]=='0'))
				{ 
                  if (dx>0)
				   { 
						rect.left =  j*tilesize - rect.width; dx*=-1; 
				   }
					else if (dx<0)
					{ 
						rect.left =  j*tilesize + tilesize;  dx*=-1; 
					}
											
				}
  }

};

int main()
{
	RenderWindow window( VideoMode(windowW,windowH),"Teemario");

	char s[151];
	std::ifstream cin("map.txt");
	for (int i=0; i<H; i++)
	{
		cin>>s;
		TileMap[i]=s;
	}
	cin.close();

	Texture teemo;                            //loading textures, applying them to sprites
	teemo.loadFromFile("teeto.png");
	Texture env;
	env.loadFromFile("Mario_tileset.png");
	Sprite envSp(env);
	Texture background;
	background.loadFromFile("ar.png");
	Sprite backSp(background);

	Font font;                                // preparing message
	font.loadFromFile("1234.otf");
	Text killtext("You killed him!",font,50);
	killtext.setColor(Color::Red);
	killtext.setPosition(10,10);
	Text wintext("You won!",font,100);
	wintext.setColor(Color::Blue);
	wintext.setPosition(0,windowH/2);
	int frames = 0;
	

	Player p(teemo);                              // setting up units
	Enemy enemy;
	enemy.set(env,npcsize[2]*tilesize,npcsize[3]*tilesize);

	Clock clock;

	p.sprite.setTextureRect(IntRect(0,0,playersize[2],playersize[3])); // initial texture applyment

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/(windowW+windowH/2);

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p.dx = -movespeedX;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = movespeedX;
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p.onGround)
			{
				p.dy=-movespeedY;
				p.onGround=false;
			}
		}
		p.update(time);
		enemy.update(time);

		if (p.endofgame) 
			{
				window.draw(wintext);
				window.display();
				while (clock.getElapsedTime().asMicroseconds()<verybigNUMBER) {}
				window.close();
			}

		if  (p.rect.intersects( enemy.rect ) )
		 {
			 if (enemy.life) {
			 if (p.dy>0) { enemy.dx=0; p.dy=-0.2; enemy.life=false; }
			 else p.sprite.setColor(Color::Red);
			 }
		 }

		if (p.rect.left>windowW/2) offsetX = p.rect.left - windowW/2;  // Drawing

		window.clear();
		window.draw(backSp);
		if ((!enemy.life) && (frames<verybigNUMBER/1000)) {window.draw(killtext); frames++;}


		for (int i=0; i<H; i++)
			for (int j=0; j<W; j++)
				{
					if (TileMap[i][j]=='P') envSp.setTextureRect(IntRect(enviromentconsts[0][0],enviromentconsts[0][1],enviromentconsts[0][2],enviromentconsts[0][3]));
					if (TileMap[i][j]=='0') continue;
					if (TileMap[i][j]=='_') continue;
					if (TileMap[i][j]=='C') envSp.setTextureRect(IntRect(enviromentconsts[1][0],enviromentconsts[1][1],enviromentconsts[1][2],enviromentconsts[1][3]));
					if (TileMap[i][j]=='T') envSp.setTextureRect(IntRect(enviromentconsts[2][0],enviromentconsts[2][1],enviromentconsts[2][2],enviromentconsts[2][3]));
					if (TileMap[i][j]=='k') envSp.setTextureRect(IntRect(enviromentconsts[3][0],enviromentconsts[3][1],enviromentconsts[3][2],enviromentconsts[3][3]));
					if (TileMap[i][j]=='G') envSp.setTextureRect(IntRect(enviromentconsts[4][0],enviromentconsts[4][1],enviromentconsts[4][2],enviromentconsts[4][3]));

					envSp.setPosition(j*tilesize-offsetX,i*tilesize-offsetY);
					window.draw(envSp);
				}
		window.draw(p.sprite);
		window.draw(enemy.sprite);
		window.display();
	}
	return 0;
}