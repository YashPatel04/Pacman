#include "Map.h"
#include "Global.h"
#include <array>
#include <SFML/Graphics.hpp>
#include <iostream>
void draw_map(const std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>& i_map, sf::RenderWindow& i_window){
    sf::Sprite sprite;
    sf::Texture texture;
    texture.loadFromFile("../assets/Map16.png");
    //The below line is very important-do not forget!
    sprite.setTexture(texture);

    for(int a=0; a<MAP_WIDTH; a++){
        for(int b=0; b<MAP_HEIGHT; b++){
            // since we are going through nested for loops below line sets the position of each sprite,
            // almost like laying tiles
            sprite.setPosition(static_cast<float>(CELL_SIZE*a), static_cast<float>(CELL_SIZE * b));

            // now just crop what we need from the texture
            switch (i_map[a][b]){
                case Cell::Door:{
                    sprite.setTextureRect(sf::IntRect(2*CELL_SIZE,CELL_SIZE,CELL_SIZE,CELL_SIZE));
                    i_window.draw(sprite);
                    break;
                }
                case Cell::Energizer:{
                    sprite.setTextureRect(sf::IntRect(CELL_SIZE,CELL_SIZE,CELL_SIZE,CELL_SIZE));
                    i_window.draw(sprite);
                    break;
                }
                case Cell::Pellet:{
                    sprite.setTextureRect(sf::IntRect (0,CELL_SIZE,CELL_SIZE,CELL_SIZE));
                    i_window.draw(sprite);
                    break;
                }
                case Cell::Wall: {
                    //std::cout<<"wall here"<<std::endl;
                    bool down = 0;
                    bool left = 0;
                    bool right = 0;
                    bool up = 0;
                    if (b < MAP_HEIGHT - 1) {
                        if (Cell::Wall == i_map[a][1 + b]) {
                            down = 1;
                        }
                    }
                    //Since we have warp tunnels, we need to draw them as if they're connected.
                    if (0 < a) {
                        if (Cell::Wall == i_map[a - 1][b]) {
                            left = 1;
                        }
                    } else {
                        left = 1;
                    }
                    if (a < MAP_WIDTH - 1) {
                        if (Cell::Wall == i_map[1 + a][b]) {
                            right = 1;
                        }
                    } else {
                        right = 1;
                    }
                    if (0 < b) {
                        if (Cell::Wall == i_map[a][b - 1]) {
                            up = 1;
                        }
                    }
                    //--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
                    sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

                    i_window.draw(sprite);
                }
                    default:{
                        continue;
                    }
            }
        }
    }
}
