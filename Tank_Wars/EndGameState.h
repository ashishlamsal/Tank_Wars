#include "state.h"
#include "Definitions.h"
#include "ResourceManager.h"
#include "Game.h"
#include "utility.h"

class EndGameState : public State {
private:
	sf::Font m_font;

	sf::Text* m_title;
	sf::Text* m_options[END_GAME_OPTIONS];
	sf::Text* m_winnerText;
	sf::Text* m_winnerScore;

	sf::Sprite m_background;
	sf::RectangleShape m_rect;

	bool m_selected[END_GAME_OPTIONS];

	int m_mouseX, m_mouseY;
	bool m_clicked;
	bool m_restartGame;
	bool m_quitGame;

protected:
	void input();
	void update(float elapsedTime);
	void draw() const;

public:
	EndGameState(sf::RenderWindow& window,fontManager &fonts, soundbufferManager& sounds);
	~EndGameState();

	void show(sf::Sprite& background, Game& m_game);
	void restart();

	inline bool quitGame() { return m_quitGame; }
	void setQuitGame(bool quit)
	{
		m_quitGame = quit;
	}
};




