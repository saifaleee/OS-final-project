#define _XOPEN_SOURCE 700

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <curses.h> // geetch
#include <X11/Xlib.h>

using namespace std;
using namespace sf;

void* draw(void* arg);

class Player {
public:
    string name;
    int x, y;
    int score;
    int player_no;
    int move_size;

    Player(string namee, int xx , int yy , int score = 0 , int moveesize =25){
        this->name = namee;
        this->y = yy;
        this->x = xx;
        this->move_size = moveesize;
    }

    void moveUp(){
        y-= move_size;
    }
    void moveDown(){
        y+= move_size;
    }
    void moveLeft(){
        x-= move_size;
    }
    void moveRight(){
        x+= move_size;
    }
};

class Object {
public:
    int x, y;
    int points;

    Object(int x,int y, int pointss){
        this->x = x;
        this->y = y;
        this->points = pointss;
    }
};

class GameBoard {
public:
    int width, height;
    int total_boxes;

    void set_total_boxes(int size){ // setter
        total_boxes = size;
    }
};

// Holds all game data
class GameState {
public:
    sf::RenderWindow &window; // window of game

    GameBoard board; // 2d game window, with boxes
    std::vector<Player> players; // All players in the game
    std::vector<Object> Objects;

    Event event;

    GameState(sf::RenderWindow& window) : window(window) {} // constructor to set rendered window obj
    
    void setEvent(Event &temp){
        this->event = temp;
    }
};

int generate_total_boxes() {

    
    int random_num = rand() % 90 + 10; // generate random number between 10 and 99
    random_num *= 9; // multiply by 9, my roll number is 
    int div_result = 649 / random_num; // integer division with roll_number
    int mod_result = div_result % 25; // modulo 25
    if (mod_result < 10) {
        mod_result += 15; // add 15 if less than 10
    }
    return mod_result;

}

int generate_total_objects(){

    int random_num = rand() % 10 + 5; // generate random number between 5 and 14

    return random_num;

}


void* gameLoop(void* arg) {
    GameState* state = static_cast<GameState*>(arg);

    while (state->window.isOpen()) {
    // Wait for a short time to allow the player movement threads to execute
    sf::sleep(sf::milliseconds(10));
        




        draw(state); // draw the board and all shapes
        // Sleep for some time to avoid 100% CPU usage
        usleep(1000);
    }

    return nullptr;
}

// Handle user input
void* handleInput(void* arg) 
{
    GameState* state = (GameState*) arg;
   while(1) { 
       sleep(milliseconds(100));
    if (state->event.type == sf::Event::Closed )
        state->window.close();

    if(state->players[0].player_no == 1){
            cout << "test" << endl;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) )
            {
                state->players[0].moveUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::A)){
                state->players[0].moveLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::S)){
                state->players[0].moveDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::D)){
                state->players[0].moveRight();
            }
        //}


    }
   }

    // return nullptr;
}


// Draw the game
// Draw the game
// Draw the game
void* draw(void* arg) {
    GameState* state = static_cast<GameState*>(arg);

    // Add drawing code here 
    state->window.clear();

    // drawing boxes on the board first
    int total_boxes_horizontal = state->board.width / state->board.total_boxes;
    int total_boxes_vertical = state->board.height / state->board.total_boxes;
    bool is_black = true;

    // loop through the boxes and draw them
    for (int i = 0; i < total_boxes_horizontal; i++) {
        for (int j = 0; j < total_boxes_vertical; j++) {

            if (j==0 || j == 1) {
                    // dont draw first 2 lines of boxes
            }
            else{



            // calculate the box position and size
            int box_size = state->board.total_boxes;
            int x = i * box_size;
            int y = j * box_size;
            int w = box_size;
            int h = box_size;

            // draw the box
            sf::RectangleShape rectangle(sf::Vector2f(w, h));
            if ((i + j) % 2 == 0) {
                // draw a black box
                rectangle.setFillColor(sf::Color::Black);
            } else {
                // draw a white box
                rectangle.setFillColor(sf::Color::White);
            }
            rectangle.setPosition(x, y);
            
            state->window.draw(rectangle);
            

            is_black = !is_black;
            }
        }
    }


    // draw objects
    for(int i=0;i< state->Objects.size(); i++){
        sf:: CircleShape circle(state->board.total_boxes - (state->board.total_boxes/2)); // size of circle
        circle.setPosition(state->Objects[i].x,state->Objects[i].y);
        circle.setFillColor(sf::Color::Green);
        state->window.draw(circle);
        
    }


    // DRAW PLAYERS
                sf::RectangleShape player_rectangle1(sf::Vector2f(state->board.total_boxes, state->board.total_boxes));
                sf::RectangleShape player_rectangle2(sf::Vector2f(state->board.total_boxes, state->board.total_boxes));

                player_rectangle1.setFillColor(sf::Color::Magenta);
                player_rectangle2.setFillColor(sf::Color::Yellow);

                            player_rectangle1.setPosition(state->players[0].x, state->players[0].y); // 1st player
                            player_rectangle2.setPosition(state->players[1].x, state->players[1].y); // 2nd player
                state->window.draw(player_rectangle1);
                state->window.draw(player_rectangle2);


            //cout << state->players[0].x << "  " << state->players[0].y << endl;
    state->window.display();
    return nullptr;
}


