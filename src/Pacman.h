#pragma once
#include <cmath>
#include <array>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Global.h"

class Pacman{
    bool animation_over;
    bool dead;
    unsigned char direction;
    unsigned char animation_timer;
    unsigned short energizer_timer;

    Position position;
public:
    Pacman();
    bool get_animation_over();
    bool get_dead();

    unsigned char get_direction();
    unsigned short get_energizer_timer();
    unsigned short get_animation_timer();

    void draw(bool i_victory, sf::RenderWindow& i_window);
    void reset();
    void set_animation_timer(unsigned short i_animation_timer, unsigned short& lives);
    void set_dead(bool i_dead);
    void set_position(short i_x, short i_y);
    void update(unsigned short i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);

    Position get_position();
};