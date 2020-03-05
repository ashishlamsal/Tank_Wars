#include "state.h"
#include "Definitions.h"

sf::Vector2u State::m_screenDimensions = sf::Vector2u(0, 0);
sf::Vector2f State::m_scaleFactor = sf::Vector2f(0, 0);

State::State(sf::RenderWindow& window, soundbufferManager& sounds)
{
	m_window = &window;
	m_clock = new sf::Clock();

	m_selectBuffer=sounds.get(bufferIdentifier::select_sound);
	m_clickBuffer = sounds.get(bufferIdentifier::click_sound);
	m_resumeBuffer=sounds.get(bufferIdentifier::resume_sound);

	m_select.setBuffer(m_selectBuffer);
	m_click.setBuffer(m_clickBuffer);
	m_resume.setBuffer(m_resumeBuffer);

}

State::~State()
{
	delete m_clock;
}