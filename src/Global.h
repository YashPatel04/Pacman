#pragma once
constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;
constexpr unsigned char CELL_SIZE = 16;
constexpr unsigned char FONT_HEIGHT = 16;
constexpr unsigned char SCREEN_RESIZE = 2;
constexpr unsigned char PACMAN_ANIMATION_FRAMES = 6;
constexpr unsigned char PACMAN_ANIMATION_SPEED = 4;
constexpr unsigned char PACMAN_SPEED = 2;
constexpr unsigned short ENERGIZER_DURATION = 512;

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