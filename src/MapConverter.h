#pragma once
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Global.h"
std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch);
//ghost and pacman work remaining