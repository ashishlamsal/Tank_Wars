#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <string>
#include "ResourceManager.h"

class State {
protected:
	sf::RenderWindow* m_window;
	sf::Clock* m_clock;

	static sf::Vector2u m_screenDimensions;
	static sf::Vector2f m_scaleFactor;

	virtual void input() = 0;
	virtual void update(float elapsedTime) = 0;
	virtual void draw() const = 0;

public:
	State(sf::RenderWindow& window,soundbufferManager &sounds);
	~State();
	static int getScreenWidth() { return m_screenDimensions.x; }
	static int getScreenHeight() { return m_screenDimensions.y; }
	static sf::Vector2f getScaleFactors() { return m_scaleFactor; }
	static void setScreenDimensions(sf::Vector2u ScreenDimensions)
	{
		m_screenDimensions = ScreenDimensions;
	}
	static void setScaleFactor(sf::Vector2f ScaleFactor)
	{
		m_scaleFactor = ScaleFactor;
	}

protected:
	sf::SoundBuffer m_selectBuffer;
	sf::SoundBuffer m_clickBuffer;
	sf::SoundBuffer m_resumeBuffer;

	sf::Sound m_select;
	sf::Sound m_click;
	sf::Sound m_resume;
};



