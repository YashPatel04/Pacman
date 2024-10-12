#include <iostream>
#include <cmath>
#include <array>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "MapConverter.h"
#include "Global.h"
#include "Pacman.h"
#include "collision.h"
#include "Ghost.h"
#include <limits>
#include <queue>
#include <tuple>

Ghost::Ghost(unsigned char i_id):id(i_id){

}
bool Ghost::pacman_collision(const Position& i_pacman_position){
    //we are checking if the ghost enters pacman's cell
    if(position.x>i_pacman_position.x-CELL_SIZE && position.x<i_pacman_position.x+CELL_SIZE){
        if(position.y>i_pacman_position.y-CELL_SIZE && position.y<i_pacman_position.y+CELL_SIZE){
            return 1;
        }
    }
    return 0;
}
float Ghost::get_target_distance(unsigned char i_direction) {
    short x = position.x;
    short y = position.y;

    //move the ghost imaginarily in a given dist
    switch(i_direction){
        case 0:{
            x+=GHOST_SPEED;
            break;
        }
        case 1:{
            y-=GHOST_SPEED;
            break;
        }
        case 2:{
            x-=GHOST_SPEED;
            break;
        }
        case 3:{
            y+=GHOST_SPEED;
        }
    }
    //Now using the good'ol distance formula
    return static_cast<float>(sqrt(pow(x-target.x,2)+pow(y-target.y,2)));
}
void Ghost::draw(bool i_flash, sf::RenderWindow &i_window) {
    //current frame of the animation
    unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer/static_cast<float>(GHOST_ANIMATION_SPEED)));
    sf::Sprite body;
    sf::Sprite face;

    sf::Texture texture;
    texture.loadFromFile("../assets/Ghost16.png");

    body.setTexture(texture);
    body.setPosition(position.x,position.y);
    body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

    face.setTexture(texture);
    face.setPosition(position.x, position.y);

    // "I'm not frightened" look
    if(0==frightened_mode){
        switch(id){
            case 0:{
                //red
                body.setColor(sf::Color(255,0,0));
                break;
            }
            case 1:{
                //Pink
                body.setColor(sf::Color(255,182,255));
                break;
            }
            case 2:{
                //Blue or Cyan
                body.setColor(sf::Color(0,255,255));
                break;
            }
            case 3:{
                //Orange
                body.setColor(sf::Color(255,182,85));
            }
        }
        face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));
        i_window.draw(body);
    }
    else if(1==frightened_mode){
        //color and face of ghosts remain the same because pacman ate the energizer
        body.setColor(sf::Color(36,36,255));
        face.setTextureRect(sf::IntRect (4*CELL_SIZE,CELL_SIZE,CELL_SIZE,CELL_SIZE));

        if(i_flash == 1 && body_frame%2 == 0){
            body.setColor(sf::Color(255,255,0));
            face.setColor(sf::Color(255,0,0));
        }else{
            body.setColor(sf::Color(36,36,255));
            face.setColor(sf::Color(255,255,255));
        }
        i_window.draw(body);
    }
    else{
        //We only draw the face here
        face.setTextureRect(sf::IntRect (CELL_SIZE*direction,2*CELL_SIZE,CELL_SIZE,CELL_SIZE));

    }
    i_window.draw(face);
    //-------------------<        This is to prevent overflowing.         >-------------------
    animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}
