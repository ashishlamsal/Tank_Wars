#pragma once
#include "state.h"
#include "Definitions.h"

class PauseState :public State {
private:
	sf::Font m_font;

	sf::Text* m_title;
	sf::Text* m_options[PAUSE_OPTIONS];

	sf::Sprite m_background;
	sf::RectangleShape m_rect;

	bool m_selected[PAUSE_OPTIONS];
	int m_mouseX, m_mouseY;
	bool m_clicked;
	bool m_resumeGame;
	bool m_quitGame;

protected:
	void input();
	void update(float elapsedTime);
	void draw() const;

public:
	PauseState(sf::RenderWindow& window, fontManager& fonts, soundbufferManager& sounds);
	~PauseState();

	void show(sf::Sprite& background);
	void resume();

	inline bool quitGame() { return m_quitGame; }
	void setQuitGame(bool quit)
	{
		m_quitGame = quit;
	}
};



