#include "MapConverter.h"
#include "Global.h"
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> convert_sketch(const std::array<std::string, MAP_HEIGHT>& i_map_sketch, Pacman& i_pacman){
    //pacman and ghost work remaining
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> output_map{};

    for(int a=0; a<MAP_HEIGHT; a++){
        for(int b=0; b<MAP_WIDTH; b++) {
            output_map[b][a] = Cell::Empty;
            //by default every cell should be empty

            switch(i_map_sketch[a][b]){
                case '#':{
                    output_map[b][a]=Cell::Wall;
                    break;
                }
                case '=':{
                    output_map[b][a]=Cell::Door;
                    break;
                }
                case '.':{
                    output_map[b][a]=Cell::Pellet;
                    break;
                }
                case 'o':{
                    output_map[b][a]=Cell::Energizer;
                    break;
                }
                case 'P':{
                    i_pacman.set_position(CELL_SIZE * b, CELL_SIZE * a);
                    break;
                }
                default:{
                    continue;
                }
            }
        }
    }
    return output_map;
}