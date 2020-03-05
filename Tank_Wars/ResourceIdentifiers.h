#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include<SFML/Audio.hpp>


enum class textureIdentifier
{
    splash_logo,

    game_background,
    tank_body,
    tank_body_flipped,
    tank_top,
    tank_top_flipped,
    tank_barrel,
    tank_wheels,
    arrow,

    hud_background,
    hud_playbutton,
    hud_leftclick,
    hud_rightclick,
    hud_anglebutton,
    hud_powerbar


};

enum class fontIdentifier
{
    game_font,
    settings_font
};

enum class bufferIdentifier {

    paus_state_sound,
    select_sound,
    resume_sound,
    clock_tick_sound,
    click_sound
};

enum class musicIdentifier {
    menustate_music,
    gamestate_music


};

enum class imageIdentifier
{

    window_icon,
    cursor

};

template <typename Resource, typename Identifier>
class ResourceManager;
class musicManager;
typedef ResourceManager<sf::Texture,textureIdentifier> textureManager;
typedef ResourceManager<sf::Font,fontIdentifier> fontManager;
typedef ResourceManager<sf::SoundBuffer,bufferIdentifier> soundbufferManager;
typedef ResourceManager<sf::Image,imageIdentifier> imageManager;
