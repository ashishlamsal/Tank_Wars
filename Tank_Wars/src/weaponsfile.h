#pragma once
#include"SFML/Graphics.hpp"
#include"projectile.h"
#include"terrain.h"
#include"collision.h"
#include "../Definitions.h"

class shot : public projectile {
	//friend class player;
private:
	//bool done = false;
	std::vector<particle> m_particle;
	float blast_radius;
public:
	bool is_powerful;	

	std::vector<sf::Vector2f> getpos() {
		std::vector<sf::Vector2f> pos;
		for (int i = 0; i < m_particle.size(); i++) {
			pos.push_back(m_particle[i].shape.getPosition());
		}
		return pos;
	}

	void init(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f accln, float blast_r,float dmg, int no=1) {
		damage = dmg;
		blast_radius = blast_r;
		m_particle.resize(no);
		int a = no/2;
		float j = 1 - 0.1 * a;
	

		for (int i = 0; i < no; i++) {
			m_particle[i].m_vel = vel * j;
			m_particle[i].m_accln = accln;
			m_particle[i].shape.setRadius(3);
			m_particle[i].shape.setOrigin(m_particle[i].shape.getRadius(), m_particle[i].shape.getRadius());
			m_particle[i].shape.setPosition(pos);
			m_particle[i].shape.setFillColor(sf::Color(255,229,153,255));
			m_particle[i].in_bound = true;
			m_particle[i].draw = true;
			j = j + 0.1;
		}
		initialized = true;
		in_bound = true;
	}

	void draw(sf::RenderWindow& window)
	{
		//window.draw(*m_particle[0].shape);
		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {
			
				//std::cout << "proj drawn" << std::endl;
				window.draw(m_particle[i].shape);
				
			}
		}
	}

	void make(float dt, float multiplier, terrain& t, Tank& tank, Tank& tank1)
	{
		float time = dt * multiplier;
		//std::cout << "make is being called" << std::endl;
		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {
				//update the position of projectile
				m_particle[i].m_vel = m_particle[i].m_vel + m_particle[i].m_accln * time;
				m_particle[i].shape.move(m_particle[i].m_vel.x * time, m_particle[i].m_vel.y * time);
				//std::cout << "position of projectile is " << m_particle[i].shape.getPosition().x << "," << m_particle[i].shape.getPosition().y << std::endl;
				//std::cout << "make called" << std::endl;
				//
				//


				b2Vec2 projectile_posb2d(m_particle[i].shape.getPosition().x / SCALE, m_particle[i].shape.getPosition().y / SCALE);
				sf::Vector2f projectile_pos = m_particle[i].shape.getPosition();

				//check for direct collision between chasis and projectile
				if (tank.get_chasis()->GetFixtureList()->TestPoint(projectile_posb2d)) {
					//std::cout << "hit oneself" << std::endl;
					float* score = tank.get_score();
					float* health = tank.get_health();
					*score = *score - damage;
					*health -= damage;
					std::cout << "health of selected player is " << *health << std::endl;
					std::cout << "score of selected player is " << *score << std::endl;

					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}

				if (tank1.get_chasis()->GetFixtureList()->TestPoint(projectile_posb2d)) {
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					float* score = tank.get_score();
					float* health = tank1.get_health();
					*score = *score + damage;
					*health -= damage;
					std::cout << "hit other tank damage done:"<< damage << std::endl;
					std::cout << "health of other player is " << *health << std::endl;

					if (!is_powerful)
						m_particle[i].draw = false;
				}
				//
				//

				if (t.circle_collides(m_particle[i].shape)) {

					t.destroy(m_particle[i].shape.getPosition(), blast_radius);


					sf::Transform trans = tank.sfml_tank->chasis.getTransform();
					sf::Transform trans1 = tank1.sfml_tank->chasis.getTransform();

					std::vector<sf::Vector2f> vertices,vertices1;
					vertices.reserve(tank.sfml_tank->chasis.getPointCount());
					vertices1.reserve(tank1.sfml_tank->chasis.getPointCount());

					for (int i = 0; i < tank.sfml_tank->chasis.getPointCount(); i++) {
						vertices.push_back(tank.sfml_tank->chasis.getPoint(i));
						vertices[i] = trans.transformPoint(vertices[i]);
						vertices1.push_back(tank1.sfml_tank->chasis.getPoint(i));
						vertices1[i] = trans1.transformPoint(vertices1[i]);
					}
					bool p1_chasis_collision = polyCircle(vertices, projectile_pos.x, projectile_pos.y, blast_radius);
					bool p1_right_wheel_collision = circleCircle(projectile_pos, blast_radius, tank.sfml_tank->right_wheel.getPosition(), tank.sfml_tank->right_wheel.getRadius());
					bool p1_left_wheel_collision = circleCircle(projectile_pos, blast_radius, tank.sfml_tank->left_wheel.getPosition(), tank.sfml_tank->left_wheel.getRadius());

					bool p2_chasis_collision = polyCircle(vertices1, projectile_pos.x, projectile_pos.y, blast_radius);
					bool p2_right_wheel_collision = circleCircle(projectile_pos, blast_radius, tank1.sfml_tank->right_wheel.getPosition(), tank1.sfml_tank->right_wheel.getRadius());
					bool p2_left_wheel_collision = circleCircle(projectile_pos, blast_radius, tank1.sfml_tank->left_wheel.getPosition(), tank1.sfml_tank->left_wheel.getRadius());
					
					if (p1_chasis_collision || p1_right_wheel_collision || p1_left_wheel_collision) {

						float dis = distance(projectile_pos, tank.sfml_tank->chasis.getPosition());
						float dmg = damage / (10 - blast_radius - c_width / 2) * (-blast_radius-c_width/2 + dis);
						float *score = tank.get_score();
						float* health = tank.get_health();
						*score = *score - dmg;
						*health -= dmg;
						std::cout << "health of selected player is " << *health<<std::endl;
						std::cout << "score of selected player is " << *score<<std::endl;

					}

					if (p2_chasis_collision || p2_right_wheel_collision || p2_left_wheel_collision) {

						float dis = distance(projectile_pos, tank1.sfml_tank->chasis.getPosition());
						float dmg = damage / (5 - blast_radius - c_width / 2) * (-blast_radius - c_width / 2 + dis);
						float* score = tank.get_score();
						float* health = tank1.get_health();
						*score = *score + dmg;	
						*health -= dmg;
						std::cout << "hit other at periphery distance is "<<dis<< " damage done is" << dmg << std::endl;
						std::cout << "health of other player is " << *health << std::endl;
						std::cout << "score of selected player is " << *score << std::endl;
					}

					if(!is_powerful)
						m_particle[i].draw=false;
				}

				/*if (polygonPoint(vertices, projectile_pos.x, projectile_pos.y)) {
					std::cout << "hit oneself" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}

				if (polygonPoint(vertices1, projectile_pos.x, projectile_pos.y)) {
					std::cout << "hit other tank" << std::endl;
					t.destroy(m_particle[i].shape.getPosition(), blast_radius);

					if (!is_powerful)
						m_particle[i].draw = false;
				}*/						
				
				if (m_particle[i].draw) {
					if (!(sf::IntRect(0, -1000, DEFAULT_WIDTH, 1768).contains((sf::Vector2i)m_particle[i].shape.getPosition()))) {
						m_particle[i].draw = false;
						//std::cout << "projectile" << i << "is out of bound" << std::endl;
					}
				}
			}			
		}

		done = true;
		
		for (int i = 0; i < m_particle.size(); i++)
			if (m_particle[i].draw)
				done = false;
		

	}

	~shot() {
		initialized = false;
		
	}	
};


