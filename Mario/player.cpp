#include "player.h"
#include "consts.h"
#include <SFML/Graphics.hpp>

using namespace sf;
Player::Player(Texture &image)                // constructor
	{
		sprite.setTexture(image);
		rect = sf::FloatRect(player.x,player.y,player.width,player.height);
		dx=dy=0;
		currentFrame=0;
		endofgame=false;
	};

void Player::update(float time)               // movements + collisions
	{
		rect.left +=dx*time;
		Collision("x");                     // collision|X
		
		if (!onGround) dy=dy+gravitystrength*time; // gravity apply
		rect.top +=dy*time;
		onGround=false;
		Collision("y");                     // collision|Y
		
		currentFrame += animationspeed*time;       // animation
		if (currentFrame > tileinanimplayer) currentFrame -=tileinanimplayer;
		if (dx>0) sprite.setTextureRect(IntRect(player.width*int(currentFrame),0,player.width,player.height));
		if (dx<0) sprite.setTextureRect(IntRect(player.width*int(currentFrame)+player.width,0,-player.width,player.height));

		sprite.setPosition(rect.left - offsetX,rect.top - offsetY); // moving sprite in window
		dx=0;
	};


void Player::Collision(String dir)
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
	};