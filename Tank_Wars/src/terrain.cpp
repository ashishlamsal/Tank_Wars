
#include "terrain.h"
#include<iostream>

int terrain::distance(sf::Vector2f first, sf::Vector2f second) {
	return sqrt(pow(first.y - second.y, 2) + pow(first.x - second.x, 2));
}

void terrain::draw(sf::RenderTarget& target, sf::RenderStates states) const {


	for (int i = 0; i < window_size.x; i++) {
		states.texture = &m_texture;
		target.draw(fills[i]);
	}
	target.draw(surface);

}

sf::FloatRect terrain::getbound(std::vector<vertexarray>& fills, sf::Vector2u& window_size) {
	sf::FloatRect ret(0, 0, window_size.x, 0);

	for (int i = 0; i < fills.size(); i++) {
		if (ret.top < fills[i].getVertexCount())
			ret.top = fills[i].getVertexCount();
	}
	ret.height = window_size.y - ret.top;
	return ret;
}

unsigned int terrain::getlow(std::vector<unsigned int>& input, int max) {

	unsigned int ret = max;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] < ret)
			ret = input[i];
	}
	return ret;
}

terrain::terrain(std::vector<unsigned int>& value, sf::RenderWindow& window) {

	window_size = window.getSize();
	window_size.y *= 0.8;
	fills.resize(window_size.x);
	surface.setPrimitiveType(sf::LineStrip);
	surface.resize(window_size.x);
	int lowest = getlow(value, window_size.y);
	for (int w = 0; w < window_size.x; w++) {
		fills[w].setPrimitiveType(sf::Points);
		fills[w].resize(window_size.y - value[w]);
		for (int h = 0; h < window_size.y - value[w]; h++) {
			fills[w][h].position = sf::Vector2f(w, window_size.y - h);
			fills[w][h].color = sf::Color(
			39.f+(234.0-39.0)*h/(window_size.y - value[w]),
			34.f+(182.0-34.0)*h/(window_size.y - value[w]),
			28.f+(79.0-28.0)*h/(window_size.y - value[w]),
			255
			);
		}
		surface[w] = fills[w][window_size.y - value[w] - 1];
	}

	m_bounds = getbound(fills, window_size);

	need_update = false;

	already_drawn = false;

}

//bool terrain::rectangle_collision(sf::RectangleShape &shape)
//{
//	int start = 0;
//	int end = window_size.x;
//	sf::Vector2f position = shape.getPosition();
//	int diagonal = sqrtf(shape.getSize().x * shape.getSize().x + shape.getSize().y * shape.getSize().y);
//
//	if ((position.x - diagonal) > 0)
//		start = position.x - diagonal;
//	
//	if ((position.x + diagonal) <= window_size.x)
//		end = position.x + diagonal;
//	sf::FloatRect rect;
//
//	for (int i = start; i < end; i++) {
//		//shape.
//	}
//
//	return false;
//}

bool terrain::circle_collides(sf::CircleShape& shape)
{
	float radius = shape.getRadius();
	sf::Vector2f position = shape.getPosition();
	int start = 0;
	int end = window_size.x;
	if ((position.x - radius) >= 0)
		start = position.x - radius;
	if ((position.x + radius) <= window_size.x)
		end = position.x + radius;


	/*std::cout << "value of position " << position.x << std::endl;
	std::cout << "value of start " << start << std::endl;
	std::cout << "value of end " << end << std::endl;*/


	//shape->setOrigin(radius, radius);

	for (int i = 0; i < 3; i++) {
		sf::Vertex b(position - sf::Vector2f(radius, radius) + shape.getPoint(i));
		for (int j = start; j < end + 1; j++) {
			if (surface[j].position.y <= b.position.y)
				return true;
		}
	}

	return false;
}


void terrain::circleintersect(sf::Vector2f position, float radius, std::vector<intersect_data>& list)
{
	for (int x = position.x < radius ? 0 : position.x - radius; x < ((position.x + radius) < window_size.x ? position.x + radius : window_size.x - 1); x++) {
		int pos = 0;
		bool is_pos_set = false;
		int start = 0;
		bool is_start_set = false;
		int i = 0;

		for (int y = 0; y < fills[x].getVertexCount(); y++) {
			if (distance(fills[x][y].position, position) < radius) {
				if (!is_pos_set) {
					pos = x;
					is_pos_set = true;
				}
				if (!is_start_set) {
					start = y;
					is_start_set = true;
				}
				i++;
			}
		}
		if (is_start_set && is_pos_set)
			list.push_back(intersect_data(start, start + i, pos));
	}
}

