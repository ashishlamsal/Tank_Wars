#include "EndGameState.h"

EndGameState::EndGameState(sf::RenderWindow& window,fontManager& fonts, soundbufferManager& sounds) : State(window,sounds),
m_mouseX(0), m_mouseY(0), m_restartGame(false), m_quitGame(false)
{
	m_font = fonts.get(fontIdentifier::game_font);

	m_title = new sf::Text("Game Over", m_font, END_GAME_TEXT_SIZE * getScaleFactors().x);
	m_options[0] = new sf::Text("Restart", m_font, END_GAME_OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_options[1] = new sf::Text("Quit", m_font, END_GAME_OPTIONS_TEXT_SIZE * getScaleFactors().x);

	m_winnerText = new sf::Text("WINNER : ", m_font, END_GAME_SCORE_TEXT_SIZE * getScaleFactors().x);
	m_winnerScore = new sf::Text("Score : ", m_font, END_GAME_SCORE_TEXT_SIZE * getScaleFactors().x);

	formatText(m_title, getScreenWidth() / 2 - m_title->getGlobalBounds().width / 2,
		getScreenHeight() / 4 - m_title->getGlobalBounds().height / 2, TEXT_COLOR_RED, sf::Color::Red, true);
	formatText(m_options[0], 32, getScreenHeight() - m_options[0]->getGlobalBounds().height - 90,
		sf::Color::White, TEXT_COLOR_BLUE, true);
	formatText(m_options[1], getScreenWidth() - m_options[1]->getGlobalBounds().width - 32,
		getScreenHeight() - m_options[1]->getGlobalBounds().height - 90, sf::Color::White, sf::Color::Red, true);

	formatText(m_winnerText, getScreenWidth() * 2 / 5 - m_winnerText->getGlobalBounds().width / 2, getScreenHeight() / 2 - m_winnerText->getGlobalBounds().height / 2, sf::Color::White, sf::Color::Black, true);
	formatText(m_winnerScore, getScreenWidth() * 2 / 5 - m_winnerScore->getGlobalBounds().width / 2, getScreenHeight() * 2 / 3 - m_winnerScore->getGlobalBounds().height / 2, sf::Color::White, sf::Color::Black, true);

	m_rect.setSize(sf::Vector2f(getScreenWidth(), getScreenHeight()));
	sf::Color rectColor(0, 0, 0, 164);
	m_rect.setFillColor(rectColor);

	for (int i = 0; i < END_GAME_OPTIONS; i++)
		m_selected[i] = false;
}

EndGameState::~EndGameState()
{
	delete m_title;
	for (int i = 0; i < END_GAME_OPTIONS; i++)
		delete m_options[i];
	delete m_winnerText;
	delete m_winnerScore;
}

void EndGameState::show(sf::Sprite& background, Game& m_game)
{
	m_window->setMouseCursorVisible(true);
	m_background = background;

	if (*(m_game.getPlayer1()->tank.get_health()) < 0 || *(m_game.getPlayer2()->tank.get_health()) < 0)
	{
		if (*(m_game.getPlayer1()->tank.get_score()) > * (m_game.getPlayer1()->tank.get_score()))
		{
			m_winnerText->setString("WINNER : PLAYER 1");
			m_winnerScore->setString("SCORE : " + std::to_string(static_cast<int>(*(m_game.getPlayer1()->tank.get_score()))));
		}
		else
		{
			m_winnerText->setString("WINNER : PLAYER 2");
			m_winnerScore->setString("SCORE : " + std::to_string(static_cast<int>(*(m_game.getPlayer2()->tank.get_score()))));
		}
	}

	while (!m_restartGame && !m_quitGame && m_window->isOpen())
	{
		float elapsedTime = m_clock->restart().asSeconds();

		input();
		update(elapsedTime);
		draw();
	}
	if (!m_quitGame)
		restart();
}

void EndGameState::restart()
{
	m_window->setMouseCursorVisible(false);
	m_restartGame = false;
}

void EndGameState::input()
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
			}
			break;
		default:
			break;
		}
	}
}

void EndGameState::update(float elapsedTime)
{
	for (int i = 0; i < END_GAME_OPTIONS; i++)
	{
		if (m_options[i]->getGlobalBounds().contains(m_mouseX, m_mouseY))
		{
			if (!m_selected[i])
			{
				m_select.play();
				m_selected[i] = true;
				m_options[i]->setFillColor(TEXT_COLOR_RED);
			}
		}
		else
		{
			if (m_selected[i])
			{
				m_selected[i] = false;
				m_options[i]->setFillColor(sf::Color::White);
			}
		}
	}

	if (m_clicked)
	{
		if (m_selected[0])
		{
			m_resume.play();
			m_restartGame = true;
		}
		else
		{
			if (m_selected[1])
			{
				m_click.play();
				m_quitGame = true;
			}
		}
		m_clicked = false;
	}
}

void EndGameState::draw() const
{
	m_window->clear();

	m_window->draw(m_background);
	m_window->draw(m_rect);
	m_window->draw(*m_title);

	for (int i = 0; i < END_GAME_OPTIONS; i++)
		m_window->draw(*m_options[i]);

	m_window->draw(*m_winnerText);
	m_window->draw(*m_winnerScore);

	m_window->display();
}
