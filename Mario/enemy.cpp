#include <SFML/Graphics.hpp>
#include "enemy.h"
#include "consts.h"

using namespace sf;

	void Enemy::set(Texture &image, int x, int y)
   {
	sprite.setTexture(image);
	rect = FloatRect(x,y,tilesize,tilesize);

    dx=movespeedenemy;
	currentFrame = 0;
	life=true;
   };

   void Enemy::update(float time)
   {	
	 rect.left += dx * time;
	
     Collision();
  

     currentFrame += time * animationspeed;
     if (currentFrame > tileinanimenemy) currentFrame -= tileinanimenemy;

    sprite.setTextureRect(IntRect(npc.x*int(currentFrame),0,tilesize,tilesize));
	if (!life) sprite.setTextureRect(IntRect(npc.y,0,tilesize,tilesize));


	sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	  
   };


   void Enemy::Collision()
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
  };
