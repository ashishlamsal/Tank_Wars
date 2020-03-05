#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include"terrain.h"

enum class Weapon { none, single_shot, triple_shot, penta_shot, total_destroy, scatter_shot };


struct particle {
	sf::CircleShape shape;
	sf::Vector2f m_vel;
	sf::Vector2f m_accln;
	bool in_bound;
	bool draw;
};


class projectile {

protected:
	
	bool initialized;
	bool in_bound;
	bool done=false;
	float damage;
public:

	bool is_initialized() { return initialized;	}
	bool is_in_bound() { return in_bound; }
	virtual std::vector<sf::Vector2f> getpos() = 0;

	projectile() : initialized(false),in_bound(false),damage(10) {}

	virtual void init(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f accln, float blast_r,float dmg, int no=1) = 0;

	virtual void draw(sf::RenderWindow& window) = 0;
	

	virtual void make(float dt, float multiplier, terrain& t, Tank& tank, Tank& tank1) = 0;
	bool is_done() { return done; }


};

