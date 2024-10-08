#pragma once
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Global.h"

bool collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map);