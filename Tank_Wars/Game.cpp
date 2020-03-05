#include "Game.h"

Game::Game(sf::RenderWindow& window, textureManager& textures) :world(b2Vec2(0, 9.8))
{
    p1 = new Player(world, inti_pos_player1.x, inti_pos_player1.y, false, textures);
    p2 = new Player(world, inti_pos_player2.x, inti_pos_player2.y, true, textures);

    game_timing = nullptr;

    multiplier = 15;
    std::vector	<unsigned int>	values;

    srand((unsigned)time(0));
    double offsetx = (rand() % 1000);
    double y = rand() % 1000;
    int octaves, llim = (250 + (rand() % 50)), ulim = DEFAULT_HEIGHT * 0.8;
    double persistance, scale, detail;
    octaves = (1 + rand() % 4);
    persistance = (1 + (rand() % 20)) * 0.001;
    scale = (1 + (rand() % 300)) * 0.00001;
    detail = DEFAULT_WIDTH / 1.0;
    double x = offsetx / detail;
    double yy = y / detail;

    values = get_values(octaves, persistance, scale, llim, ulim, x, yy);

    t = new terrain(values, window);

    t->CreateGround(world);

    listner_instance.setup(p1->tank, p2->tank, *t);
    world.SetContactListener(&listner_instance);
    p1->tank.test = 1;
    p2->tank.test = 2;

    p1->armory.push_back(Weapon::none);
    p1->armory.push_back(Weapon::single_shot);
    p1->armory.push_back(Weapon::triple_shot);
    p1->armory.push_back(Weapon::penta_shot);
    p1->armory.push_back(Weapon::total_destroy);
    p1->armory.push_back(Weapon::scatter_shot);
    p1->selected_weapon = &p1->armory[0];

    p2->armory.push_back(Weapon::none);
    p2->armory.push_back(Weapon::single_shot);
    p2->armory.push_back(Weapon::triple_shot);
    p2->armory.push_back(Weapon::penta_shot);
    p2->armory.push_back(Weapon::total_destroy);
    p2->armory.push_back(Weapon::scatter_shot);
    p2->selected_weapon = &p2->armory[0];

    selected_player = p1;

    bg = textures.get(textureIdentifier::game_background);
    bg_s.setTexture(bg);
    bg_s.scale(2.f * sf::Vector2f(1, 1));
    bg_s.setOrigin(bg.getSize().x / 2, bg.getSize().x / 2);
    bg_s.setPosition(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2);

}

std::vector<unsigned int> Game::get_values(int octaves, double persistance, double scale, int llim, int ulim, double x, double yy) {
    std::vector	<unsigned int> values;

    for (int i = 0; i < DEFAULT_WIDTH; i++)
        values.push_back(scaled_octave_noise_2d(octaves, persistance, scale, llim, ulim, double(i + x), yy));
    return  values;

}

void Game::make(float dt)
{

    //code to select appropriate player
    if (selected_player->state == Player_state::away) {

        selected_player->state = Player_state::idle;

        if (selected_player == p1) {
            selected_player = p2;
            //selected_player->state = Player_state::idle;
            std::cout << "selected player is p2" << std::endl;
        }
        else if (selected_player == p2) {
            selected_player = p1;
            //selected_player->state == Player_state::idle;
            std::cout << "selected player is p1" << std::endl;
        }

    }


    //code to maintain the movement time of tanks
    if (game_timing != nullptr) {
        if (selected_player->state == Player_state::move_left) {
            if (game_timing->getElapsedTime().asSeconds() < player_move_time) {
                selected_player->tank.moveleft(*t);
            }
            else
            {
                selected_player->tank.stop(*t);
                selected_player->state = Player_state::idle;
                delete game_timing;
                game_timing = nullptr;
            }
        }
        else if (selected_player->state == Player_state::move_right) {
            if (game_timing->getElapsedTime().asSeconds() < player_move_time) {
                selected_player->tank.moveright(*t);
            }
            else
            {
                selected_player->tank.stop(*t);
                selected_player->state = Player_state::idle;
                delete game_timing;
                game_timing = nullptr;
            }
        }
    }


    //code to check if a joint is needed to make or destroy depending on the position of wheels and ground
    if (p1->state == Player_state::idle || p1->state == Player_state::away) {
        if (p1->tank.left_wheel_contact && p1->tank.right_wheel_contact) {

            if (p1->tank.chasis_joint == nullptr) {

                p1->tank.makejoint(*t);
                //std::cout << "joint made becauuse they are touuching of p1" << std::endl<< std::endl;

            }
        }
        else {
            if (p1->tank.chasis_joint != nullptr) {
                world.DestroyJoint(p1->tank.chasis_joint);
                //std::cout << "joint destroyed becaues in the air of p1" << std::endl<< std::endl;
                p1->tank.chasis_joint = nullptr;
            }
        }
    }

    if (p2->state == Player_state::idle || p2->state == Player_state::away) {

        if (p2->tank.left_wheel_contact && p2->tank.right_wheel_contact) {

            if (p2->tank.chasis_joint == nullptr) {

                p2->tank.makejoint(*t);
                //std::cout << "joint made becauuse they are touuching of p2" << std::endl;

            }
        }
        else {
            if (p2->tank.chasis_joint != nullptr) {
                world.DestroyJoint(p2->tank.chasis_joint);
                //std::cout << "joint destroyed becaues in the air of p2" << std::endl;
                p2->tank.chasis_joint = nullptr;
            }
        }

    }


    //Code to make the projectile of player
    p1->make(dt, multiplier, *t, p2->tank);
    p2->make(dt, multiplier, *t, p1->tank);


    //Code to simulate the world of box2d
    world.Step(dt, 5, 8);


    //code to find right time to recompute chainshape of box2d world
    if (t->need_update) {
        if (!t->need_update_b2d)
            t->need_update_b2d = true;
    }

    if (t->need_update_b2d) {
        if (!t->need_update) {
            t->recomputechain(world);

            //check if fall is needed and update values of contact variables
            p1->tank.fall_if_needed(*t);
            p2->tank.fall_if_needed(*t);

        }


        //Code to make terrain in sfml world depending on its case 
        t->make();
    }
}

