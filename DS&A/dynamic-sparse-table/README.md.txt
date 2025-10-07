# Dynamic Sparse Table

## Course Information
- **Course Name**: DS&A (Design and Analysis of Algorithms)
- **Institution**: Politechnika Gdańska
- **Semester**: Summer 2025
- **Date**: 20/05/2025

## 📝 Description

### Context
To store values indexed by sparse keys from a certain universe N, hash tables seem to be a good solution. A downside of using them may be poor access time in the pessimistic case. Another way to handle this task is to use a trie tree, which offers a better pessimistic time estimate.

### Key Features
- **Trie Structure**: The trie is described by two parameters: n and k
- **Tree Construction**: 
  - Root contains n child nodes
  - Each child node contains k more child nodes
- **Insertion**: Start from root, if occupied use `div = x mod n` then `div = div mod k` until finding free spot
- **Searching**: Similar to insertion, follow nodes with indices `mod n` then `mod k` until finding value or empty node
- **Deletion**: Find node with key, if internal node find leftmost leaf candidate, copy value and delete candidate node

### Problem Solved
Efficient storage and retrieval of sparse keys using trie structure with better worst-case performance than hash tables.

## 📥 Input Format
```
N
min max
n k

[command_1 ... command_N]
```

Commands:
- `I x` - Insert key x into trie
- `L x` - Lookup key x in trie  
- `D x` - Delete key x from trie
- `P` - Print trie contents in preorder

### Example Input:
```
29
0 511
8 4

I 3
I 31
I 150
I 190
I 130
I 174
I 81
I 30
I 203
P
L 32
L 30
L 150
...
```

## 📤 Output Format
Command responses:
- `I x`: "x exist" if already exists, otherwise no output
- `L x`: "x exist" if found, "x not exist" if not found
- `D x`: "x not exist" if deletion fails, otherwise no output
- `P`: Space-separated values in preorder traversal

### Example Output:
```
3 81 130 203 150 174 190 30 31 
32 not exist
30 exist
150 exist
150 not exist
150 exist
3 not exist
3 exist
3 not exist
30 not exist
30 exist
30 not exist
```

## 🛠️ Tech Stack

### Languages
- C++ (restricted to no STL usage except for basic I/O)

### Key Structures
- Custom trie tree implementation with dynamic node allocation
- Manual memory management for all tree nodes
- Efficient traversal algorithms for insert, search, and delete operations

## 🚀 Features

- 📌 **Custom Trie Implementation**: Manual tree structure with configurable branching factors
- 🔍 **Efficient Operations**: Insert, lookup and delete with optimized traversal
- 🍃 **Smart Deletion**: Leftmost leaf selection for internal node deletion
- 📊 **Preorder Traversal**: Complete tree visualization in preorder sequence
- 🧠 **Manual Memory Management**: Clean allocation/deallocation without STL containers

## 📂 Project Structure
```
dynamic-sparse-table/
├── main.cpp               # main implementation file
├── README.md              # project description
```

## 🚦 Running the Project

### Compilation:
```bash
g++ -O2 -std=c++17 main.cpp -o sparse_table
```

### Usage:
```bash
./sparse_table < input.txt
```

## 📚 Documentation
- All code is well-commented for clarity
- Modular function design for trie operations
- Special attention to memory management and tree traversal algorithms

## 👨‍💻 Contributors
- Piotr Skierka

## 🎓 Academic Note
This project was developed as part of coursework at Politechnika Gdańska, within the "DS&A" course.
