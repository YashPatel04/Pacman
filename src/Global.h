#pragma once
constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;
constexpr unsigned char CELL_SIZE = 16;
constexpr unsigned char FONT_HEIGHT = 16;
constexpr unsigned char SCREEN_RESIZE = 2;
constexpr unsigned char PACMAN_ANIMATION_FRAMES = 6;
constexpr unsigned char PACMAN_ANIMATION_SPEED = 3;
constexpr unsigned char PACMAN_SPEED = 2;
constexpr unsigned short ENERGIZER_DURATION = 512;
constexpr unsigned char GHOST_ANIMATION_FRAMES = 6;
constexpr unsigned char GHOST_ANIMATION_SPEED = 4;
constexpr unsigned char GHOST_ESCAPE_SPEED = 4;
constexpr unsigned char GHOST_FRIGHTENED_SPEED = 3;
constexpr unsigned char GHOST_SPEED = 1;
//Which cell in front of Pacman the pink ghost will target.
constexpr unsigned char GHOST_1_CHASE = 2;
//Which cell in front of Pacman the blue ghost will use to calculate the target.
constexpr unsigned char GHOST_2_CHASE = 1;
//If the distance between the orange ghost and Pacman is less than this value, the orange ghost will go into the scatter mode.
constexpr unsigned char GHOST_3_CHASE = 4;



enum Cell
{
    Door,
    Empty,
    Energizer,
    Pellet,
    Wall
};
struct Position
{
    short x;
    short y;

    //See? I'm an expert.
    bool operator==(const Position& i_position)
    {
        return this->x == i_position.x && this->y == i_position.y;
    }
};