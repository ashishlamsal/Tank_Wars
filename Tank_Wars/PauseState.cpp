#include "PauseState.h"
#include "utility.h"

PauseState::PauseState(sf::RenderWindow& window, fontManager& fonts, soundbufferManager& sounds) : State(window, sounds), m_mouseX(0), m_mouseY(0), m_clicked(false), m_resumeGame(false), m_quitGame(false)
{
	m_font=fonts.get(fontIdentifier::game_font);

	m_title = new sf::Text("Paused", m_font, PAUSED_TEXT_SIZE * getScaleFactors().x);
	m_options[0] = new sf::Text("Resume", m_font, PAUSED_OPTIONS_TEXT_SIZE * getScaleFactors().x);
	m_options[1] = new sf::Text("Quit", m_font, PAUSED_OPTIONS_TEXT_SIZE * getScaleFactors().x);

	formatText(m_title, getScreenWidth() / 2 - m_title->getGlobalBounds().width / 2, getScreenHeight() / 4 - m_title->getGlobalBounds().height / 2, TEXT_COLOR_YELLOW, sf::Color::White, true);
	for (int i = 0; i < PAUSE_OPTIONS; i++)
	{
		formatText(m_options[i], getScreenWidth() / 2 - m_options[i]->getGlobalBounds().width / 2, getScreenHeight() / 2 - m_options[0]->getGlobalBounds().height / 2 + 128 * i * getScaleFactors().x, sf::Color::White);
	}

	m_rect.setSize(sf::Vector2f(getScreenWidth() / 3, getScreenHeight() * 3 / 4));
	sf::Color rectColor(0, 0, 0, 164);
	m_rect.setFillColor(rectColor);
	m_rect.setPosition(getScreenWidth() / 2 - m_rect.getGlobalBounds().width / 2, getScreenHeight() / 2 - m_rect.getGlobalBounds().height / 2);
	m_rect.setOutlineColor(TEXT_COLOR_WHITE);
	m_rect.setOutlineThickness(15.0f);

	for (int i = 0; i < PAUSE_OPTIONS; i++)
		m_selected[i] = false;
}

PauseState::~PauseState()
{
	delete m_title;
	for (int i = 0; i < PAUSE_OPTIONS; i++)
		delete m_options[i];
}

void PauseState::show(sf::Sprite& background)
{
	m_window->setMouseCursorVisible(true);
	m_background = background;

	while (!m_resumeGame && !m_quitGame && m_window->isOpen())
	{
		float elapsedTime = m_clock->restart().asSeconds();
		input();
		update(elapsedTime);
		draw();
	}
	if (!m_quitGame)
		resume();
}

void PauseState::resume()
{
	m_window->setMouseCursorVisible(true);
	m_resume.play();
	m_resumeGame = false;
}

void PauseState::input()
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
		case sf::Event::KeyPressed:
			if (evnt.key.code == sf::Keyboard::Escape)
				m_resumeGame = true;
			break;
		default:
			break;
		}
	}
}

void PauseState::update(float elapsedTime)
{
	for (int i = 0; i < PAUSE_OPTIONS; i++)
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
			m_resumeGame = true;
		else
			if (m_selected[1])
			{
				m_click.play();
				m_quitGame = true;
			}
		m_clicked = false;
	}
}

void PauseState::draw() const
{
	m_window->clear();

	m_window->draw(m_background);
	m_window->draw(m_rect);

	m_window->draw(*m_title);
	for (int i = 0; i < PAUSE_OPTIONS; i++)
		m_window->draw(*m_options[i]);

	m_window->display();
}




