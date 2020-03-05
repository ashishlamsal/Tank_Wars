#include "SFML/Graphics.hpp"
#include "SFML/Window/Cursor.hpp"

#include "MenuState.h"
#include "GameState.h"
#include "EndGameState.h"
#include "SplashState.h"
#include "Definitions.h"
#include "ResourceManager.h"

int main()
{
	
	sf::RenderWindow window(sf::VideoMode(DEFAULT_WIDTH, DEFAULT_HEIGHT), GAME_TITLE, sf::Style::Default);

	window.setFramerateLimit(FRAME_RATE);
	State::setScreenDimensions(window.getSize());
	State::setScaleFactor(sf::Vector2f((float)window.getSize().x / DEFAULT_WIDTH, (float)window.getSize().y / DEFAULT_HEIGHT));


	musicManager musics;
	textureManager textures;
	fontManager fonts;
	soundbufferManager sounds;
	imageManager images;


	if (!(textures.load(textureIdentifier::splash_logo, SPLASH_IMAGE_PATH) &&
		textures.load(textureIdentifier::game_background, GAME_BACKGROUND) &&
		textures.load(textureIdentifier::tank_body, TANK_BODY) &&
		textures.load(textureIdentifier::tank_body_flipped, TANK_BODY_FLIPPED) &&
		textures.load(textureIdentifier::tank_top, TANK_TOP) &&
		textures.load(textureIdentifier::tank_top_flipped, TANK_TOP_FLIPPED) &&
		textures.load(textureIdentifier::tank_wheels, TANK_WHEELS) &&
		textures.load(textureIdentifier::arrow, TANK_ARROW)&&
		textures.load(textureIdentifier::tank_barrel, TANK_BARREL)&&
		textures.load(textureIdentifier::hud_background, HUD_BACKGROUND_IMAGE_PATH)&&
		textures.load(textureIdentifier::hud_playbutton, HUD_PLAY_BUTTON_PATH)&&
		textures.load(textureIdentifier::hud_leftclick, HUD_LEFT_CLICK_PATH)&&
		textures.load(textureIdentifier::hud_rightclick, HUD_RIGHT_CLICK_PATH)&&
		textures.load(textureIdentifier::hud_powerbar, HUD_POWER_BAR_PATH)&&
		textures.load(textureIdentifier::hud_anglebutton, HUD_ANGLE_BUTTON_PATH)
		
		)) {
		std::cout << "textures not loaded" << std::endl;
		throw std::runtime_error("failed to load textures");
	}
	if (!(fonts.load(fontIdentifier::game_font, GAME_FONT_PATH)&&
		fonts.load(fontIdentifier::settings_font,SETTINGS_OPTIONS_FONT_PATH)

		)) {
		std::cout << "fonts not loaded" << std::endl;
		throw std::runtime_error("failed to load fonts");
	}
	if (!(sounds.load(bufferIdentifier::paus_state_sound, PAUSE_STATE_SOUND_PATH) &&
		sounds.load(bufferIdentifier::resume_sound, RESUME_SOUND_PATH) &&
		sounds.load(bufferIdentifier::select_sound, SELECT_SOUND_PATH)&&
		sounds.load(bufferIdentifier::click_sound, CLICK_SOUND_PATH)
		)) {
		std::cout << "sounds not load" << std::endl;
		throw std::runtime_error("failed to load sounds");
	}
	if (!(images.load(imageIdentifier::cursor, CURSOR_IMAGE_PATH) &&
		images.load(imageIdentifier::window_icon, ICON_IMAGE_PATH)
		)) {
		std::cout << "images not loaded" << std::endl;
		throw std::runtime_error("failed to load images");
	}
	if (!(musics.load(musicIdentifier::gamestate_music, GAME_STATE_MUSIC_PATH) &&
		musics.load(musicIdentifier::menustate_music, MENU_STATE_MUSIC_PATH)
		)) {
		std::cout << "music not loaded" << std::endl;
		throw std::runtime_error("failed to load music");
	}


	sf::Image Icon=images.get(imageIdentifier::window_icon);
	sf::Image CursorImage=images.get(imageIdentifier::cursor);;
	sf::Cursor cursor;
	

	window.setMouseCursorVisible(false);
	cursor.loadFromPixels(CursorImage.getPixelsPtr(), CursorImage.getSize(), sf::Vector2u(2, 2));
	window.setIcon(Icon.getSize().x, Icon.getSize().y, Icon.getPixelsPtr());
	window.setMouseCursor(cursor);


	SplashState* splash = new SplashState(window, textures,sounds);
	MenuState* menu = new MenuState(window,fonts,sounds);
	GameState* game = new GameState(window,fonts,textures,sounds);

	splash->start();

	while (window.isOpen())
	{

		if (!game->gameOver()){
			menu->show();
		}


		if (menu->startGame()){
			menu->setStartGame(false);
			game->restart();
		}


	}

	delete game;
	delete splash;
	delete menu;

	return 0;
}