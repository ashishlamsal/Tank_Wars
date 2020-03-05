#include "HUD.h"
#include "utility.h"

HUD::HUD(sf::RenderWindow& window, fontManager& fonts, textureManager& textures) : m_mouseX(0), m_mouseY(0), m_clicked(false), p1_score(0), p2_score(0), angle(0), p1_hp(0), p2_hp(0)
{
	{
		m_font = fonts.get(fontIdentifier::game_font);

		m_player = new sf::Text("Player 1", m_font, HUD_TEXT_SIZE * State::getScaleFactors().x);
		m_enemy = new sf::Text("Player 2", m_font, HUD_TEXT_SIZE * State::getScaleFactors().x);
		m_time = new sf::Text("Time: 0", m_font, HUD_TEXT_SIZE * State::getScaleFactors().x);
		m_fire = new sf::Text("FIRE", m_font, HUD_TEXT_SIZE * State::getScaleFactors().x);
		m_moves = new sf::Text("MOVE", m_font, HUD_OPTIONS_TEXT_SIZE * State::getScaleFactors().x);
		m_power = new sf::Text("POWER", m_font, HUD_OPTIONS_TEXT_SIZE * State::getScaleFactors().x);
		m_score1 = new sf::Text("SCORE: 0", m_font, HUD_SCORE_TEXT_SIZE * State::getScaleFactors().x);
		m_score2 = new sf::Text("SCORE: 0", m_font, HUD_SCORE_TEXT_SIZE * State::getScaleFactors().x);
		m_angle = new sf::Text("ANGLE", m_font, HUD_OPTIONS_TEXT_SIZE * State::getScaleFactors().x);
		m_weapon_title = new sf::Text("WEAPON", m_font, HUD_OPTIONS_TEXT_SIZE * State::getScaleFactors().x);
		m_currentweapon = new sf::Text("WEAPON", m_font, HUD_SCORE_TEXT_SIZE * State::getScaleFactors().x);

		//background image HUD
		m_background = textures.get(textureIdentifier::hud_background);
		m_backgroundSprite.setPosition(sf::Vector2f(0, 1080 - 250));
		m_backgroundSprite.setTexture(m_background);

		//play button
		m_button[0] = textures.get(textureIdentifier::hud_playbutton);
		m_buttonSprite[0].setTexture(m_button[0]);
		m_buttonSprite[0].setPosition(State::getScreenWidth() / 2 - m_time->getGlobalBounds().width / 2, 850);
		m_buttonSprite[0].scale(sf::Vector2f(1.5f, 1.5f));

		//left click button
		m_button[1] = textures.get(textureIdentifier::hud_leftclick);
		m_buttonSprite[1].setTexture(m_button[1]);
		m_buttonSprite[1].setPosition(sf::Vector2f(1200, 1080 - 220));

		//right click button
		m_button[2] = textures.get(textureIdentifier::hud_rightclick);
		m_buttonSprite[2].setTexture(m_button[2]);
		m_buttonSprite[2].setPosition(sf::Vector2f(1500, 1080 - 220));

		//hud power bar
		m_button[3] = textures.get(textureIdentifier::hud_powerbar);
		m_buttonSprite[3].setTexture(m_button[3]);
		m_buttonSprite[3].setPosition(sf::Vector2f(1200, 1080 - 100));

		//hud angle button
		m_button[4] = textures.get(textureIdentifier::hud_anglebutton);
		m_buttonSprite[4].setTexture(m_button[4]);
		m_buttonSprite[4].setPosition(sf::Vector2f(500, 1080 - 230));
		m_buttonSprite[4].scale(sf::Vector2f(2.f, 2.f));

		//hud power down
		m_buttonSprite[5].setTexture(m_button[1]);
		m_buttonSprite[5].setPosition(sf::Vector2f(1140, 1080 - 90));

		//hud power up
		m_buttonSprite[6].setTexture(m_button[2]);
		m_buttonSprite[6].setPosition(sf::Vector2f(1560, 1080 - 90));

		formatText(m_player, 32, HUD_TEXT_Y, TEXT_COLOR_GREEN, sf::Color::White, true);
		formatText(m_enemy, State::getScreenWidth() - m_enemy->getGlobalBounds().width - 64, HUD_TEXT_Y, TEXT_COLOR_GREEN, sf::Color::White, true);
		formatText(m_time, State::getScreenWidth() / 2 - m_time->getGlobalBounds().width / 2, HUD_TEXT_Y, TEXT_COLOR_BLUE, sf::Color::White, true);
		formatText(m_fire, m_buttonSprite[0].getPosition().x + 30, m_buttonSprite[0].getPosition().y + 70, TEXT_COLOR_RED, sf::Color::White, true);
		formatText(m_moves, m_buttonSprite[1].getPosition().x + 100, m_buttonSprite[1].getPosition().y, TEXT_COLOR_RED, sf::Color::White, true);
		formatText(m_power, m_buttonSprite[3].getPosition().x + 80, m_buttonSprite[3].getPosition().y - 40, TEXT_COLOR_RED, sf::Color::White, true);
		formatText(m_score1, 32, HUD_TEXT_Y + 60, TEXT_COLOR_YELLOW, sf::Color::White, true);
		formatText(m_score2, State::getScreenWidth() - m_score2->getGlobalBounds().width - 120, HUD_TEXT_Y + 60, TEXT_COLOR_YELLOW, sf::Color::White, true);
		formatText(m_angle, m_buttonSprite[4].getPosition().x, m_buttonSprite[4].getPosition().y, TEXT_COLOR_RED, sf::Color::White, true);
		formatText(m_weapon_title, 100, m_buttonSprite[1].getPosition().y, TEXT_COLOR_RED, sf::Color::White, true);
		formatText(m_currentweapon, 100, m_buttonSprite[1].getPosition().y + 50, TEXT_COLOR_YELLOW, sf::Color::White, true);
		

		rect_power.setPosition(sf::Vector2f(m_buttonSprite[3].getPosition().x + 65, m_buttonSprite[3].getPosition().y + 23));
		rect_power.setSize(sf::Vector2f(270, 35));
		rect_power.setFillColor(TEXT_COLOR_RED);

		hp_player1.setPosition(sf::Vector2f(m_score1->getPosition().x, m_score1->getPosition().y + 50));
		hp_player1.setSize(sf::Vector2f(300, 15));
		hp_player1.setFillColor(TEXT_COLOR_RED);

		hp_player2.setPosition(sf::Vector2f(m_score2->getPosition().x, m_score2->getPosition().y + 50));
		hp_player2.setSize(sf::Vector2f(300, 15));
		hp_player2.setFillColor(TEXT_COLOR_RED);

		mouse_captured_for_angle = false;

		for (int i = 0; i < HUD_OPTIONS; i++)
			m_optionSelected[i] = false;

	}
}

