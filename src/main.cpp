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
#include "Text.h"
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
    unsigned short level = 0;
    Pacman pacman;
    GhostManager ghostManager;
    std::array<Position, 4> ghost_positions{};
    std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> map = convert_sketch(map_sketch,ghost_positions,pacman);
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));
    window.setFramerateLimit(60);
    ghostManager.reset(level, ghost_positions);
    bool move = 0;
    int lives = 3;
    while(window.isOpen()){
        sf::Event event;

        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (move == 0){
            map = convert_sketch(map_sketch,ghost_positions,pacman);
            pacman.reset();
            ghostManager.reset(level, ghost_positions);

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
                move = 1;
            }
        }
        window.clear();
        draw_text(0, 0, 335, "Lives: " + std::to_string(lives) + ", Level: " + std::to_string(level), window);        //*************TO DO: make pacman smoother on turns********************
        //*** GAME LOGIC ***
        //if game not won and pacman dead {level remain the same ; lives--)
        //if game won and pacman not dead {level ++;}
        //else just keep playing
        if(move){
            if (!game_won && pacman.get_dead()) {
                // Reset the animation timer only when Pac-Man first dies
                if (pacman.get_animation_over() == 0 && pacman.get_animation_timer() == 0) {
                    pacman.set_animation_timer(1); // Start the death animation
                }
                // Draw the death animation
                draw_text(1,250,250,"GAME OVER",window);

                pacman.draw(1, window);
                // Check if the animation has completed
                if (pacman.get_animation_over()) {
                    // Animation has finished, now stop the game logic and reset
                    move = 0;
                }

            } else if (game_won && !pacman.get_dead()) {
                if (pacman.get_animation_over() == 0 && pacman.get_animation_timer() == 0) {
                    pacman.set_animation_timer(1);
                }
                draw_map(map, window);
                pacman.draw(1, window);
                draw_text(1,250,250,"YOU WON",window);
                if (pacman.get_animation_over()) {
                    move = 0;
                }
            } else if (!game_won && !pacman.get_dead()) {
                draw_map(map, window);
                pacman.draw(game_won, window);
                ghostManager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);

                pacman.update(level, map);
                ghostManager.update(level, map, pacman);
                //how to win the game
                for (const std::array<Cell, MAP_HEIGHT>& column : map){
                    for (const Cell& cell : column){
                        game_won = 1;
                        if (Cell::Pellet == cell){
                            game_won = 0; //The game is not yet won.
                            break;
                        }
                    }
                    if (0 == game_won){
                        break;
                    }
                }
            }
        }
        else{ //This is for when we are waiting for player to press enter and start playing
            draw_map(map, window);

            pacman.draw(0,window);
            ghostManager.draw(0,window);
            pacman.update(level,map);
            ghostManager.update(level,map,pacman);

        }
        window.display();
    }
    return 0;
}