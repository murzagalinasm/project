#include "objects.h"
#include "map.h"
#include "view.h"//подключили код с видом камеры

Object::Object(const std::string name_file,
       const float obj_size_x,
       const float obj_size_y,
       int pos_x, int pos_y, sf::Texture texture_ = sf::Texture(),
           sf::Sprite sprite_ = sf::Sprite()) : 
        velocity_obj(0, 0),
        acceleration_obj(0, 0),
        file_load_name(name_file),
        size_obj(obj_size_x, obj_size_y),
        pos_obj(pos_x, pos_y),
        texture(texture_),
        obj_sprite(sprite_)
        {
            image.loadFromFile(file_load_name);
            image.createMaskFromColor(sf::Color::Black);
            texture.loadFromImage(image);
            obj_sprite.setTexture(texture);
            obj_sprite.setTextureRect(sf::IntRect(0, 0, obj_size_x, obj_size_y));
            obj_sprite.setPosition(pos_obj.x, pos_obj.y);
        }

Hero::Hero(const std::string name_file,
            const float obj_size_x,
            const float obj_size_y,
            int pos_x, int pos_y) :

        Object(name_file, obj_size_x, obj_size_y, pos_x, pos_y),
        currentFrame(0.0),
        previous_direction(RIGHT),
        ON_GROUND(true)
        {}

void Hero::motion()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        this->velocity_obj.x = -0.1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        this->velocity_obj.x = 0.1;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        if (this->ON_GROUND)
        {
            this->velocity_obj.y = -0.35;
            this->ON_GROUND = false;
            //hero.current_direction = Hero::JUMP_UP;
        }
    }
}

void Hero::draw(sf::RenderWindow &window)
{

	if (this->current_direction == RIGHT)
	{
		this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame), 0, 551, 509));
	}
	if (this->current_direction == LEFT)
		this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame) + 551, 0, -551, 509));

	if (this->current_direction == JUMP_UP)
	{
		if (this->previous_direction == RIGHT)
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame), 509, 551, 530));
		else
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame) + 551, 509, -551, 530));
	}

	if (this->current_direction == JUMP_DOWN)
	{
		if (this->previous_direction == RIGHT)
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame), 509 + 530, 551, 502));
		else
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame) + 551, 509 + 530, -551, 502));
	}

	if (this->current_direction == STAY)
	{
		if (this->previous_direction == RIGHT)
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame), 509 + 530 + 502, 551, 509));
		else
			this->obj_sprite.setTextureRect(sf::IntRect(551 * int(currentFrame) + 551, 509 + 530 + 502, -551, 509));
	}
	//	getplayercoordinateforview(getplayercoordinateX(), getplayercoordinateY());//передаем координаты игрока в функцию управления камерой

	window.draw(this->obj_sprite);
}

void Hero::update(float time)
{

	this->pos_obj.x += this->velocity_obj.x * time;

	if (!this->ON_GROUND)
		this->velocity_obj.y = this->velocity_obj.y + 0.0005 * time;

	this->pos_obj.y += this->velocity_obj.y * time;
	this->ON_GROUND = false;

	if (this->pos_obj.y > GROUND$)
	{
		this->pos_obj.y = GROUND$;
		this->velocity_obj.y = 0;
		this->ON_GROUND = true;
	}

	currentFrame += 0.005 * time;

	if (currentFrame > 4)  //fix this + spritesheet @TODO 
		currentFrame -= 4;

	this->obj_sprite.setPosition(this->pos_obj.x, this->pos_obj.y);

	this->previous_direction_2 = this->current_direction;

	if (this->current_direction == LEFT || this->current_direction == RIGHT)
	{
		this->previous_direction = this->current_direction;
	}

	if (this->velocity_obj.y < 0)
	{
		this->current_direction = JUMP_UP;
	}
	else
	{
		if (this->velocity_obj.y > 0)
		{
			this->current_direction = JUMP_DOWN;
		}
		else
		{
			if (this->velocity_obj.x > 0)
			{
				this->current_direction = RIGHT;
			}
			else
			{
				if (this->velocity_obj.x < 0)
				{
					this->current_direction = LEFT;
				}
				else
				{
					this->current_direction = STAY;
				}
			}
		}
	}

	if (this->previous_direction_2 != this->current_direction)
	{
		currentFrame = 0;
	}

	this->velocity_obj.x = 0;
}

