#pragma once

#include<array>
#include"SFML/Graphics.hpp"
#include"box2d/box2d.h"
#include"terrain.h"
#include"../Definitions.h"
#include"../ResourceManager.h"

static const float c_width = 65;
static const float c_height = 25;
static const unsigned fall_limit = 5;
static const float motorspeed = 10;

struct sftank {

	sf::ConvexShape chasis;
	sf::ConvexShape top;
	sf::CircleShape left_wheel;
	sf::CircleShape right_wheel;
	sf::RectangleShape barrel;

	float barrel_angle;
	sf::Vector2f muzzle;

	void change_angle(float value) {
		barrel_angle = value * b2_pi / 180;
		barrel.setRotation(value);
		muzzle = barrel.getPosition() + barrel.getSize().x * sf::Vector2f(cos(barrel_angle), sin(barrel_angle));
	}

	void point_to(sf::Vector2f pos) {
		//std::cout << "point to from p1" << std::endl;
		double  angle = 180 / 3.14159565 * atan2(pos.y - chasis.getPosition().y, pos.x - chasis.getPosition().x);
		change_angle(angle);
	}

};



class Tank
{
private:
	b2World* c_world;
	b2Body* chasis, * leftwheel, * rightwheel, * top;
	b2FixtureDef m_chasisdef, m_wheeldef, m_topdef;
	std::vector<b2Vec2> points;
	float max_normal_force;
	b2WheelJoint* leftaxis, * rightaxis;
	float score;
	float health;
	unsigned frames_in_air;


	sf::Texture body_texture;
	sf::Texture wheels_texture;
	sf::Texture top_texture;
	sf::Texture barrel_texture;

	std::vector<sf::Vector2f> top_points;
	void create_chasis(float x, float y, bool flip);
	void create_wheels(float x, float y);
public:
	b2Joint* left_joint, * right_joint, * chasis_joint;
	std::array<b2Vec2, 2> right_wheel_contact_point;
	std::array<b2Vec2, 2> left_wheel_contact_point;
	bool right_wheel_contact;
	bool left_wheel_contact;
	bool chasis_contact;
	sftank* sfml_tank;
	int test;

	

	Tank();
	Tank(b2World& world, b2FixtureDef& chasisdef, b2FixtureDef& wheeldef, float x, float y, bool flip, textureManager& textures);
	void setup(b2World& world, b2FixtureDef& chasisdef, b2FixtureDef& wheeldef, float x, float y, bool flip,textureManager& textures);
	void moveright(terrain& t);
	void moveleft(terrain& t);
	void makejoint(terrain& t);
	void stop(terrain& t);
	void applynormalforce(terrain& t);
	void respawn_Tank(bool flip);
	void fall_if_needed(terrain& t);
	b2Body* get_chasis() { return chasis; }
	b2Body* get_rightwheel() { return rightwheel; }
	b2Body* get_leftwheel() { return leftwheel; }
	b2Body* get_top() { return top; }
	b2World* get_world() { return c_world; }
	float* get_score() { return &score; }
	float* get_health() { return &health; }
	virtual ~Tank() {
		delete sfml_tank;
	}

	inline float getlow(float* input, int no) {

		float ret = 10000;
		for (int i = 0; i < no; i++) {
			if (input[i] < ret)
				ret = input[i];
		}
		return ret;
	}
	inline float gethigh(float* input, int no) {

		float ret = 0;
		for (int i = 0; i < no; i++) {
			if (input[i] > ret)
				ret = input[i];
		}
		return ret;
	}
	inline sf::FloatRect getaabb() {
		sf::FloatRect chasis_b = sfml_tank->chasis.getGlobalBounds();
		sf::FloatRect leftwheel_b = sfml_tank->left_wheel.getGlobalBounds();
		sf::FloatRect rightwheel_b = sfml_tank->right_wheel.getGlobalBounds();
		sf::FloatRect barrel_b = sfml_tank->barrel.getGlobalBounds();
		float left[4] = { chasis_b.left,leftwheel_b.left,rightwheel_b.left, barrel_b.left };
		float top[4] = { chasis_b.top,leftwheel_b.top,rightwheel_b.top, barrel_b.top };
		float width[4] = { chasis_b.left + chasis_b.width,leftwheel_b.left + leftwheel_b.width,rightwheel_b.left + rightwheel_b.width, barrel_b.left + barrel_b.width };
		float height[4] = { chasis_b.top + chasis_b.height,leftwheel_b.top + leftwheel_b.height,rightwheel_b.top + rightwheel_b.height, barrel_b.top + barrel_b.height };

		float re_left = getlow(left, 4);
		float re_top = getlow(top, 4);
		float re_width = gethigh(width, 4) - re_left;
		float re_height = gethigh(height, 4) - re_top;

		sf::FloatRect ret(re_left, re_top, re_width, re_height);
		return ret;

	}
};