int random_pixel_in_range(int x) {

  int pixel = std::rand() % 1001 + 100;
  while (pixel % x != 0) { // not in multiple
    pixel = std::rand() % 1001 + 100;
  }
  return pixel;
}








int main() {
         XInitThreads();

    Event event;

    srand(time(NULL)); // seed the random number generator with current time

    int player_count = 0 ;
    PLAYERINPUT:
    cout << "ENTER THE NUMBER OF PLAYERS (1-3)" << endl;
    cin >> player_count ;
    cout << "Enter Player 1 Name : " << endl;
    string p1;
    cin >> p1;

    cout << "Enter Player 2 Name : " << endl;
    string p2;
    cin >> p2;
    



    if (player_count > 0 & player_count < 3){

    // Creating the game window
    RenderWindow window(VideoMode(1000, 1000), "My Game");


    
    // Creating the initial game state
    GameState state(window); // set window as well

    state.setEvent(event);

    state.board.set_total_boxes(generate_total_boxes()+30); // set box pixel size of board
    state.board.height=1000;
    state.board.width=1000; // same as Rendered window

        state.players.push_back( Player(p1,4*(state.board.total_boxes),4*(state.board.total_boxes),0,state.board.total_boxes/2)); // initialize 1st player
        state.players[0].player_no = 1;

        state.players.push_back( Player(p2,15*(state.board.total_boxes),15*(state.board.total_boxes),0,state.board.total_boxes/2)); // initialize 1st player
        state.players[1].player_no = 2;

    // initialize Players


    // initialize objects
    int total_objects = generate_total_objects();

    for(int i=0;i<total_objects;i++){

        state.Objects.push_back(Object(random_pixel_in_range(state.board.total_boxes),random_pixel_in_range(state.board.total_boxes),rand()%21));

    }







    // Create the game loop thread and make it detached
    pthread_t gameloopThread;
    pthread_attr_t gameloopThreadAttr;
    pthread_attr_init(&gameloopThreadAttr);
    pthread_attr_setdetachstate(&gameloopThreadAttr, PTHREAD_CREATE_DETACHED);

    // ##################### RUNNING GAMELOOP THREAD #########################
    pthread_create(&gameloopThread, &gameloopThreadAttr, gameLoop, static_cast<void*>(&state));
    // ##################### RUNNING GAMELOOP THREAD #########################
    pthread_attr_destroy(&gameloopThreadAttr);

    vector<pthread_t> movement_threads; // make n number of threads to handle players movement
    //Handle user input in separate threads

    // for (vector<Player>::iterator it = state.players.begin(); it != state.players.end(); ++it) {
            pthread_t movement_thread;
            pthread_attr_t movement_thread_Attr;
            pthread_attr_init(&movement_thread_Attr);
            pthread_attr_setdetachstate(&movement_thread_Attr, PTHREAD_CREATE_DETACHED);
            pthread_create(&movement_thread, &movement_thread_Attr, handleInput, static_cast<void*>(&state));
            movement_threads.push_back(movement_thread);
            // pthread_attr_destroy(&movement_thread_Attr);
    // }

    handleInput(static_cast<void*>(&state));



    

    // Wait for the window to close
    while (window.isOpen()) {
       
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                
            }

            else {


            }
        }

        // Sleep for some time to avoid 100% CPU usage
        usleep(1000);
    }

    }
    else{
        // Invalid Number of players
        cout << "Invalid Number of players" << endl;
        goto PLAYERINPUT;
    }

    exit(0);
}
