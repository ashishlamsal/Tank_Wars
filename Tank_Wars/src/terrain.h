#pragma once
#include<SFML/Graphics.hpp>
#include"modified_vertexarray/vertexarray.h"
#include"box2d/box2d.h"
#include"../Definitions.h"


#define PI 3.14159265



struct intersect_data {
	sf::Vector2u range;
	unsigned int pos;
	intersect_data(unsigned int i, unsigned int f, unsigned int po) :pos(po) {
		range.x = i;
		range.y = f;
	}
};



class terrain : public sf::Drawable
{
private:
	sf::Vector2u					window_size;
	sf::Texture						m_texture;
	sf::IntRect						m_textureRect;
	sf::Color						m_fillColor = sf::Color::Blue;
	std::vector<vertexarray>		fills;
	vertexarray						surface;
	sf::FloatRect					m_bounds;
	b2ChainShape* chainshape;
	b2Body* chainbody;
	std::vector <b2Vec2> points;

	sf::Vector2f computeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2);

	float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2);

	sf::FloatRect getbound(std::vector<vertexarray>& fills, sf::Vector2u& window_size);

	unsigned int getlow(std::vector<unsigned int>& input, int max);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	void circleintersect(sf::Vector2f position, float radius, std::vector<intersect_data>& list);

	void recompute_surface();

public:

	bool need_update;
	bool need_update_b2d;
	bool already_drawn;

	int distance(sf::Vector2f first, sf::Vector2f second);

	terrain(std::vector<unsigned int>& value, sf::RenderWindow& window);

	bool circle_collides(sf::CircleShape& shape);

	void destroy(sf::Vector2f position, float radius);

	std::vector<intersect_data> check_pixels_on_air();

	void test(std::vector<intersect_data>& list);

	bool drop_air_pixel_by_1(std::vector<intersect_data>& list);

	void make();

	vertexarray get_surface();

	b2Body* getchainbody() { return chainbody; }

	void CreateGround(b2World& World);

	void recomputechain(b2World& world);

	b2Vec2 get_chain_point(b2Vec2 pos);


};

