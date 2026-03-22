#include <iostream>
#include <vector>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace std;

void drawBoard(int height, int width, vector<pair<int, int>> &snake, int foodX, int foodY, string dir, int score, int speed){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(i == 0 || i == height - 1 || j == 0 || j == width - 1){
                cout << "#";
                continue;
            }else if(i == foodY && j == foodX){
                cout << "O";
                continue;
            }

            bool isSnake = false;
            for(int k = 0; k < snake.size(); k++){
                if(i == snake[k].second && j == snake[k].first){
                    cout<<(k == 0 ? "@" : "o");
                    isSnake = true;
                    break;
                }
            }
            if(!isSnake) cout<<" ";
        }
        cout<<endl;
    }

    cout<<"Score: "<<score<<endl;
    cout<<"Speed: "<<speed<<endl;
    cout<<"FoodX: "<<foodX<<endl;
    cout<<"FoodY: "<<foodY<<endl;
    cout<<"Direction: "<<dir<<endl;
}

pair<int, int> getFood(int height, int width, vector<pair<int, int>> snake){
    int foodX, foodY;
    bool collision = false;
    do{
        foodX = rand() % (width - 2) + 1;
        foodY = rand() % (height - 2) + 1;
        for(auto s: snake){
            if(foodX == s.first && foodY == s.second){
                collision = true;
                break;
            }
        }
    }
    while(collision);
    return make_pair(foodX, foodY);
}

bool checkCollision(int headX, int headY, vector<pair<int, int>> &snake, int height, int width){
    if(headX == 0 || headX == width - 1 || headY == 0 || headY == height - 1){
        return true;
    }
    for(int i = 1; i < snake.size(); i++){
        if(headX == snake[i].first && headY == snake[i].second){
            return true;
        }
    }

    return false;
}

int getMillisecondsBySpeed(int speed){
    return speed < 10 ? (200 / speed) : 20;
}

/*
wall - #
snake - @
food - O
*/

int main(){
    int height = 20, width = 40;
    int score = 0, speed = 1;

    vector<pair<int, int>> snake;
    snake.push_back(make_pair(width / 2, height / 2));
    // int snakeX = rand() % (width - 2) + 1;
    // int snakeY = rand() % (height - 2) + 1;
    auto [foodX, foodY] = getFood(height, width, snake);
    string dir = "RIGHT";

    while(true){
        system("cls");
        drawBoard(height, width, snake, foodX, foodY, dir, score, speed);
        
        // get input
        if(_kbhit()){
            char input = _getch();
            if(input == 'q'){
                break;
            }else if(input == 'w' && dir != "DOWN"){
                dir = "UP";
            }else if(input == 's' && dir != "UP"){
                dir = "DOWN";
            }else if(input == 'a' && dir != "RIGHT"){
                dir = "LEFT";
            }else if(input == 'd' && dir != "LEFT"){
                dir = "RIGHT";
            }
        }

        // eat food
        int headX = snake[0].first;
        int headY = snake[0].second;
        if(headX == foodX && headY == foodY){
            ++score;
            ++speed;
            auto [newFoodX, newFoodY] = getFood(height, width, snake);;
            foodX = newFoodX;
            foodY = newFoodY;
        }else{
            // remove tail
            snake.pop_back();
        }
        
        // move snake
        if(dir == "RIGHT"){
            headX++;
        }else if(dir == "LEFT"){
            headX--;
        }else if(dir == "UP"){
            headY--;
        }else if(dir == "DOWN"){
            headY++;
        }
        snake.insert(snake.begin(), make_pair(headX, headY));

        // collision
        if(checkCollision(headX, headY, snake, height, width)){
            cout<<"Game Over"<<endl;
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(getMillisecondsBySpeed(speed)));
    }

    return 0;
}