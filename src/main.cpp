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
#include "GhostManager.h"
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
    bool game_won = 0;
    bool set_dead = 0;
    Pacman pacman;
    GhostManager ghostManager;
    std::array<Position, 4> ghost_positions{};
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map = convert_sketch(map_sketch,ghost_positions,pacman);
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    window.setFramerateLimit(60);
    ghostManager.reset(0, ghost_positions);
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::KeyPressed){
                set_dead = 1;
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        draw_map(map, window);
        //*************TO DO: make pacman smoother on turns********************
        if(set_dead){
            pacman.update(0, map);
            ghostManager.update(0,map,pacman);
        }
        pacman.draw(0, window);
        ghostManager.draw(0,window);

        window.display();
    }
    return 0;
}