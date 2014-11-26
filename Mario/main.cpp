#include <SFML/Graphics.hpp>

using namespace sf;

float offsetX=0, offsetY=0;

const int H = 25;
const int W = 150;

String TileMap[H] = {

"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                                                                                    0",
"0                                                                                    C                                                               0",
"0                   C                                  C                   C                                                                         0",
"0                                      C                                       kk                                                                    0",
"0                                                                             k  k    k    k                                                         0",
"0                      C                                                      k      kkk  kkk  w                                                     0",
"0                                                                       P     k       k    k                                                         0",
"0                                                                      PP     k  k                                                                   0",
"0                                                                     PPP      kk                                                                    0",
"0                    PPPPP                                           PPPP                                                                            0",
"0                                      T0                           PPPPP                                                                            0",
"0G                                     00              T0          PPPPPP                               G                                            0",
"0           P    P       P             00              00         PPPPPPP                                                                            0",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",

};


class Player{
public:

float dx,dy;
FloatRect rect;
bool onGround;
Sprite sprite;
float currentFrame;

	Player(Texture &image)
	{
		sprite.setTexture(image);
		rect = FloatRect(100,64,55,57);

		dx=dy=0;
		currentFrame=0;
	}

	void update(float time)
	{
		rect.left +=dx*time;
		Collision(0);
		if (!onGround) dy=dy+0.0005*time;
		rect.top +=dy*time;
		onGround=false;
		Collision(1);
		currentFrame += 0.005*time;
		if (currentFrame > 7) currentFrame -=7;
		
		if (dx>0) sprite.setTextureRect(IntRect(55*int(currentFrame),0,55,57));
		if (dx<0) sprite.setTextureRect(IntRect(55*int(currentFrame)+55,0,-55,57));

		sprite.setPosition(rect.left - offsetX,rect.top - offsetY);
		dx=0;
	}


	void Collision(int dir)
	{
		for (int i=rect.top/16; i<(rect.top+rect.height)/16; i++)
			for (int j=rect.left/16; j<(rect.left+rect.width)/16;j++)
			{
				if ((TileMap[i][j]=='P') || (TileMap[i][j]=='k') || (TileMap[i][j]=='0') || (TileMap[i][j]=='r') || (TileMap[i][j]=='T'))
				{
					if ((dx>0) && (dir==0)) rect.left = j*16-rect.width;
					if ((dx<0) && (dir==0)) rect.left = j*16+16;
					if ((dy>0) && (dir==1)) 
						{
							rect.top = i*16-rect.height;
							dy=0;
							onGround=true;
						}
					if ((dy<0) && (dir==1))
						{
							rect.top = i*16+16;
							dy=0;
						}
				}

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
	rect = FloatRect(x,y,16,16);

    dx=0.05;
	currentFrame = 0;
	life=true;
   }

   void update(float time)
   {	
	 rect.left += dx * time;
	
     Collision();
  

     currentFrame += time * 0.005;
     if (currentFrame > 2) currentFrame -= 2;

    sprite.setTextureRect(IntRect(18*int(currentFrame),0,16,16));
    if (!life) sprite.setTextureRect(IntRect(58,0,16,16));


	sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
	  
   }


   void Collision()
  {
	for (int i = rect.top/16 ; i<(rect.top+rect.height)/16; i++)
		for (int j = rect.left/16; j<(rect.left+rect.width)/16; j++)
			 if ((TileMap[i][j]=='P') || (TileMap[i][j]=='0'))
				{ 
                  if (dx>0)
				   { 
						rect.left =  j*16 - rect.width; dx*=-1; 
				   }
					else if (dx<0)
					{ 
						rect.left =  j*16 + 16;  dx*=-1; 
					}
											
				}
  }

};

int main()
{
	RenderWindow window( VideoMode(600,400),"Teemario");

	Texture t;
	t.loadFromFile("teeto.png");
	Texture env;
	env.loadFromFile("Mario_tileset.png");
	Sprite s(env);
	Texture background;
	background.loadFromFile("ar.png");
	Sprite sq(background);

	Font font;
	font.loadFromFile("1234.otf");
	Text mytext("You killed him!",font,50);
	mytext.setColor(Color::Red);
	mytext.setPosition(10,10);


	

	float currentFrame=0;

	Player p(t);
	Enemy enemy;
	enemy.set(env,48*16,21*16);

	Clock clock;

	p.sprite.setTextureRect(IntRect(0,0,55,57));

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/800;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			p.dx = -0.1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			p.dx = 0.1;

			
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			if (p.onGround)
			{
				p.dy=-0.4;
				p.onGround=false;
			}
		}
		p.update(time);
		enemy.update(time);

		if  (p.rect.intersects( enemy.rect ) )
		 {
			 if (enemy.life) {
			 if (p.dy>0) { enemy.dx=0; p.dy=-0.2; enemy.life=false; }
			 else p.sprite.setColor(Color::Red);
			 }
		 }

		if (p.rect.left>300) offsetX = p.rect.left - 300;
		//if (p.rect.top>200) offsetY = p.rect.top - 200;

		window.clear();
		window.draw(sq);
		if (!enemy.life) {window.draw(mytext);}


		for (int i=0; i<H; i++)
			for (int j=0; j<W; j++)
				{
					if (TileMap[i][j]=='P') s.setTextureRect(IntRect(0,25,16,16));
					if (TileMap[i][j]=='0') continue;
					if (TileMap[i][j]==' ') continue;
					if (TileMap[i][j]=='C') s.setTextureRect(IntRect(27,23,64,32));
					if (TileMap[i][j]=='T') s.setTextureRect(IntRect(0,50,32,49));
					if (TileMap[i][j]=='k') s.setTextureRect(IntRect(0,0,16,16));

					s.setPosition(j*16-offsetX,i*16-offsetY);
					window.draw(s);
				}
		window.draw(p.sprite);
		window.draw(enemy.sprite);
		window.display();
	}

	return 0;
}