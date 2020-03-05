#include "Player.h"

Player::Player(b2World& world, float x, float y, bool flip, textureManager& textures) {

    indicator.setup(textures);

    selected_weapon = new Weapon;
    *selected_weapon = Weapon::none;

    state = Player_state::idle;
    power = 50.f;

    b2FixtureDef Tankfixture, wheelfixure;
    Tankfixture.density = 6;
    Tankfixture.friction = .4;
    Tankfixture.restitution = 0.f;


    wheelfixure.density = 3;
    wheelfixure.friction = 10;
    wheelfixure.restitution = 0.1f;


    tank.setup(world, Tankfixture, wheelfixure, x, y, flip,textures);

    tank.sfml_tank->barrel_angle = -PI / 4;


}

void Player::make(float dt, float mul, terrain& t, Tank& tank2)
{
    indicator.make(dt);


    if (state == Player_state::fired) {

        if (m_shot != nullptr) {

            if (m_shot->is_initialized()) {
                m_shot->make(dt, mul, t, tank, tank2);

                if ((m_shot->is_done())) {
                    delete m_shot;
                    std::cout << "m_shot destroyed" << std::endl;
                    t.need_update = true;
                    m_shot = nullptr;
                    state = Player_state::away;
                }
            }
        }

    }

}

void Player::fire()
{
    if (*selected_weapon == Weapon::none)
        return;       

    else if (m_shot == nullptr) {
        switch (*selected_weapon) {

        case Weapon::single_shot:

            m_shot = new shot;
            m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 50, 40, 1);

            break;

        case Weapon::triple_shot:
            m_shot = new shot;
            m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 50, 30, 3);

            break;

        case Weapon::penta_shot:
            m_shot = new shot;
            m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 50, 20, 5);

            break;

        case Weapon::total_destroy:
            m_shot = new shot;
            m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 20, 15, 2);
            dynamic_cast<shot*>(m_shot)->is_powerful = true;
            break;

        case Weapon::scatter_shot:
            m_shot = new scatter;
            m_shot->init(tank.sfml_tank->muzzle, power * sf::Vector2f(cos(tank.sfml_tank->barrel_angle), sin(tank.sfml_tank->barrel_angle)), sf::Vector2f(0, 5), 35, 7.5);
            break;
        }
    }
    else
        return;



    state = Player_state::fired;
}

void Player::draw(sf::RenderWindow& window)
{

    if (state == Player_state::fired) {
        if (m_shot != nullptr) {

            //std::cout << "m_shot drawn" << std::endl;
            if (m_shot->is_initialized()) {
                m_shot->draw(window);
            }
        }
    }
    /*if (flying_text) {
        damage.draw(window, *tank.sfml_tank);
    }*/

}

void Player::draw_arrow(sf::RenderWindow& window)
{

    indicator.draw(window, *tank.sfml_tank);

}

Player::~Player() {
    //delete selected_weapon;
}