HUD::~HUD()
{
	delete m_player;
	delete m_enemy;
	delete m_time;
	delete m_fire;
	delete m_moves;
	delete m_power;
	delete m_score1;
	delete m_score2;
	delete m_angle;
	delete m_weapon_title;
	delete m_currentweapon;
}


void HUD::update(Element element, float elapsedTime, Game& m_game)
{
	switch (element)
	{
	case Controls:
		for (int i = 0; i < HUD_OPTIONS; i++)
		{
			if (m_buttonSprite[i].getGlobalBounds().contains(m_mouseX, m_mouseY))
			{
				if (!m_optionSelected[i])
				{
					m_optionSelected[i] = true;
					m_buttonSprite[i].setColor(TEXT_COLOR_RED);
				}
			}
			else
			{
				if (m_optionSelected[i])
				{
					m_optionSelected[i] = false;
					m_buttonSprite[i].setColor(sf::Color::White);
				}
			}
		}

		if (m_clicked)
		{
			if (m_optionSelected[0])
			{
				//fire
				m_game.fire_selected_player();
				m_clicked = false;
			}
			else
			{
				if (m_optionSelected[1])
				{
					//move left
					if (m_game.selected_player->state == Player_state::idle) {
						m_game.selected_player->state = Player_state::move_left;
						m_game.set_game_timing();
					}
				}
				else
				{
					if (m_optionSelected[2])
					{
						//move right
						if (m_game.selected_player->state == Player_state::idle) {
							m_game.selected_player->state = Player_state::move_right;
							m_game.set_game_timing();
						}
					}
					else
					{
						if (m_optionSelected[4])
						{
							//angle
							m_game.selected_player->tank.sfml_tank->point_to(sf::Vector2f(m_mouseX, m_mouseY));
							mouse_captured_for_angle = true;
						}
						else
						{
							if (m_optionSelected[5])
							{
								//power up
								m_game.selected_player->set_power(m_game.selected_player->get_power() - 3);

							}
							else
							{
								if (m_optionSelected[6])
								{
									//power down
									m_game.selected_player->set_power(m_game.selected_player->get_power() + 3);
								}
							}

						}

					}
				}
			}
			m_clicked = false;
		}
		break;

	case Score:
		p1_score = *(m_game.getPlayer1()->tank.get_score());
		p2_score = *(m_game.getPlayer2()->tank.get_score());
		m_score1->setString("Score: " + std::to_string(p1_score));
		m_score2->setString("Score: " + std::to_string(p2_score));
		break;

	case Angle:
		angle = -(m_game.selected_player->tank.sfml_tank->barrel_angle) * 180 / PI;
		m_angle->setString("Angle: " + std::to_string(static_cast<int>(angle)));
		break;

	case TimePassed:
		m_time->setString("Time: " + std::to_string(static_cast<int>(elapsedTime)));

		if (elapsedTime > 60)
			m_time->setFillColor(TEXT_COLOR_RED);
		else
		{
			if (elapsedTime > 20)
				m_time->setFillColor(TEXT_COLOR_YELLOW);
			else
			{
				m_time->setFillColor(TEXT_COLOR_GREEN);
			}
		}
		break;

	case Power:
		m_power->setString("Power: " + std::to_string(static_cast<int>(m_game.selected_player->get_power())));
		rect_power.setSize(sf::Vector2f((2.7 * m_game.selected_player->get_power()), 35));
		break;

	case HP:
		p1_hp = *(m_game.getPlayer1()->tank.get_health());
		p2_hp = *(m_game.getPlayer2()->tank.get_health());
		hp_player1.setSize(sf::Vector2f(3 * p1_hp, 15));
		hp_player2.setSize(sf::Vector2f(3 * p2_hp, 15));

		if (p1_hp > 70)
			hp_player1.setFillColor(TEXT_COLOR_GREEN);
		else
		{
			if (p1_hp > 40)
				hp_player1.setFillColor(TEXT_COLOR_YELLOW);
			else
			{
				hp_player1.setFillColor(TEXT_COLOR_RED);
			}
		}

		if (p2_hp > 70)
			hp_player2.setFillColor(TEXT_COLOR_GREEN);
		else
		{
			if (p2_hp > 40)
				hp_player2.setFillColor(TEXT_COLOR_YELLOW);
			else
			{
				hp_player2.setFillColor(TEXT_COLOR_RED);
			}
		}
		break;

	case Weapons:
		switch (*(m_game.selected_player->selected_weapon))
		{
		case Weapon::single_shot:
			m_currentweapon->setString("1 SHOT");
			break;
		case Weapon::triple_shot:
			m_currentweapon->setString("3 SHOT");
			break;
		case Weapon::penta_shot:
			m_currentweapon->setString("5 SHOT");
			break;
		case Weapon::scatter_shot:
			m_currentweapon->setString("SCATTTER\n SHOT");
			break;
		case Weapon::total_destroy:
			m_currentweapon->setString("POWER\n SHOT");
			break;
		}
		break;

	default:
		break;
	}
}


void HUD::draw(sf::RenderWindow& window)
{
	window.draw(*m_player);
	window.draw(*m_enemy);
	window.draw(*m_time);

	window.draw(m_backgroundSprite);
	for (int i = 0; i < 7; i++)
		window.draw(m_buttonSprite[i]);

	window.draw(*m_score1);
	window.draw(*m_score2);
	window.draw(*m_angle);
	window.draw(*m_weapon_title);
	window.draw(*m_currentweapon);
	window.draw(*m_fire);
	window.draw(*m_moves);
	window.draw(*m_power);
	window.draw(rect_power);
	window.draw(hp_player1);
	window.draw(hp_player2);
}
