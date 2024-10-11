#include <array>
#include <cmath>
#include <iostream>
#include "Global.h"
#include "collision.h"

bool collision(bool i_collect_pellets, bool i_use_door, short i_x, short i_y, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map){
    bool output = 0;

    //exact location
    float cell_x = i_x / static_cast<float>(CELL_SIZE);
    float cell_y = i_y / static_cast<float>(CELL_SIZE);

    //a ghost/pacman can intersect 4 cells at most
    for (unsigned char a = 0; a < 4; a++){
        short x = 0;
        short y = 0;
        switch(a){
            case 0: {//Top left cell
                x = static_cast<short>(floor(cell_x));
                y = static_cast<short>(floor(cell_y));
                break;
            }
            case 1: {//Top right cell
                x = static_cast<short>(ceil(cell_x));
                y = static_cast<short>(floor(cell_y));
                break;
            }
            case 2: {//Bottom left cell
                x = static_cast<short>(floor(cell_x));
                y = static_cast<short>(ceil(cell_y));
                break;
            }
            case 3: {//Bottom right cell
                x = static_cast<short>(ceil(cell_x));
                y = static_cast<short>(ceil(cell_y));
            }
        }
        //make sure that the position is inside of the map
        if(0<=x && 0<=y && MAP_HEIGHT>y && MAP_WIDTH > x){
            //We are just checking for walls here
            if(0==i_collect_pellets){
                if(Cell::Wall == i_map[x][y]){
                    output = 1;
                }
                else if(!i_use_door && Cell::Door == i_map[x][y]){
                    output = 1;
                }
            }
            else{ //Now only care about collectable
                if(Cell::Energizer == i_map[x][y]){
                    output = 1;
                    i_map[x][y] = Cell::Empty;
                }
                else if(Cell::Pellet == i_map[x][y]){
                    i_map[x][y] = Cell::Empty;
                }
            }
        }
    }
    return output;
}