//void Hero::interactionwithmap()
//{

//}

/*
   Game::Game(sf::Texture texture_ = sf::Texture(),
   sf::Sprite sprite_ = sf::Sprite(), sf::Event event_ = sf::Event(),
   sf::Clock clock_ = sf::Clock()):
   window(sf::VideoMode(1280, 720), "GO"),
   time_game(0.0),
   Background(texture_),
   sprite_background(sprite_),
   event_game(event_),
   clock_game(clock_)
   {
   Background.loadFromFile("maxresdefault.jpg");
   sprite_background.setTexture(Background);
   }
   */
Game::Game() : 
	window(sf::VideoMode(1280, 720), "GO"),
	//	view.reset(sf::FloatRect(0, 0, 1280, 720)),//размер "вида" камеры при создании объекта вида камеры
	time_game(0.0),
	Background(sf::Texture()),
	sprite_background(sf::Sprite()),
	Map(sf::Texture()),
	sprite_map(sf::Sprite()),
	event_game(sf::Event()),
	clock_game(sf::Clock())
{
	Background.loadFromFile("maxresdefault.jpg");
	sprite_background.setTexture(Background);

	Map.loadFromFile("sheet.png");
	sprite_map.setTexture(Map);
}
//void Game::getplayercoordinateforview(float x,float y) { //функция для считывания координат игрока


//	view.setCenter(x + 100, y); //следим за игроком, передавая его координаты камере. +100 - сместили камеру по иксу вправо. эксперементируйте


//}
void Game::run()
{
	Hero hero("Corgi_2.png", 551.0, 509.0, 0, GROUND$);

	while(window.isOpen())
	{
		this->time_game = clock_game.getElapsedTime().asMicroseconds();
		this->clock_game.restart();

		this->time_game = this->time_game / 1000;

		if (this->time_game > 20)
			this->time_game = 20;

		view.setCenter(pos_obj.x,pos_obj.y);
		window.setView(view);//"оживляем" камеру в окне sfml

		window.clear();

		window.draw(sprite_background);

		// Рисуем карту
		for(int i = 0; i< HEIGHT_MAP; i++)
			for(int j = 0; j< WIDTH_MAP; j++)
			{
				if(TileMap[i][j] ==' ') sprite_map.setTextureRect(IntRect(350,70,70,70));
				if(TileMap[i][j] =='p') sprite_map.setTextureRect(IntRect(0,0,70,70));
				if(TileMap[i][j] =='k') sprite_map.setTextureRect(IntRect(210,0,70,70));
				if(TileMap[i][j] =='l') sprite_map.setTextureRect(IntRect(280,0,70,70));
				if(TileMap[i][j] =='d') sprite_map.setTextureRect(IntRect(350,0,70,70));
				if(TileMap[i][j] =='o') sprite_map.setTextureRect(IntRect(280,210,70,70));
				if(TileMap[i][j] =='u') sprite_map.setTextureRect(IntRect(140,280,70,70));
				if(TileMap[i][j] =='q') sprite_map.setTextureRect(IntRect(0,350,70,70));
				if(TileMap[i][j] =='n') sprite_map.setTextureRect(IntRect(140,350,70,70));
				if(TileMap[i][j] =='c') sprite_map.setTextureRect(IntRect(210,420,70,70));
				if(TileMap[i][j] =='e') sprite_map.setTextureRect(IntRect(210,210,70,70));
				if(TileMap[i][j] =='f') sprite_map.setTextureRect(IntRect(210,140,70,70));
				sprite_map.setPosition(j*70,i*70);
				window.draw(sprite_map);
			}
		window.pollEvent(event_game);

		if (event_game.type == sf::Event::Closed)
			window.close();
		// The escape key was pressed
		if ((event_game.type == sf::Event::KeyPressed) && (event_game.key.code == sf::Keyboard::Escape))
			window.close();



		hero.motion();

		hero.update(time_game);


		hero.draw(window);
		window.display();
	}
}
