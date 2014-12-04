#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include "consts.h"
#include "player.h"
#include "enemy.h"
#include <string>

using namespace sf;
using namespace std;

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
	enemy.set(env,npc.width*tilesize,npc.height*tilesize);

	Clock clock;

	p.sprite.setTextureRect(IntRect(0,0,player.width,player.height)); // initial texture applyment

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
		if (!enemy.life) {window.draw(killtext); frames++;}


		for (int i=0; i<H; i++)
			for (int j=0; j<W; j++)
				{
					if (TileMap[i][j]=='P') envSp.setTextureRect(IntRect(block.x,block.y,block.width,block.height));
					if (TileMap[i][j]=='0') continue;
					if (TileMap[i][j]=='_') continue;
					if (TileMap[i][j]=='C') envSp.setTextureRect(IntRect(cloud.x,cloud.y,cloud.width,cloud.height));
					if (TileMap[i][j]=='T') envSp.setTextureRect(IntRect(pipe.x,pipe.y,pipe.width,pipe.height));
					if (TileMap[i][j]=='k') envSp.setTextureRect(IntRect(shroom.x,shroom.y,shroom.width,shroom.height));
					if (TileMap[i][j]=='G') envSp.setTextureRect(IntRect(banner.x,banner.y,banner.width,banner.height));

					envSp.setPosition(j*tilesize-offsetX,i*tilesize-offsetY);
					window.draw(envSp);
				}
		window.draw(p.sprite);
		window.draw(enemy.sprite);
		window.display();
	}
	return 0;
}