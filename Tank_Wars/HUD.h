#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Definitions.h"
#include "state.h"
#include "utility.h"
#include "Game.h"


enum Element { Controls, HP, Score, Angle, Power, Weapons, TimePassed };

class HUD {
private:

	sf::Font m_font;

	sf::Text* m_player;
	sf::Text* m_enemy;
	sf::Text* m_time;
	sf::Text* m_fire;
	sf::Text* m_moves;
	sf::Text* m_power;
	sf::Text* m_score1;
	sf::Text* m_score2;
	sf::Text* m_angle;
	sf::Text* m_weapon_title;
	sf::Text* m_currentweapon;

	sf::SoundBuffer m_fireSoundBuffer;
	sf::Sound m_clockTick;

	sf::RectangleShape rect_power;
	sf::RectangleShape hp_player1;
	sf::RectangleShape hp_player2;
	

	sf::Texture m_background;
	sf::Sprite m_backgroundSprite;

	sf::Texture m_button[HUD_TEXTURE];
	sf::Sprite m_buttonSprite[HUD_SPRITE];

	bool m_optionSelected[HUD_OPTIONS];
	int p1_score, p2_score;
	int p1_hp, p2_hp;
	float angle;

public:
	int m_mouseX, m_mouseY;
	bool mouse_captured_for_angle;
	bool m_clicked;

	HUD(sf::RenderWindow& window, fontManager& fonts, textureManager& textures);
	~HUD();

	void update(Element element, float elapsedTime, Game& m_game);
	void draw(sf::RenderWindow& window);
};


