#include "GameState.h"
#include "PauseState.h"
#include "EndGameState.h"
#include "HUD.h"

GameState::GameState(sf::RenderWindow& window, fontManager& fonts, textureManager& textures, soundbufferManager& sounds) : State(window,sounds), m_gameOver(false), m_time(GAME_TIME), m_timeSinceLastFrame(0)
{
	m_hud = new HUD(window, fonts, textures); //HUD
	m_pauseState = new PauseState(*m_window,fonts,sounds);
	m_endGameState = new EndGameState(*m_window,fonts,sounds);

	m_mainTheme.openFromFile(GAME_STATE_MUSIC_PATH);
	m_mainTheme.setLoop(true);
	m_mainTheme.setVolume(30);

	m_pauseBuffer=sounds.get(bufferIdentifier::paus_state_sound);
	m_pause.setBuffer(m_pauseBuffer);

	
	m_game = new Game(window,textures);

}

GameState::~GameState()
{
	delete m_hud; //HUD
	delete m_pauseState;
	delete m_endGameState;
}

void GameState::run()
{
	if (m_mainTheme.getStatus() == sf::SoundSource::Status::Stopped)
		m_mainTheme.play();

	while (!m_gameOver && !m_pauseState->quitGame() && m_window->isOpen())
	{
		float elapsedTime = m_clock->restart().asSeconds();
		input(*m_hud);
		if (!m_pauseState->quitGame() && m_window->isOpen())
		{
			update(elapsedTime);			
			draw();
			
		}
	}

	if (!m_pauseState->quitGame() && m_window->isOpen())
	{
		result();
		if (!m_endGameState->quitGame())
			restart();
		else
		{
			m_endGameState->setQuitGame(false);
			m_gameOver = false;
		}
	}
	else
	{
		m_mainTheme.stop();
		m_pauseState->setQuitGame(false);
	}  
		
}

void GameState::input(HUD& h)
{
	sf::Event evnt;
	if (m_window->pollEvent(evnt))
	{
		if (!m_gameOver)
		{
			bool wasPaused = false;
			switch (evnt.type)
			{
			case sf::Event::Closed:
				m_window->close();
				break;
			case sf::Event::KeyPressed:
				switch (evnt.key.code) 
				{
				case sf::Keyboard::Escape:
					pause(wasPaused);
					break;
				
				case sf::Keyboard::A:
					if (m_game->selected_player->state == Player_state::idle) {
						m_game->selected_player->state = Player_state::move_left;
						m_game->set_game_timing();
					}
					break;

				case sf::Keyboard::D:
					if (m_game->selected_player->state == Player_state::idle) {
						m_game->selected_player->state = Player_state::move_right;
						m_game->set_game_timing();
					}
					break;

				case sf::Keyboard::R:
					m_game->respawn_selected_player();
					break;

				case sf::Keyboard::F:
					m_game->fire_selected_player();
					break;

				case sf::Keyboard::Q:
					m_game->change_selected_player();
					break;

				case sf::Keyboard::Num1:
					if (!m_game->selected_player->armory.empty()) {
						for (int i = 0; i < m_game->selected_player->armory.size(); i++) {
							if (m_game->selected_player->armory[i] == Weapon::single_shot) {
								m_game->selected_player->selected_weapon = &m_game->selected_player->armory[i];
							}
						}						
					}
					break;

				case sf::Keyboard::Num2:
					if (!m_game->selected_player->armory.empty()) {
						for (int i = 0; i < m_game->selected_player->armory.size(); i++) {
							if (m_game->selected_player->armory[i] == Weapon::triple_shot) {
								m_game->selected_player->selected_weapon = &m_game->selected_player->armory[i];
							}
						}
					}
					break;

				case sf::Keyboard::Num3:
					if (!m_game->selected_player->armory.empty()) {
						for (int i = 0; i < m_game->selected_player->armory.size(); i++) {
							if (m_game->selected_player->armory[i] == Weapon::penta_shot) {
								m_game->selected_player->selected_weapon = &m_game->selected_player->armory[i];
							}
						}
					}
					break;

				case sf::Keyboard::Num4:
					if (!m_game->selected_player->armory.empty()) {
						for (int i = 0; i < m_game->selected_player->armory.size(); i++) {
							if (m_game->selected_player->armory[i] == Weapon::total_destroy) {
								m_game->selected_player->selected_weapon = &m_game->selected_player->armory[i];
							}
						}
					}
					break;

				case sf::Keyboard::Num5:
					if (!m_game->selected_player->armory.empty()) {
						for (int i = 0; i < m_game->selected_player->armory.size(); i++) {
							if (m_game->selected_player->armory[i] == Weapon::scatter_shot) {
								m_game->selected_player->selected_weapon = &m_game->selected_player->armory[i];
							}
						}
					}
					break;
				}
				break;

			case sf::Event::MouseMoved:
				h.m_mouseX = evnt.mouseMove.x;
				h.m_mouseY = evnt.mouseMove.y;
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					h.m_mouseX = evnt.mouseButton.x;
					h.m_mouseY = evnt.mouseButton.y;
					h.m_clicked = true;
					if (h.mouse_captured_for_angle)
					{
						h.mouse_captured_for_angle = false;
					}
				}break;

			case sf::Event::MouseWheelScrolled:
				if (evnt.mouseWheelScroll.delta == 1)
					m_game->selected_player->set_power(m_game->selected_player->get_power() + 3);
				else
					m_game->selected_player->set_power(m_game->selected_player->get_power() - 3);

				break;
			default:
				break;
			}

			if (h.mouse_captured_for_angle) {
				sf::Vector2i mouse_pos = sf::Mouse::getPosition(*m_window);
				m_game->selected_player->tank.sfml_tank->point_to((sf::Vector2f) mouse_pos);
			}
			if (wasPaused && !m_pauseState->quitGame())
			{
				m_mainTheme.play();
				m_clock->restart();
			}
		}
	}
}