class scatter :public projectile {
private:
	std::vector<particle> m_particle;
	float blast_radius;
public:
	std::vector<sf::Vector2f> getpos() {
		std::vector<sf::Vector2f> pos;
		for (int i = 0; i < m_particle.size(); i++) {
			pos.push_back(m_particle[i].shape.getPosition());
		}
		return pos;
	}

	void init(sf::Vector2f pos, sf::Vector2f vel, sf::Vector2f accln, float blast_r, float dmg, int no=1) {
		damage = dmg;
		blast_radius = blast_r;
		//m_particle.resize(5);
		std::cout << "scatter initialized" << std::endl;
		particle temp;
		temp.m_vel = vel;
		temp.m_accln = accln;
		temp.shape.setRadius(2);
		temp.shape.setOrigin(temp.shape.getRadius(), temp.shape.getRadius());
		temp.shape.setPosition(pos);
		temp.shape.setFillColor(sf::Color::Red);
		temp.in_bound = true;
		temp.draw = true;
		m_particle.push_back(temp);

		initialized = true;
		in_bound = true;

	}

	void make(float dt, float multiplier, terrain& t, Tank& tank, Tank& tank1) {

		float time = dt * multiplier;

		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {

				m_particle[i].m_vel = m_particle[i].m_vel + m_particle[i].m_accln * time;
				m_particle[i].shape.move(m_particle[i].m_vel.x * time, m_particle[i].m_vel.y * time);


				b2Vec2 projectile_posb2d(m_particle[i].shape.getPosition().x / SCALE, m_particle[i].shape.getPosition().y / SCALE);
				sf::Vector2f projectile_pos = m_particle[i].shape.getPosition();


				if (t.circle_collides(m_particle[i].shape)) {
					//m_particle.clear();
					if (m_particle.size() == 1) {
						sf::Vector2f temp_acc = m_particle[0].m_accln;
						m_particle.resize(5);
						for (int i = 0; i < m_particle.size(); i++) {
							m_particle[i].m_vel = -35.f * sf::Vector2f(cos(PI / 12 * (4.f + i)), sin(PI / 12 * (4.f + i)));
							m_particle[i].m_accln = temp_acc;
							m_particle[i].shape.setRadius(2);
							m_particle[i].shape.setOrigin(m_particle[0].shape.getRadius(), m_particle[0].shape.getRadius());
							m_particle[i].shape.setPosition(projectile_pos - sf::Vector2f(0, 10));
							m_particle[i].shape.setFillColor(sf::Color(255 - 255 / 4 * i, 255 / 4 * i, 0, 255));
							m_particle[i].in_bound = true;
							m_particle[i].draw = true;
						}
					}
					else
					{
						t.destroy(m_particle[i].shape.getPosition(), blast_radius);


						sf::Transform trans = tank.sfml_tank->chasis.getTransform();
						sf::Transform trans1 = tank1.sfml_tank->chasis.getTransform();

						std::vector<sf::Vector2f> vertices, vertices1;
						vertices.reserve(tank.sfml_tank->chasis.getPointCount());
						vertices1.reserve(tank1.sfml_tank->chasis.getPointCount());

						for (int i = 0; i < tank.sfml_tank->chasis.getPointCount(); i++) {
							vertices.push_back(tank.sfml_tank->chasis.getPoint(i));
							vertices[i] = trans.transformPoint(vertices[i]);
							vertices1.push_back(tank1.sfml_tank->chasis.getPoint(i));
							vertices1[i] = trans1.transformPoint(vertices1[i]);
						}
						bool p1_chasis_collision = polyCircle(vertices, projectile_pos.x, projectile_pos.y, blast_radius);
						bool p1_right_wheel_collision = circleCircle(projectile_pos, blast_radius, tank.sfml_tank->right_wheel.getPosition(), tank.sfml_tank->right_wheel.getRadius());
						bool p1_left_wheel_collision = circleCircle(projectile_pos, blast_radius, tank.sfml_tank->left_wheel.getPosition(), tank.sfml_tank->left_wheel.getRadius());

						bool p2_chasis_collision = polyCircle(vertices1, projectile_pos.x, projectile_pos.y, blast_radius);
						bool p2_right_wheel_collision = circleCircle(projectile_pos, blast_radius, tank1.sfml_tank->right_wheel.getPosition(), tank1.sfml_tank->right_wheel.getRadius());
						bool p2_left_wheel_collision = circleCircle(projectile_pos, blast_radius, tank1.sfml_tank->left_wheel.getPosition(), tank1.sfml_tank->left_wheel.getRadius());

						if (p1_chasis_collision || p1_right_wheel_collision || p1_left_wheel_collision) {

							float dis = distance(projectile_pos, tank.sfml_tank->chasis.getPosition());
							float dmg = damage / (10 - blast_radius - c_width / 2) * (-blast_radius - c_width / 2 + dis);
							float* score = tank.get_score();
							float* health = tank.get_health();
							*score = *score - dmg;
							*health -= dmg;
							std::cout << "health of selected player is " << *health << std::endl;
							std::cout << "score of selected player is " << *score << std::endl;

						}

						if (p2_chasis_collision || p2_right_wheel_collision || p2_left_wheel_collision) {

							float dis = distance(projectile_pos, tank1.sfml_tank->chasis.getPosition());
							float dmg = damage / (5 - blast_radius - c_width / 2) * (-blast_radius - c_width / 2 + dis);
							float* score = tank.get_score();
							float* health = tank1.get_health();
							*score = *score + dmg;
							*health -= dmg;
							std::cout << "hit other at periphery distance is " << dis << " damage done is" << dmg << std::endl;
							std::cout << "health of other player is " << *health << std::endl;
							std::cout << "score of selected player is " << *score << std::endl;
						}

						m_particle[i].draw = false;

					}					
				}

				if (m_particle.size() == 5) {
					//check for direct collision between chasis and projectile
					if (tank.get_chasis()->GetFixtureList()->TestPoint(projectile_posb2d)) {
						//std::cout << "hit oneself" << std::endl;
						float* score = tank.get_score();
						float* health = tank.get_health();
						*score = *score - damage;
						*health -= damage;
						std::cout << "health of selected player is " << *health << std::endl;
						std::cout << "score of selected player is " << *score << std::endl;

						t.destroy(m_particle[i].shape.getPosition(), blast_radius);	

						m_particle[i].draw = false;
					}

					if (tank1.get_chasis()->GetFixtureList()->TestPoint(projectile_posb2d)) {
						t.destroy(m_particle[i].shape.getPosition(), blast_radius);

						float* score = tank.get_score();
						float* health = tank1.get_health();
						*score = *score + damage;
						*health -= damage;
						std::cout << "hit other tank damage done:" << damage << std::endl;
						std::cout << "health of other player is " << *health << std::endl;

						m_particle[i].draw = false;
					}
					//
					//
				}
				else {
					m_particle[0].shape.setFillColor(sf::Color::Green);
				}
			
			}
		}

		done = true;

		for (int i = 0; i < m_particle.size(); i++)
			if (m_particle[i].draw)
				done = false;
	}

	void draw(sf::RenderWindow& window)
	{
		//window.draw(*m_particle[0].shape);
		for (int i = 0; i < m_particle.size(); i++) {
			if (m_particle[i].draw) {

				//std::cout << "scatter drawn" << std::endl;

				window.draw(m_particle[i].shape);

			}
		}
	}

	~scatter() {
		initialized = false;

	}

};