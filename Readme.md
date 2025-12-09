Low Latency Snake (C++)
A high-performance implementation of the classic Snake game. This project demonstrates Low Latency software design principles by enforcing strict memory management constraints (Zero-Allocation on the Hot Path) and using hardware-accelerated graphics.

Key Features:
Zero Heap Allocation: No new, malloc, or std::vector resizing occurs during the game loop. All memory is pre-allocated.

Ring Buffer Architecture: Snake movement is handled via O(1) ring buffer logic rather than costly array shifting.

Hardware Acceleration: Rendering is decoupled from logic and handled by the SDL2 graphics library.

Cross-Platform Logic: Game mechanics are isolated from the OS-specific rendering layer.

Prerequisites
C++ Compiler (g++ recommended)

SDL2 Library

Windows (MSYS2): pacman -S mingw-w64-x86_64-SDL2

Linux (Ubuntu/Debian): sudo apt-get install libsdl2-dev

Build Instructions
Windows (MSYS2 / MinGW)
Run the following command in your terminal:

Bash

g++ game.cpp -o game -I C:/msys64/mingw64/include -L C:/msys64/mingw64/lib -lmingw32 -lSDL2main -lSDL2
(Note: If you installed MSYS2 in a custom location, update the -I and -L paths accordingly.)

Linux
Bash

g++ game.cpp -o game -lSDL2
Controls
W / A / S / D: Movement

Q or X: Quit Game

Design Philosophy
Standard game implementations often destroy and recreate objects (Snake Segments) every frame. This project avoids that Memory Churn by using a fixed-size stack array and updating only the head and tail indices. This was made as a beginner project for getting started with low level C++.

NOTE: The entire core logic was designed and implemented by me. However, only the rendering part using SDL2 was done with help from Gemini.