void GameState::update(float elapsedTime)
{
	m_hud->update(Controls, elapsedTime, *m_game);
	m_hud->update(HP, elapsedTime, *m_game);
	m_hud->update(Score, elapsedTime, *m_game);
	m_hud->update(Angle, elapsedTime, *m_game);
	m_hud->update(Power, elapsedTime, *m_game);
	m_hud->update(Weapons, elapsedTime, *m_game);

	m_time += elapsedTime;
	m_timeSinceLastFrame += elapsedTime;
	if (m_timeSinceLastFrame >= 1)
	{

		m_hud->update(TimePassed, m_time, *m_game);
		m_timeSinceLastFrame = 0;
	}
	if (*(m_game->getPlayer1()->tank.get_health()) < 0 || *(m_game->getPlayer2()->tank.get_health()) < 0)
		m_gameOver = true;

	m_game->make(elapsedTime);

}

void GameState::draw() const
{
	m_window->clear(sf::Color::Black);
	m_game->draw(*m_window);
	m_hud->draw(*m_window);
	m_window->display();
}

void GameState::pause(bool& wasPaused)
{
	sf::Texture backgroundTexture;
	sf::Sprite background;

	backgroundTexture.create(m_window->getSize().x, m_window->getSize().y);
	backgroundTexture.update(*m_window);
	background.setTexture(backgroundTexture);

	wasPaused = true;
	m_mainTheme.pause();
	m_pause.play();
	m_pauseState->show(background);
}

void GameState::result()
{
	sf::Texture backgroundTexture;
	sf::Sprite background;

	backgroundTexture.create(m_window->getSize().x, m_window->getSize().y);
	backgroundTexture.update(*m_window);
	background.setTexture(backgroundTexture);

	m_mainTheme.stop();
	m_endGameState->show(background, *m_game);
}

void GameState::restart()
{
	m_gameOver = false;
	m_time = GAME_TIME;
	m_timeSinceLastFrame = 0;
	m_clock->restart();
	run();
}