void Ghost::reset(const Position &i_home, const Position &i_home_exit) {
    movement_mode = 0;
    use_door = 0<id;

    direction = 0;
    frightened_mode = 0;
    frightened_speed_timer = 0;

    animation_timer = 0;
    home = i_home;
    home_exit = i_home_exit;
    target = i_home_exit;
}
void Ghost::set_position(short i_x, short i_y) {
    position = {i_x,i_y};
}
void Ghost::switch_mode() {
    movement_mode = 1-movement_mode;
}
void Ghost::update(unsigned char i_level, std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH> &i_map, Ghost &i_ghost_0,
                   Pacman &i_pacman) {
    // can the ghost move?
    bool move = 0;

    unsigned char available_ways = 0;
    unsigned char speed = GHOST_SPEED;

    std::array<bool,4> walls{};

    //Here the ghosts start and stop being frightened
    if(0 == frightened_mode && i_pacman.get_energizer_timer() == ENERGIZER_DURATION/pow(2,i_level)){
        frightened_speed_timer = GHOST_FRIGHTENED_SPEED;
        frightened_mode = 1;
    }
    else if(0 == i_pacman.get_energizer_timer() && 1 == frightened_mode){
        frightened_mode = 0;
    }
    if(2==frightened_mode && 0== position.x % GHOST_ESCAPE_SPEED && 0==position.y%GHOST_ESCAPE_SPEED){
        speed = GHOST_ESCAPE_SPEED;
    }
    update_target(i_pacman.get_direction(),i_ghost_0.get_position(), i_pacman.get_position());

    walls[0] = collision(0,use_door,speed+position.x, position.y,i_map);
    walls[1] = collision(0,use_door,position.x, position.y-speed,i_map);
    walls[2] = collision(0,use_door,position.x - speed, position.y,i_map);
    walls[3] = collision(0,use_door,position.x, speed+position.y,i_map);
    if (1!= frightened_mode){
        // dummy value for the below variable because cant pass in 0
        unsigned char optimal_direction = 4;
        move = 1;
        for(unsigned char a = 0;a<4;a++){
            //Ghosts cannot turn back unless they really have to
            //the below condition does prevents going backwards
            if(a == (2+direction)%4){
                continue;
            }
            else if(0==walls[a]){
                if(4 == optimal_direction){
                    optimal_direction = a;
                }
                available_ways++;

                if(get_target_distance(a) < get_target_distance((optimal_direction))){
                    //optimal direction is the one that is closest to the target
                    optimal_direction = a;
                }
            }
        }
        if(1<available_ways){
            //when ghost is at the intersection it chooses the optimal direction
            direction = optimal_direction;
        }
        else{
            if( 4 == optimal_direction){
                //"Unless they have to" part.In this case the ghost will HAVE TO turn back
                direction = (2 + direction) % 4;
            }else{
                direction = optimal_direction;
            }
        }
    }
    else {
        unsigned char random_direction = rand() % 4;
        if (0 == frightened_speed_timer) {
            //the ghost can move after certain number of frames
            move = 1;
            frightened_speed_timer = GHOST_FRIGHTENED_SPEED;
            for (unsigned char a = 0; a < 4; a++) {
                //they cant turn back even if they are frightened
                if (a == (2 + direction) % 4) {
                    continue;
                } else if (0 == walls[a]) {
                    available_ways++;
                }
            }
            if (available_ways > 0) {
                while (1 == walls[random_direction] || random_direction == (2 + direction) % 4) {
                    //We keep picking a random direction until we can use it and we don't want our ghost to move back, for now atleast
                    random_direction = rand() % 4;
                }
                direction = random_direction;
            } else {
                //If there's no other way, it turns back.
                direction = (2 + direction) % 4;
            }
        } else {
            frightened_speed_timer--;
        }
    }
    //if ghost can move we move it
    if(move == 1){
        switch (direction){
            case 0:{
                position.x += speed;
                break;
            }
            case 1:{
                position.y -= speed;
                break;
            }
            case 2:{
                position.x -= speed;
                break;
            }
            case 3:{
                position.y += speed;
            }
        }
        //warping.
        //when ghost leaves the map we move it to the other side
        if (-CELL_SIZE >= position.x){
            position.x = CELL_SIZE * MAP_WIDTH - speed;
        }
        else if (position.x >= CELL_SIZE * MAP_WIDTH){
            position.x = speed - CELL_SIZE;
        }
    }
    if (1 == pacman_collision(i_pacman.get_position())){
        if (0 == frightened_mode){//When the ghost is not frightened and collides with Pacman, we kill Pacman.
            i_pacman.set_dead(1);
        }
        else{ //Otherwise, the ghost starts running towards the house.
            use_door = 1;
            frightened_mode = 2;
            Position temp = {160,144};
            target = temp;
        }
    }
}


void Ghost::update_target(unsigned char i_pacman_direction, const Position &i_ghost_0_position,
                          const Position &i_pacman_position) {

    if(1==use_door){//if the ghost can use the door
        if(position == target){
            if(home_exit == target){// if the ghost has reached the exit it can no longer use the door
                use_door = 0;
            }
            else if( home == target ){ // it stops being frightened and leaves the home
                frightened_mode = 0;
                target=home_exit;
            }
        }
    }
    else{
        if(0 == movement_mode){//scatter mode
            //each ghost targets the corner its assigned to
            switch(id){
                case 0:{
                    target = {CELL_SIZE * (MAP_WIDTH - 1), 0};
                    break;
                }
                case 1:{
                    target = {0, 0};
                    break;
                }
                case 2:{
                    target = {CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1)};
                    break;
                }
                case 3:{
                    target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
                }
            }
        }
        else{ //chase mode
            switch(id){
                case 0:{ //the red ghost will chase pacman
                    target = i_pacman_position;
                    break;
                }
                case 1:{ //the Pink ghost will chase the fourth cell in front of pacman
                    target = i_pacman_position;
                    switch(i_pacman_direction){
                        case 0:{
                            target.x += CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 1:{
                            target.y -= CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 2:{
                            target.x -= CELL_SIZE * GHOST_1_CHASE;
                            break;
                        }
                        case 3:{
                            target.y += CELL_SIZE * GHOST_1_CHASE;
                        }
                    }
                    break;
                }
                case 2:{// The blue ghost will send a vector from the red ghost to the second cell in front of pacman
                    // then it will double that vector and follow that
                    target = i_pacman_position;

                    //getting the second cell in front of pacman
                    switch(i_pacman_direction){
                        case 0:{
                            target.x += CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 1:{
                            target.y -= CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 2:{
                            target.x -= CELL_SIZE * GHOST_2_CHASE;
                            break;
                        }
                        case 3:{
                            target.y += CELL_SIZE * GHOST_2_CHASE;
                        }
                    }
                    target.x += target.x - i_ghost_0_position.x;
                    target.y += target.y - i_ghost_0_position.y;

                    break;
                }
                case 3:{//the orange ghost will chase pacman until it gets close to him. Then it'll switch to the scatter mode.
                    //Using the Pythagoras' theorem again.
                    if (CELL_SIZE * GHOST_3_CHASE <= sqrt(pow(position.x - i_pacman_position.x, 2) + pow(position.y - i_pacman_position.y, 2))){
                        target = i_pacman_position;
                    }
                    else{
                        target = {0, CELL_SIZE * (MAP_HEIGHT - 1)};
                    }
                }
            }
        }
    }
}
Position Ghost::get_position(){
    return position;
}