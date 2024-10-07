//
// Created by Yash on 04-10-2024.
//
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
using namespace std;

int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Pac-Man");

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            window.clear();
            //Game objects here

            window.display();
        }

    }
    return 0;
}