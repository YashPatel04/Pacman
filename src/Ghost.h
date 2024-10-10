#pragma once
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Global.h"
#include "Pacman.h"

class Ghost {
    bool movement_mode;
    //it can be scatter or chase mode
    bool use_door;
    //to get them out of their room once the game starts (again)

    unsigned char direction;
    //Current direction
    unsigned char frightened_mode;
    // 0-Fearless
    // 1-Lil bit frightened
    // 2-Running straight back to house
    unsigned char frightened_speed_timer;
    unsigned char id;
    // 0-Red
    // 1-Pink
    // 2-Blue
    // 3-Orange

    unsigned short animation_timer;

    Position home;
    //ghost will go here when escaping
    Position home_exit;
    //You can't stay in your house forever
    Position position;
    //current position
    Position target;
    //Current target
public:
    Ghost(unsigned char i_id);

    bool pacman_collision(const Position &i_pacman_position);

    float get_target_distance(unsigned char i_direction);

    void draw(bool i_flash, sf::RenderWindow &i_window);

    void reset(const Position &i_home, const Position &i_home_exit);

    void set_position(short i_x, short i_y);

    void switch_mode();

    void update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> &i_map, Ghost &i_ghost_0,
                Pacman &i_pacman);

    void update_target(unsigned char i_pacman_direction, const Position &i_ghost_0_position,
                       const Position &i_pacman_position);
    Position get_position();

};