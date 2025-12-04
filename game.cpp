#include <iostream>
#include <conio.h>  
#include <windows.h> 
#include <thread>   
#include <chrono>   
#include <vector>
#include <cstdlib>   // for rand, system
using namespace std;



// First a way to delay by 1 sec
//Then a snake class to control the movement
//Then a game class to control mechanics 

//now I need to flatten the vector
// and use chrono for timing instead of d
//lastly implement the logic for rendering the board and snake

void update(char &current_dir, int &head_x, int& head_y)
{
    if(current_dir == 'a')
    {
        head_x--;
    }
    if(current_dir == 'w')
    {
        head_y--;
    }
    if(current_dir == 'd')
    {
        head_x++;
    }
    if(current_dir == 's')
    {
        head_y++;
    }
    return;
}

void assign(pair<int,int> & apple, pair<int,char>* board)
{
    while(board[apple.first + 50*apple.second].first == 1)
    {
        apple.first = rand() % 50;
        apple.second = rand() % 50;
    }
}

void check(char& current_dir, int& head_x, int& head_y,pair<int,char>* board)
{
    if(head_x < 0)
    {
        head_x++;
        if(head_y>3)
        {
            current_dir = 'w';
            board[head_x + head_y*50].second = 'w';
            head_y--;
        }
        else
        {
            current_dir = 's';
            board[head_x + 50*head_y].second = 's';
            head_y++;
        }
    }
    if(head_y < 0)
    {
        head_y++;
        if(head_x>3)
        {
            current_dir = 'a';
            board[head_x + 50*head_y].second = 'a';
            head_x--;
        }
        else
        {
            current_dir = 'd';
            board[head_x + 50*head_y].second = 'd';
            head_x++;
        }
    }
    if(head_x > 49)
    {
        head_x--;
        if(head_y>3)
        {
            current_dir = 'w';
            board[head_x + 50*head_y].second = 'w';
            head_y--;
        }
        else
        {
            current_dir = 's';
            board[head_x + 50*head_y].second = 's';
            head_y++;
        }
    }
    if(head_y > 49)
    {
        head_y--;
        if(head_x>3)
        {
            current_dir = 'a';
            board[head_x + 50*head_y].second = 'a';
            head_x--;
        }
        else
        {
            current_dir = 'd';
            board[head_x + 50*head_y].second = 'd';
            head_x++;
        }
    }
}

void gameplayh(char& current_dir, int& head_x, int& head_y)
{
    if(current_dir == 'a')
        {
            head_x = head_x - 1;
        }
        else if(current_dir == 'w')
        {
            head_y = head_y - 1;
        }
        else if(current_dir == 'd')
        {
            head_x = head_x + 1;
        }
        else if(current_dir == 's')
        {
            head_y = head_y + 1;
        }
}

void gameplayt(char& tail_dir, int& tail_x, int& tail_y)
{
    if(tail_dir == 'd')
    {
        tail_x = tail_x + 1;
    }
    else if(tail_dir == 'w')
    {
        tail_y = tail_y - 1;
    }
    else if(tail_dir == 's')
    {
        tail_y = tail_y + 1;
    }
    else if(tail_dir == 'a')
    {
        tail_x = tail_x - 1;
    }
}

bool take_input()
{
    return true;
}

void gameover()
{
    cout<<"Gameover";
    exit(0);
}

// render function



int main() {
    //the board is the game board where the snake plays
    pair<int,char> board[50*50];
    for(int i = 0; i < 50 * 50; i++) {
        board[i] = {0, 'n'}; 
    }
    int snake_size = 1;
    char current_dir = 'n';
    char input_dir = 'n';
    char tail_dir = 'n';
    int head_x = 25,head_y = 25;
    int tail_x = 25,tail_y = 25;
    int d = 0;
    pair<int,int> apple = {4,4};

    using clock = std::chrono::high_resolution_clock;
    auto last_time = clock::now();
    double acc = 0.0;
    double ticks = 0.1;

    while(true)
    {
        if(_kbhit())
        {
            input_dir = _getch();
            if(input_dir == 'n')
            continue;
            else if(current_dir == 'w' && input_dir == 's')
            continue;
            else if(current_dir == 's' && input_dir == 'w')
            continue;
            else if(current_dir == 'a' && input_dir == 'd')
            continue;
            else if(current_dir == 'd' && input_dir == 'a')
            continue;
            else
            {
                current_dir = input_dir;
                board[head_x + head_y*50].second = current_dir;
            }   
        }
        auto current_time = clock::now();
        std::chrono::duration<double> elapsed = current_time - last_time;
        last_time = current_time;
        acc += elapsed.count();
        if(acc>=ticks)
        {
            acc = 0;
            gameplayh(current_dir,head_x,head_y);
            check(current_dir,head_x,head_y,board);
            board[head_x + head_y*50].first++;
            if(board[head_x + head_y*50].first>1)
            {
                gameover();
            }
            if(board[tail_x + tail_y*50].second != 'n')
            {
                tail_dir = board[tail_x + tail_y*50].second;
            }
            gameplayt(tail_dir,tail_x,tail_y);
            board[tail_x + tail_y*50].first--;
            if(head_x == apple.first && head_y == apple.second)
            {
                update(current_dir,head_x,head_y);
                check(current_dir, head_x, head_y,board);
                board[head_x + head_y*50].first++;
                assign(apple,board);
            }
        } 
    }
    return 0;
}
