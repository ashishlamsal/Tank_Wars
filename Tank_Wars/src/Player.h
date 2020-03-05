#pragma once
#include<iostream>

#include"SFML/Graphics.hpp"
#include"box2d/box2d.h"
#include"Tank.h"
#include"weaponsfile.h"
#include"../Definitions.h"
#include"../ResourceManager.h"
enum class Player_state { idle, fired, move_right, move_left, away };

class arrow {
	sf::Texture arrow_t;
	sf::Sprite arrow_s;
	sf::Vector2f vel;
	float time_in_secs;
public:
	void setup(textureManager& textures) {
		arrow_t=textures.get(textureIdentifier::arrow);
		arrow_s.setTexture(arrow_t);
		arrow_s.setOrigin(arrow_s.getTextureRect().width / 2, arrow_s.getTextureRect().height / 2);
		arrow_s.scale(0.5, 0.5);
		time_in_secs = 0;
	}
	void make(float dt) {
		time_in_secs += dt * 10.f;
	}


	void draw(sf::RenderWindow& window, sftank& tank) {
		sf::Transform trans = tank.chasis.getTransform();
		arrow_s.setPosition(tank.chasis.getPosition().x, tank.chasis.getPosition().y - 60 + 15 * sin(time_in_secs));
		window.draw(arrow_s);
	}
};


class Player
{
private:
	projectile* m_shot;
	float power;	
	arrow indicator;
public:
	Tank tank;
	std::vector<Weapon> armory;
	Weapon* selected_weapon;
	Player_state state;

	Player(b2World& world, float x, float y, bool flip,textureManager &textures);

	void set_power(float p) { power = p; }
	float get_power() { return power; }
	void make(float dt, float mul, terrain& t, Tank& tank2);
	void fire();
	void draw(sf::RenderWindow& window);
	void draw_arrow(sf::RenderWindow& window);
	~Player();
};