void  terrain::recompute_surface() {
	for (int w = 0; w < window_size.x; w++) {
		if (fills[w].getVertexCount() > 0)
			surface[w] = fills[w][fills[w].getVertexCount() - 1];
		else
			surface[w] = sf::Vertex(sf::Vector2f(w, window_size.y - 1));
	}
}

void terrain::destroy(sf::Vector2f position, float radius)   //index must have its origin set to center of circle
{
	int a;
	std::vector<intersect_data> list;
	circleintersect(position, radius, list);
	for (int i = 0; i < list.size(); i++) {
		fills[list[i].pos].erase(list[i].range.x, list[i].range.y);
		//recompute_surface();
		//std::cout << "value of pos " << list[i].pos << " deleted from " << list[i].range.x << " -- " << list[i].range.y << std::endl;
	}


}

std::vector<intersect_data> terrain::check_pixels_on_air() {
	std::vector<intersect_data> list;
	for (int w = 0; w < window_size.x; w++) {
		bool set = false;
		int start = 0, end = 0;
		int a = fills[w].getVertexCount();
		for (int h = 0; h < a; h++) {
			if (fills[w][h].position.y != (window_size.y - h)) {
				set = true;
				start = h;
				end = fills[w][h].position.y;

				break;
			}
		}
		if (set) {
			list.push_back(intersect_data(start, end, w));

		}
	}
	return list;
}

bool alldone(std::vector<bool>& test) {
	for (int i = 0; i < test.size(); i++) {
		if (test[i] = false);
		return false;
	}
	return true;
}

void terrain::test(std::vector<intersect_data>& list) {
	for (int i = 0; i < list.size(); i++) {
		for (int j = list[i].range.x; j < fills[list[i].pos].getVertexCount(); j++) {
			fills[list[i].pos][j].color = sf::Color::Green;
		}
	}
}

bool terrain::drop_air_pixel_by_1(std::vector<intersect_data>& list) {

	for (int i = 0; i < list.size(); i++) {
		int w = list[i].pos;

		for (int j = list[i].range.x; j < fills[w].getVertexCount(); j++) {
			fills[w][j].position.y += 1;

		}
	}
	return true;
}

void terrain::make() {

	if (need_update) {
		auto a = check_pixels_on_air();
		if (a.empty()) {
			need_update = false;
		}
		drop_air_pixel_by_1(a);
	}
	recompute_surface();
}

vertexarray terrain::get_surface()
{
	return surface;
}

void terrain::CreateGround(b2World& World)
{
	//entity type = entity::Terrain;

	chainshape = new b2ChainShape;

	points.resize(window_size.x);
	for (int i = 0; i < window_size.x; i++) {

		points[i] = b2Vec2(i / SCALE, (surface[i].position.y) / SCALE);

	}

	chainshape->CreateChain(&points[0], points.size());

	b2BodyDef BodyDef;
	chainbody = World.CreateBody(&BodyDef);
	chainbody->SetUserData(this->chainbody);
	b2FixtureDef def;
	def.density = 1;
	def.friction = 1;
	def.shape = chainshape;
	chainbody->CreateFixture(&def);

	need_update_b2d = false;
}

void terrain::recomputechain(b2World& world) {
	for (b2Body* BodyIterator = world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext()) {
		if (static_cast<b2Body*>(BodyIterator->GetUserData()) == chainbody) {
			b2Fixture* fixure = BodyIterator->GetFixtureList();
			BodyIterator->DestroyFixture(fixure);
			delete chainshape;
			chainshape = new b2ChainShape;
			for (int i = 0; i < window_size.x; i++)
				points[i] = b2Vec2(i / SCALE, (surface[i].position.y) / SCALE);

			chainshape->CreateChain(&points[0], points.size());
			b2FixtureDef new_fix;
			new_fix.density = 1;
			new_fix.friction = 10;
			new_fix.shape = chainshape;
			BodyIterator->CreateFixture(&new_fix);
			need_update_b2d = false;
		}
	}
}

b2Vec2 terrain::get_chain_point(b2Vec2 pos) {
	b2EdgeShape temp1, temp2;
	chainshape->GetChildEdge(&temp1, static_cast<int>(pos.x * SCALE) + 50);
	chainshape->GetChildEdge(&temp2, static_cast<int>(pos.x * SCALE) - 50);
	return b2Vec2(temp1.m_vertex1 - temp2.m_vertex2);
}



