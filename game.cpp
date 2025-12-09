#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

// INCLUDE SDL2
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

using namespace std;

const int GRID_SIZE = 20;    
const int CELL_SIZE = 30;    
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE; 

// --- SDL GLOBALS ---
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

bool initGraphics() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL Error: " << SDL_GetError() << endl;
        return false;
    }
    window = SDL_CreateWindow("Low Latency Snake", 
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                              WINDOW_SIZE, WINDOW_SIZE, 
                              SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return (renderer != nullptr);
}

void closeGraphics() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// --- RENDER FUNCTION (Now uses Pixels!) ---
void render(pair<int, char>* board, pair<int, int> apple, int score) {
    // 1. Clear Screen (Black)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 2. Draw Grid Content
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            // Define the square
            SDL_Rect rect;
            rect.x = x * CELL_SIZE;
            rect.y = y * CELL_SIZE;
            rect.w = CELL_SIZE;
            rect.h = CELL_SIZE;

            // Check what is in this cell
            int type = board[y * GRID_SIZE + x].first;
            
            if (x == apple.first && y == apple.second) {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // RED Apple
                SDL_RenderFillRect(renderer, &rect);
            } 
            else if (type == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // GREEN Snake
                SDL_RenderFillRect(renderer, &rect);
                
                // Optional: Draw a border around snake segments to see them better
                SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
                SDL_RenderDrawRect(renderer, &rect);
            }
        }
    }

    // 3. Swap Buffer (Show frame)
    SDL_RenderPresent(renderer);
}
void wrap(int &x, int &y) {
    if(x < 0) x = GRID_SIZE - 1;
    else if(x >= GRID_SIZE) x = 0;
    if(y < 0) y = GRID_SIZE - 1;
    else if(y >= GRID_SIZE) y = 0;
}
void assign(pair<int,int> & apple, pair<int,char>* board) {
    while(true) {
        int r = rand() % (GRID_SIZE * GRID_SIZE);
        if(board[r].first == 0) {
            apple.first = r % GRID_SIZE;
            apple.second = r / GRID_SIZE;
            break;
        }
    }
}
void gameplayh(char current_dir, int& head_x, int& head_y) {
    if(current_dir == 'a') head_x--;
    else if(current_dir == 'w') head_y--;
    else if(current_dir == 'd') head_x++;
    else if(current_dir == 's') head_y++;
    wrap(head_x, head_y);
}
void gameplayt(char tail_dir, int& tail_x, int& tail_y) {
    if(tail_dir == 'd') tail_x++;
    else if(tail_dir == 'w') tail_y--;
    else if(tail_dir == 's') tail_y++;
    else if(tail_dir == 'a') tail_x--;
    wrap(tail_x, tail_y);
}
int main(int argc, char* argv[]) {
    if (!initGraphics()) return 1;
    srand(time(0));
    pair<int,char> board[GRID_SIZE * GRID_SIZE];
    for(int i = 0; i < GRID_SIZE * GRID_SIZE; i++) board[i] = {0, 'n'};
    int head_x = GRID_SIZE/2, head_y = GRID_SIZE/2;
    int tail_x = head_x, tail_y = head_y;
    board[head_y * GRID_SIZE + head_x] = {1, 'n'};
    pair<int,int> apple;
    assign(apple, board);
    char current_dir = 'n';
    char next_dir = 'n';
    int score = 0;
    bool quit = false;

    // TIMING
    using clock = std::chrono::high_resolution_clock;
    auto last_time = clock::now();
    double acc = 0.0;
    double ticks = 0.1; 

    SDL_Event e;

    while(!quit) {
        // --- 1. SDL INPUT (Replaces kbhit) ---
        // This loop pulls all input events from the OS queue
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_w: if (current_dir != 's') next_dir = 'w'; break;
                    case SDLK_s: if (current_dir != 'w') next_dir = 's'; break;
                    case SDLK_a: if (current_dir != 'd') next_dir = 'a'; break;
                    case SDLK_d: if (current_dir != 'a') next_dir = 'd'; break;
                    case SDLK_q: quit = true; break;
                }
                if (current_dir == 'n') current_dir = next_dir;
            }
        }
        auto current_time = clock::now();
        std::chrono::duration<double> elapsed = current_time - last_time;
        last_time = current_time;
        acc += elapsed.count();
        if(acc >= ticks) {
            acc -= ticks;
            
            if (current_dir != 'n') {
                current_dir = next_dir;
                board[head_y * GRID_SIZE + head_x].second = current_dir;

                int next_x = head_x; 
                int next_y = head_y;
                gameplayh(current_dir, next_x, next_y);
                if(board[next_y * GRID_SIZE + next_x].first == 1) {
                     if (!(next_x == tail_x && next_y == tail_y)) {
                         cout << "GAME OVER! Score: " << score << endl;
                         quit = true;
                    }
                }
                bool ateApple = (next_x == apple.first && next_y == apple.second);
                board[next_y * GRID_SIZE + next_x].first = 1;
                head_x = next_x;
                head_y = next_y;
                if(!ateApple) {
                    if(board[tail_y * GRID_SIZE + tail_x].second != 'n') {
                        char tail_dir = board[tail_y * GRID_SIZE + tail_x].second;
                        board[tail_y * GRID_SIZE + tail_x].first = 0;
                        gameplayt(tail_dir, tail_x, tail_y);
                    }
                } else {
                    score++;
                    assign(apple, board);
                }
            }
            // --- 4. RENDER ---
            render(board, apple, score);
        }
        
        // Don't burn CPU
        SDL_Delay(1);
    }

    closeGraphics();
    return 0;
}