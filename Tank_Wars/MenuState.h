#pragma once
#include "state.h"
#include "Definitions.h"
#include "ResourceManager.h"

enum MenuScreen { Main, Settings, Credits };

class MenuState :public State {
private:
	sf::Font m_font;
	sf::Font m_controlsFont;

	sf::Text* m_title;
	sf::Text* m_titleOptions[MENU_OPTIONS];

	sf::Text* m_settingsTitle;
	sf::Text* m_settingsOptions[SETTING_LIST];

	sf::Text* m_creditTitle;
	sf::Text* m_creditOptions[CREDITS_OPTIONS];
	sf::Text* m_Credits[CREDITS_ITEMS];
	sf::Text* m_names[CREDITS_ITEMS];
	sf::Text* m_poweredBySFML;

	sf::Music m_menuTheme;

	bool m_optionSelected[MENU_OPTIONS];
	bool m_settingSelected[SETTING_LIST];
	bool m_creditSelected[CREDITS_OPTIONS];

	int m_mouseX, m_mouseY;
	bool m_clicked;
	bool m_startGame;

	MenuScreen m_currentScreen;

protected:
	void input();
	void update(float elapsedTime);
	void draw() const;

public:
	MenuState(sf::RenderWindow& window,fontManager &fonts, soundbufferManager& sounds);
	~MenuState();

	void show();

	inline bool startGame() { return m_startGame; }
	void setStartGame(bool start)
	{
		m_startGame = start;
	}
};