void Game::draw(sf::RenderWindow &window)
{

    window.draw(bg_s);
    for (b2Body* BodyIterator = world.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
    {

        if (BodyIterator->GetType() != b2_dynamicBody)
        {
            window.draw(*t);
        }
        else
        {
            float X = SCALE * BodyIterator->GetPosition().x;
            float Y = SCALE * BodyIterator->GetPosition().y;

            b2Body* body = static_cast<b2Body*> (BodyIterator->GetUserData());

            if (body == p1->tank.get_chasis())
            {

                p1->tank.sfml_tank->chasis.setOrigin(body->GetLocalCenter().x, body->GetLocalCenter().y);
                p1->tank.sfml_tank->chasis.setPosition(X, Y);
                p1->tank.sfml_tank->chasis.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);

                sf::Transform trans = p1->tank.sfml_tank->chasis.getTransform();


                sf::Vertex point(sf::Vector2f(-1, p1->tank.sfml_tank->chasis.getPoint(0).y - 3));
                sf::Vector2f p = trans.transformPoint(point.position);
                p1->tank.sfml_tank->barrel.setPosition(p.x, p.y);

                window.draw(p1->tank.sfml_tank->barrel);
                window.draw(p1->tank.sfml_tank->top, trans);
                window.draw(p1->tank.sfml_tank->chasis);

            }
            else if (body == p1->tank.get_leftwheel()) {
                p1->tank.sfml_tank->left_wheel.setPosition(X, Y);
                p1->tank.sfml_tank->left_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p1->tank.sfml_tank->left_wheel);

            }
            else if (body == p1->tank.get_rightwheel()) {
                p1->tank.sfml_tank->right_wheel.setPosition(X, Y);
                p1->tank.sfml_tank->right_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p1->tank.sfml_tank->right_wheel);


            }

            if (body == p2->tank.get_chasis())
            {

                p2->tank.sfml_tank->chasis.setOrigin(body->GetLocalCenter().x, body->GetLocalCenter().y);
                p2->tank.sfml_tank->chasis.setPosition(X, Y);
                p2->tank.sfml_tank->chasis.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);

                sf::Transform trans = p2->tank.sfml_tank->chasis.getTransform();

                sf::Vertex point(sf::Vector2f(6, p2->tank.sfml_tank->chasis.getPoint(0).y - 2));
                sf::Vector2f p = trans.transformPoint(point.position);
                p2->tank.sfml_tank->barrel.setPosition(p.x, p.y);

                window.draw(p2->tank.sfml_tank->barrel);
                window.draw(p2->tank.sfml_tank->top, trans);
                window.draw(p2->tank.sfml_tank->chasis);

            }
            else if (body == p2->tank.get_leftwheel()) {
                p2->tank.sfml_tank->left_wheel.setPosition(X, Y);
                p2->tank.sfml_tank->left_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p2->tank.sfml_tank->left_wheel);
            }
            else if (body == p2->tank.get_rightwheel()) {
                p2->tank.sfml_tank->right_wheel.setPosition(X, Y);
                p2->tank.sfml_tank->right_wheel.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
                window.draw(p2->tank.sfml_tank->right_wheel);
            }


        }
    }

    p1->draw(window);
    p2->draw(window);
    selected_player->draw_arrow(window);
    
}

Game::~Game() {
    delete t;
    delete p1;
    delete p2;

}