#pragma once

#include<vector>
#include"SFML/Graphics.hpp"

inline float distance(sf::Vector2f a, sf::Vector2f b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

inline bool circleCircle(sf::Vector2f center1, float radius1, sf::Vector2f center2, float radius2) {
	return distance(center1,center2) < radius1 + radius2;
}

	// POINT/CIRCLE
	 inline bool pointCircle(float px, float py, float cx, float cy, float r) {

		float distance = sqrt(pow(cx - px, 2) + pow(cy - py, 2));

		if (distance <= r) {
			return true;
		}
		return false;
	}

	// LINE/POINT
	inline bool linePoint(float x1, float y1, float x2, float y2, float px, float py) {

		float d1 = sqrt(pow(x1 - px, 2) + pow(y1 - py, 2));

		float d2 = sqrt(pow(x2 - px, 2) + pow(y2 - py, 2));

		float lineLen = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

		float buffer = 0.01;

		if (d1 + d2 >= lineLen - buffer && d1 + d2 <= lineLen + buffer) {
			return true;
		}
		return false;
	}

	// LINE/CIRCLE
	inline bool lineCircle(float x1, float y1, float x2, float y2, float cx, float cy, float r) {


		bool inside1 = pointCircle(x1, y1, cx, cy, r);
		bool inside2 = pointCircle(x2, y2, cx, cy, r);
		if (inside1 || inside2) return true;

		float distX = x1 - x2;
		float distY = y1 - y2;
		float len = sqrt((distX * distX) + (distY * distY));

		float dot = (((cx - x1) * (x2 - x1)) + ((cy - y1) * (y2 - y1))) / pow(len, 2);

		float closestX = x1 + (dot * (x2 - x1));
		float closestY = y1 + (dot * (y2 - y1));

		bool onSegment = linePoint(x1, y1, x2, y2, closestX, closestY);
		if (!onSegment) return false;

		/*sf::CircleShape test(10);
		test.setOrigin(test.getRadius(), test.getRadius());
		test.setFillColor(sf::Color::Green);
		test.setPosition(closestX, closestY);
		window.draw(test);*/

		distX = closestX - cx;
		distY = closestY - cy;
		float distance = sqrt((distX * distX) + (distY * distY));

		if (distance <= r) {
			return true;
		}
		return false;
	}


	// POLYGON/POINT
	inline bool polygonPoint(std::vector<sf::Vector2f> vertices, float px, float py) {
		bool collision = false;


		int next = 0;
		for (int current = 0; current < vertices.size(); current++) {

			next = current + 1;
			if (next == vertices.size()) next = 0;

			sf::Vector2f vc = vertices[current];
			sf::Vector2f vn = vertices[next];

			if (((vc.y > py&& vn.y < py) || (vc.y < py && vn.y > py)) &&
				(px < (vn.x - vc.x) * (py - vc.y) / (vn.y - vc.y) + vc.x)) {
				collision = !collision;
			}
		}
		return collision;
	}

	// POLYGON/CIRCLE
	inline bool polyCircle(std::vector<sf::Vector2f> vertices, float cx, float cy, float r) {


		int next = 0;
		for (int current = 0; current < vertices.size(); current++) {
			next = current + 1;

			if (next == vertices.size())
				next = 0;

			sf::Vector2f vc = vertices[current];
			sf::Vector2f vn = vertices[next];

			bool collision = lineCircle(vc.x, vc.y, vn.x, vn.y, cx, cy, r);
			if (collision) return true;
		}

		bool centerInside = polygonPoint(vertices, cx, cy);
		if (centerInside) return true;

		return false;
	}
