

# WYCIĄGAMI I GÓRAMI

## Course Information
- **Course Name**: DS&A (Design and Analysis of Algorithms)
- **Institution**: Politechnika Gdańska
- **Semester**: Summer 2025
- **Date**: 20/05/2025

## 📝 Description

### Context
"You are given a rectangular terrain map with elevation data and a set of ski lifts. Your goal is to find the shortest possible time to move from the starting point to the target location. You can move by walking to adjacent squares or by using available lifts, which operate at regular time intervals."

### Key Features
- **Map Representation**: 2D grid where each cell has a height (non-negative integer)
- **Movement Cost**:
  - Moving to a higher elevation: `A - B + 1` minutes
  - Moving to a lower or equal elevation: `1` minute
- **Movement**: Only in four cardinal directions (up/down/left/right)
- **Lifts**:
  - One-way teleportation from source to destination
  - Operate at fixed time intervals (e.g., every 5 minutes)
  - Require waiting time and travel time
  - Optimal use may involve choosing longer waits for faster overall routes

### Problem Solved
Efficient pathfinding with terrain elevation and time-based lift constraints.

## 📥 Input Format
```
width height
start_x start_y
target_x target_y
number_of_lifts
[lift_1 ... lift_n]
[map_heights]
```

Each lift is defined by:
```
start_x start_y dest_x dest_y travel_time departure_interval
```

### Example Input:
```
9 8
0 0
8 7
0
0 0 0 0 0 0 0 0 8
0 9 9 9 9 9 9 9 0
0 1 0 1 0 0 0 9 0
...
```

## 📤 Output Format
```
<shortest_time_to_reach_destination>
```

## 🛠️ Tech Stack

### Languages
- C++ (restricted to no STL usage except for basic I/O)

### Key Structures
- Custom priority queue (min-heap) for Dijkstra's algorithm
- Manual memory management for all dynamic structures
- Efficient lift mapping for O(1) access to lifts starting at any position

## 🚀 Features

- 📌 **Custom Priority Queue**: Manual implementation of min-heap used in Dijkstra
- 🏔️ **Elevation-aware pathfinding**: Handles both terrain and lifts optimally
- 🚡 **Lift scheduling logic**: Accounts for waiting time based on current time and lift interval
- 🧠 **Efficient Memory Use**: Clean and safe dynamic allocation/deallocation without STL
- 📈 **Optimized Performance**: Suitable for correctness and performance tests (tests 0–15)

## 📂 Project Structure
```
wyciagami-i-gorami/
├── main.cpp               # main implementation file
├── README.md              # project description
```

## 🚦 Running the Project

### Compilation:
```bash
g++ -O2 -std=c++17 main.cpp -o wyciagi
```

### Usage:
```bash
./wyciagi < input.txt
```

## 📚 Documentation
- All code is well-commented for clarity
- Functions are modular and descriptive
- Special attention given to memory management and data structure efficiency

## 👨‍💻 Contributors
- Piotr Skierka

## 🎓 Academic Note
This project was developed as part of coursework at Politechnika Gdańska
