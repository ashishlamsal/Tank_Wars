#include "SplashState.h"

SplashState::SplashState(sf::RenderWindow& window, textureManager& textures, soundbufferManager& sounds) : State(window,sounds),
m_onScreenTime(0), m_fadeTime(0), m_start(false)
{
	m_logoTexture=textures.get(textureIdentifier::splash_logo);
	m_logoTexture.setSmooth(true);
	m_logo.setTexture(m_logoTexture);
	m_logo.setOrigin(m_logo.getGlobalBounds().width / 2, m_logo.getGlobalBounds().height / 2);
	m_logo.scale(getScaleFactors());
	m_logo.setPosition(getScreenWidth() / 2, getScreenHeight() / 2);
}

SplashState::~SplashState() {}

void SplashState::start()
{
	while (m_window->isOpen() && !m_start)
	{
		float elapsedTime = m_clock->restart().asSeconds();

		input();
		update(elapsedTime);
		draw();
	}
}

void SplashState::input()
{
	sf::Event evnt;

	if (m_window->pollEvent(evnt))
		switch (evnt.type)
		{
		case sf::Event::Closed:
			m_window->close();
			break;
		default:
			break;
		}
}

void SplashState::update(float elapsedTime)
{
	m_onScreenTime += elapsedTime;
	if (m_onScreenTime < SPLASH_STATE_DURATION / 3)
		m_fadeTime += elapsedTime;
	else
		if (m_onScreenTime > SPLASH_STATE_DURATION * 2 / 3)
			m_fadeTime -= elapsedTime;

	float alphaMultiplier = std::max(0.0f, std::min(m_fadeTime / (SPLASH_STATE_DURATION / 3), SPLASH_STATE_DURATION / 3));
	sf::Color newColor(255, 255, 255, 255 * alphaMultiplier);
	m_logo.setColor(newColor);

	if (m_onScreenTime >= SPLASH_STATE_DURATION)
		m_start = true;
}

void SplashState::draw() const
{
	m_window->clear(sf::Color::White);
	m_window->draw(m_logo);
	m_window->display();
}