#pragma once
#include "src/terrain.h"
#include "src/contact_listener.h"
#include "src/Player.h"
#include "Definitions.h"
#include "src/simplexnoise.h"
#include "ResourceManager.h"

static const float player_move_time = 2.0f;

static const b2Vec2 inti_pos_player1(400, 200);
static const b2Vec2 inti_pos_player2(DEFAULT_WIDTH - 400, 200);

class Game
{
private:

	b2World world;
	terrain* t;
	Player* p1;
	Player* p2;
	float multiplier;
	sf::Clock* game_timing;
	unsigned max_fps;

	contactlistener listner_instance;

	sf::Texture bg;
	sf::Sprite  bg_s;

public:
	Player* selected_player;	
	void make(float dt);
	void draw(sf::RenderWindow &window);

	Game( sf::RenderWindow& window, textureManager& textures);
	Player* getPlayer1() { return p1; }
	Player* getPlayer2() { return p2; }
	sf::Clock* get_game_timing() { return game_timing; }
	void set_game_timing() {
		if (game_timing == nullptr)
			game_timing = new sf::Clock;
	}

	void respawn_selected_player() {
		if (selected_player->state == Player_state::idle)
			selected_player == p1 ? selected_player->tank.respawn_Tank(false) : selected_player->tank.respawn_Tank(true);
	}

	void fire_selected_player() {
		if (selected_player->state == Player_state::idle) {
			selected_player->fire();
		}
	}

	void change_selected_player() {
		if (selected_player == p1) {
			selected_player = p2;
		}
		else if (selected_player == p2) {
			selected_player = p1;
		}
	}
	std::vector<unsigned int> get_values(int octaves, double persistance, double scale, int llim, int ulim, double x, double yy);

	~Game();
};

