#pragma once
#include <ctime>
#include "state.h"
#include "Definitions.h"
#include "Game.h" 


class HUD;
class PauseState;
class EndGameState;

class GameState : public State {
private:

	HUD* m_hud;

	PauseState* m_pauseState;
	EndGameState* m_endGameState;

	sf::Music m_mainTheme;

	sf::SoundBuffer m_pauseBuffer;
	sf::Sound m_pause;

	bool m_gameOver;
	float m_time;
	float m_timeSinceLastFrame;

	Game* m_game;

	void result();
	void run();

protected:
	void input() {};
	void input(HUD& h);
	void update(float elapsedTime);
	void draw() const;

public:
	GameState(sf::RenderWindow& window, fontManager& fonts, textureManager& textures, soundbufferManager& sounds);
	~GameState();

	Game getGame() { return *m_game; }
	void pause(bool& wasPaused);
	void restart();

	inline bool gameOver() { return m_gameOver; }

};