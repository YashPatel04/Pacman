//
// Created by Yash on 04-10-2024.
//
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "MapConverter.h"
#include "Global.h"
#include "collision.h"
#include "Pacman.h"
#include "Ghost.h"
using namespace std;

int main(){
    const std::array<std::string, MAP_HEIGHT> map_sketch = {
            " ################### ",
            " #........#........# ",
            " #o##.###.#.###.##o# ",
            " #.................# ",
            " #.##.#.#####.#.##.# ",
            " #....#...#...#....# ",
            " ####.### # ###.#### ",
            "    #.#   0   #.#    ",
            "#####.# ##=## #.#####",
            "     .  #123#  .     ",
            "#####.# ##### #.#####",
            "    #.#       #.#    ",
            " ####.# ##### #.#### ",
            " #........#........# ",
            " #.##.###.#.###.##.# ",
            " #o.#.....P.....#.o# ",
            " ##.#.#.#####.#.#.## ",
            " #....#...#...#....# ",
            " #.######.#.######.# ",
            " #.................# ",
            " ################### "
    };
    Pacman pacman;
    //remove 👇👇👇
    Ghost g1(0);
    //remove 👆👆👆
    std::array<Position, 4> ghost_positions{};
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map = convert_sketch(map_sketch,ghost_positions,pacman);
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    window.setFramerateLimit(60);
    bool move = 0;
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::KeyPressed){
                move = 1;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        draw_map(map, window);
        //*************TO DO: make pacman smoother on turns********************
        if(move){pacman.update(0, map);}
        pacman.draw(0, window);
        //remove 👇👇👇
        g1.draw(0,window);
        g1.update(0,map,g1,pacman);
        //remove 👆👆👆
        window.display();
    }
    return 0;
}