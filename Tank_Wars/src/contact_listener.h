#pragma once
#include "Box2d/Box2d.h"
#include "Tank.h"


class contactlistener :public b2ContactListener
{
	Tank* c1;
	Tank* c2;
	terrain* t;
	int l_counter1, r_counter1, l_counter2, r_counter2;

public:

	void setup(Tank& ca, Tank& cb, terrain& te) {
		c1 = &ca;
		c2 = &cb;
		t = &te;
		l_counter1 = 0;
		r_counter1 = 0;
		l_counter2 = 0;
		r_counter2 = 0;
	}


	void BeginContact(b2Contact* contact) {
		b2Body* type_a = static_cast<b2Body*>(contact->GetFixtureA()->GetBody()->GetUserData());
		b2Body* type_b = static_cast<b2Body*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (type_a == c1->get_chasis() && type_b == t->getchainbody()) {
			if (!c1->chasis_contact) {
				c1->chasis_contact = true;
			}
		}

		if (type_a == c2->get_chasis() && type_b == t->getchainbody()) {
			if (!c2->chasis_contact) {
				c2->chasis_contact = true;
			}
		}
		
		if (type_b == c1->get_rightwheel() && type_a == t->getchainbody()) {

			b2WorldManifold temp_manifold;
			contact->GetWorldManifold(&temp_manifold);
			c1->right_wheel_contact_point[0] = temp_manifold.points[0];
			c1->right_wheel_contact_point[1] = temp_manifold.points[1];

			if (!c1->right_wheel_contact) {
				c1->right_wheel_contact = true;
			
			}

		}

		if ((type_b == c1->get_leftwheel() && type_a == t->getchainbody())) {

			b2WorldManifold temp_manifold;
			contact->GetWorldManifold(&temp_manifold);
			c1->left_wheel_contact_point[0] = temp_manifold.points[0];
			c1->left_wheel_contact_point[1] = temp_manifold.points[1];


			if (!c1->left_wheel_contact) {
				c1->left_wheel_contact = true;
			}
		}

		if ((type_b == c2->get_rightwheel() && type_a == t->getchainbody())) {

			b2WorldManifold temp_manifold;
			contact->GetWorldManifold(&temp_manifold);
			c2->right_wheel_contact_point[0] = temp_manifold.points[0];
			c2->right_wheel_contact_point[1] = temp_manifold.points[1];

			if (!c2->right_wheel_contact) {
				c2->right_wheel_contact = true;

			}

		}

		if ((type_b == c2->get_leftwheel() && type_a == t->getchainbody())) {

			b2WorldManifold temp_manifold;
			contact->GetWorldManifold(&temp_manifold);
			c2->left_wheel_contact_point[0] = temp_manifold.points[0];
			c2->left_wheel_contact_point[1] = temp_manifold.points[1];


			if (!c2->left_wheel_contact) {
				c2->left_wheel_contact = true;
			}
		}

	}


	void EndContact(b2Contact* contact) {
		b2Body* type_a = static_cast<b2Body*>(contact->GetFixtureA()->GetBody()->GetUserData());
		b2Body* type_b = static_cast<b2Body*>(contact->GetFixtureB()->GetBody()->GetUserData());

		if (type_a == c1->get_chasis() && type_b == t->getchainbody()) {
			if (c1->chasis_contact) {
				c1->chasis_contact = false;
			}
		}

		if (type_a == c2->get_chasis() && type_b == t->getchainbody()) {
			if (c2->chasis_contact) {
				c2->chasis_contact = false;
			}
		}


		if ((type_b == c1->get_rightwheel() && type_a == t->getchainbody())) {
			//c1->right_wheel_contact_point = b2Vec2(0, 0);
			r_counter1++;

			if (c1->right_wheel_contact && r_counter1 > 3) {
				c1->right_wheel_contact = false;
				//std::cout << "right wheel stopped touching" << std::endl;
				r_counter1 = 0;
			}

		}

		if ((type_b == c1->get_leftwheel() && type_a == t->getchainbody())) {
			//c1->left_wheel_contact_point = b2Vec2(0, 0);
			l_counter1++;
			if (c1->left_wheel_contact && l_counter1 > 3) {
				c1->left_wheel_contact = false;
				//std::cout << "left wheel stopped touching" << std::endl;
				l_counter1 = 0;
			}
		}





		if ((type_b == c2->get_rightwheel() && type_a == t->getchainbody())) {
			//c2->right_wheel_contact_point = b2Vec2(0, 0);
			r_counter2++;

			if (c2->right_wheel_contact && r_counter2 > 3) {
				c2->right_wheel_contact = false;
				//std::cout << "right wheel stopped touching" << std::endl;
				r_counter2 = 0;
			}

		}

		if ((type_b == c2->get_leftwheel() && type_a == t->getchainbody())) {
			//c2->left_wheel_contact_point = b2Vec2(0, 0);
			l_counter2++;
			if (c2->left_wheel_contact && l_counter2 > 3) {
				c2->left_wheel_contact = false;
				//std::cout << "left wheel stopped touching" << std::endl;
				l_counter2 = 0;
			}
		}

	}


};