snakeII (Snake 2.0)

Overview
- A C++ console snake game with modular architecture.
- Features: FSM state flow, fixed-timestep update loop, robust input filtering, save/load, colored border rendering.

Project Structure
- core/: Game loop + state machine
- systems/: Input, render, time
- gameplay/: Snake, food
- persistence/: Save system
- main.cpp: Entry point

Requirements
- Windows 10/11
- CMake 3.10+
- Visual Studio 2022 Build Tools (MSVC)
- VS Code extensions (recommended):
  - ms-vscode.cpptools
  - ms-vscode.cmake-tools

Build and Run (Terminal)
1) Configure:
   cmake -S . -B build
2) Build:
   cmake --build build
3) Run:
   .\\build\\Debug\\snakeII.exe

Build and Run (VS Code Tasks)
- Run task: Configure snakeII
- Run task: Build snakeII
- Run task: Run snakeII

Controls
- Move: WASD / Arrow Keys
- Pause/Resume: P
- Back to menu: M
- Save/Load: K / L
- Speed: + / -
- Exit: Q

Notes
- Border uses horizontal 2x mapping for consistent visual density in monospace console.
- Snake and food keep single-cell logic coordinates.
