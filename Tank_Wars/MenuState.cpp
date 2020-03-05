#include "MenuState.h"
#include "utility.h"

MenuState::MenuState(sf::RenderWindow& window, fontManager &fonts, soundbufferManager& sounds) : State(window,sounds), m_mouseX(0), m_mouseY(0), m_clicked(false), m_startGame(false), m_currentScreen(Main)
{
	m_font = fonts.get(fontIdentifier::game_font);
	m_controlsFont = fonts.get(fontIdentifier::settings_font);
	m_menuTheme.openFromFile(MENU_STATE_MUSIC_PATH);
	m_menuTheme.setVolume(50);
	m_menuTheme.setLoop(true);


	m_title = new sf::Text(GAME_TITLE, m_font, TITLE_TEXT_SIZE * getScaleFactors().x);
	m_titleOptions[0] = new sf::Text("Play", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_titleOptions[1] = new sf::Text("Controls", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_titleOptions[2] = new sf::Text("Credits", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_titleOptions[3] = new sf::Text("Exit", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);

	m_settingsTitle = new sf::Text("Controls", m_font, SUB_TITLE_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[0] = new sf::Text("Back", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[1] = new sf::Text("KEYBOARD", m_controlsFont, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[2] = new sf::Text("MOUSE", m_controlsFont, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[3] = new sf::Text("WEAPONS", m_controlsFont, OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[4] = new sf::Text("F : FIRE\nA : MOVE LEFT\nD : MOVE RIGHT\nR : RESPAWN\nQ : SKIP", m_controlsFont, SMALL_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[5] = new sf::Text("MOUSE SCROLL UP         : POWER UP\nMOUSE SCROLL DOWN : POWER DOWN", m_controlsFont, SMALL_TEXT_SIZE * getScaleFactors().x);
	m_settingsOptions[6] = new sf::Text("KEYBOARD 1  : 1 SHOT\nKEYBOARD 2 : 3 SHOT\nKEYBOARD 3 : 5 SHOT\nKEYBOARD 4 : POWER SHOT\nKEYBOARD 5 : SCATTER SHOT\n", m_controlsFont, SMALL_TEXT_SIZE * getScaleFactors().x);


	m_creditTitle = new sf::Text("Credits", m_font, SUB_TITLE_TEXT_SIZE * getScaleFactors().x);
	m_Credits[0] = new sf::Text("Game UI Designer", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);
	m_Credits[1] = new sf::Text("Game Programmer", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);
	m_Credits[2] = new sf::Text("Game Specialist", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);

	m_names[0] = new sf::Text("Ashish Lamsal", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);
	m_names[1] = new sf::Text("Ankit Poudel", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);
	m_names[2] = new sf::Text("Arpan Pokharel", m_font, CREDITS_TEXT_SIZE * getScaleFactors().x);

	m_poweredBySFML = new sf::Text("Powered By SFML 2.5.1", m_font, SMALL_TEXT_SIZE * getScaleFactors().x);

	m_creditOptions[0] = new sf::Text("Back", m_font, OPTIONS_TEXT_SIZE * getScaleFactors().x);

	formatText(m_title, getScreenWidth() / 2 - m_title->getGlobalBounds().width / 2, getScreenHeight() / 3 - m_title->getGlobalBounds().height / 2, sf::Color::Red, sf::Color::White, true);

	for (int i = 0; i < MENU_OPTIONS; i++)
	{
		formatText(m_titleOptions[i], getScreenWidth() / 2 - m_titleOptions[i]->getGlobalBounds().width / 2, getScreenHeight() / 2 + 96 * (i + 1) * getScaleFactors().x - m_titleOptions[i]->getGlobalBounds().height / 2, sf::Color::White);
	}

	formatText(m_settingsTitle, getScreenWidth() / 2 - m_settingsTitle->getGlobalBounds().width / 2, getScreenHeight() / 6 - m_title->getGlobalBounds().height / 2, sf::Color::Red, sf::Color::White, true);
	formatText(m_settingsOptions[0], getScreenWidth() * 6 / 7 - m_settingsOptions[0]->getGlobalBounds().width / 2, getScreenHeight() * 6 / 7 - m_settingsOptions[0]->getGlobalBounds().height, sf::Color::White);

	//keyboard
	formatText(m_settingsOptions[1], getScreenWidth() * 1 / 8, getScreenHeight() * 1 / 4, TEXT_COLOR_WHITE);
	formatText(m_settingsOptions[4], getScreenWidth() * 1 / 8, getScreenHeight() * 1 / 4 + 100, TEXT_COLOR_YELLOW);
	//weapons
	formatText(m_settingsOptions[3], getScreenWidth() * 5 / 8, getScreenHeight() * 2 / 5, TEXT_COLOR_WHITE);
	formatText(m_settingsOptions[6], getScreenWidth() * 5 / 8, getScreenHeight() * 2 / 5+100, TEXT_COLOR_YELLOW);
	//mouse
	formatText(m_settingsOptions[2], getScreenWidth() * 1 / 8, getScreenHeight() * 3 / 5, TEXT_COLOR_WHITE);
	formatText(m_settingsOptions[5], getScreenWidth() * 1 / 8, getScreenHeight() * 3 / 5+100, TEXT_COLOR_YELLOW);

	formatText(m_creditTitle, getScreenWidth() / 2 - m_creditTitle->getGlobalBounds().width / 2, getScreenHeight() / 6 - m_creditTitle->getGlobalBounds().height / 2, sf::Color::Red, sf::Color::White, true);

	for (int i = 0; i < CREDITS_ITEMS; i++)
	{
		formatText(m_Credits[i], getScreenWidth() / 5 - m_Credits[i]->getGlobalBounds().width / 2, getScreenHeight() * 1 / 3 + 96 * i * getScaleFactors().x - m_Credits[i]->getGlobalBounds().height / 2, TEXT_COLOR_BLUE);

		formatText(m_names[i], getScreenWidth() * 3 / 4 - m_names[i]->getGlobalBounds().width / 2, getScreenHeight() * 1 / 3 + 96 * i * getScaleFactors().x - m_names[i]->getGlobalBounds().height / 2, TEXT_COLOR_YELLOW);
	}
	formatText(m_poweredBySFML, getScreenWidth() / 2 - m_poweredBySFML->getGlobalBounds().width / 2, getScreenHeight() * 2 / 5 + 96 * 3 * getScaleFactors().x - m_poweredBySFML->getGlobalBounds().height / 2, sf::Color::White);
	formatText(m_creditOptions[0], getScreenWidth() * 6 / 7 - m_creditOptions[0]->getGlobalBounds().width / 2, getScreenHeight() * 6 / 7 - m_creditOptions[0]->getGlobalBounds().height, sf::Color::White);

	m_settingSelected[0] = false;
	for (int i = 0; i < MENU_OPTIONS; i++)
		m_optionSelected[i] = false;
	for (int i = 0; i < CREDITS_OPTIONS; i++)
		m_creditSelected[i] = false;
}

MenuState::~MenuState()
{

	delete m_title;
	delete m_settingsTitle;
	delete m_creditTitle;
	for (int i = 0; i < MENU_OPTIONS; i++)
		delete m_titleOptions[i];
	for (int i = 0; i < SETTING_LIST; i++)
		delete m_settingsOptions[i];
	for (int i = 0; i < CREDITS_OPTIONS; i++)
		delete m_creditOptions[i];
	for (int i = 0; i < CREDITS_ITEMS; i++)
	{
		delete m_Credits[i];
		delete m_names[i];
	}
	delete m_poweredBySFML;
}

void MenuState::show()
{
	if (m_menuTheme.getStatus() == sf::SoundSource::Status::Stopped)
		m_menuTheme.play();

	m_window->setMouseCursorVisible(true);

	while (!m_startGame && m_window->isOpen())
	{
		float elapsedTime = m_clock->restart().asSeconds();
		input();
		update(elapsedTime);
		draw();
	}
}

void MenuState::input()
{
	sf::Event evnt;

	if (m_window->pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		case sf::Event::MouseMoved:
			m_mouseX = evnt.mouseMove.x;
			m_mouseY = evnt.mouseMove.y;
			break;
		case sf::Event::MouseButtonPressed:
			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				m_mouseX = evnt.mouseButton.x;
				m_mouseY = evnt.mouseButton.y;
				m_clicked = true;
			}break;
		default:
			break;
		}
	}
}

void MenuState::update(float elapsedTime)
{
	switch (m_currentScreen)
	{
	case Main:
		for (int i = 0; i < MENU_OPTIONS; i++)
		{
			if (m_titleOptions[i]->getGlobalBounds().contains(m_mouseX, m_mouseY))
			{
				if (!m_optionSelected[i])
				{
					m_select.play();
					m_optionSelected[i] = true;
					m_titleOptions[i]->setFillColor(TEXT_COLOR_RED);
				}
			}
			else
			{
				if (m_optionSelected[i])
				{
					m_optionSelected[i] = false;
					m_titleOptions[i]->setFillColor(sf::Color::White);
				}
			}
		}
		if (m_clicked)
		{
			if (m_optionSelected[0])
			{
				m_click.play();
				m_window->setMouseCursorVisible(true);
				m_startGame = true;
				m_menuTheme.stop();
			}
			else
			{
				if (m_optionSelected[1])
				{
					m_click.play();
					m_currentScreen = Settings;
				}
				else
				{
					if (m_optionSelected[2])
					{
						m_click.play();
						m_currentScreen = Credits;
					}
					else
						if (m_optionSelected[3])
						{
							m_window->close();
						}
				}

			}
			m_clicked = false;
		}
		break;

	case Settings:
		{
			if (m_settingsOptions[0]->getGlobalBounds().contains(m_mouseX, m_mouseY))
			{
				if (!m_settingSelected[0])
				{
					m_select.play();
					m_settingSelected[0] = true;
					m_settingsOptions[0]->setFillColor(TEXT_COLOR_RED);
				}
			}
			else
			{
				if (m_settingSelected[0])
				{
					m_settingSelected[0] = false;
					m_settingsOptions[0]->setFillColor(sf::Color::White);
				}
			}
		}
		if (m_clicked)
		{
			if (m_settingSelected[0])
			{
				m_click.play();
				m_currentScreen = Main;
			}
			m_clicked = false;
		}
		break;

	case Credits:
		for (int i = 0; i < CREDITS_OPTIONS; i++)
		{
			if (m_creditOptions[i]->getGlobalBounds().contains(m_mouseX, m_mouseY))
			{
				if (!m_creditSelected[i])
				{
					m_select.play();
					m_creditSelected[i] = true;
					m_creditOptions[i]->setFillColor(TEXT_COLOR_RED);
				}
			}
			else
			{
				if (m_creditSelected[i])
				{
					m_creditSelected[i] = false;
					m_creditOptions[i]->setFillColor(sf::Color::White);
				}
			}
		}
		if (m_clicked)
		{
			if (m_creditSelected[0])
			{
				m_click.play();
				m_currentScreen = Main;
			}
			m_clicked = false;
		}
		break;
	default:
		break;
	}
}

void MenuState::draw() const
{
	m_window->clear(sf::Color::Black);
	switch (m_currentScreen)
	{
	case Main:
		m_window->draw(*m_title);
		for (int i = 0; i < MENU_OPTIONS; i++)
			m_window->draw(*m_titleOptions[i]);
		break;
	case Settings:
		m_window->draw(*m_settingsTitle);
		for (int i = 0; i < SETTING_LIST; i++)
			m_window->draw(*m_settingsOptions[i]);
		break;
	case Credits:
		m_window->draw(*m_creditTitle);
		for (int i = 0; i < CREDITS_OPTIONS; i++)
			m_window->draw(*m_creditOptions[i]);
		for (int i = 0; i < CREDITS_ITEMS; i++)
		{
			m_window->draw(*m_Credits[i]);
			m_window->draw(*m_names[i]);
		}
		m_window->draw(*m_poweredBySFML);
		break;

	default:
		break;
	}
	m_window->display();
}


