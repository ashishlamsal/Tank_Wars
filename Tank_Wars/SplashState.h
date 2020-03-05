#pragma once
#include "state.h"
#include "Definitions.h"
#include "ResourceManager.h"


class SplashState : public State {
private:
	sf::Texture m_logoTexture;
	sf::Sprite m_logo;
	float m_onScreenTime;
	float m_fadeTime;
	bool m_start;

protected:
	void input();
	void update(float elapsedTime);
	void draw() const;

public:
	SplashState(sf::RenderWindow& window, textureManager &textures, soundbufferManager& sounds);
	~SplashState();
	
	void start();
};



