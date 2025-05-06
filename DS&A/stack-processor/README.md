# STACK PROCESSOR 

**Course Name**: DS&A  
**Institution**: Politechnika Gdańska  
**Semester**: [Spring 2025]  
**Date**: [20/04/2025]  

---

## 📝 Description  
Context:
"The interstellar commission of Gedanum designed the STOS A15D stack-based processor to control energy-efficient space probes. This emulator accelerates the development of probe control systems before mass production begins."
Key Features

    Memory: Program memory (sequence of single-character instructions), instruction pointer, and a stack.

    Stack Elements: Lists of characters (digits, letters, symbols). Numbers are stored as digit sequences with optional trailing - (e.g., -1234 → 4321-).

    Instructions: 23 commands (e.g., ' push empty list, + add numbers, & print stack).

    I/O: Reads input characters and writes output via processor commands (e.g., >, .).

Constraints

    No STL (except I/O) or arrays (only for program/input storage).

    Single loop allowed (use recursion otherwise).

Input/Output

    Input:

        Line 1: Program (≤20k instructions, no whitespace).

        Line 2: Input characters (≤20k).

    Output: All processor-generated output.
Instruction set
    ' 	apostrophe	push an empty list on the stack
    , 	comma	pop a list from the stack
    : 	colon	put a copy of the list on the top of the stack
    ; 	semicolon	swap the places of the list on the top of the stack and the list directly below it
    @ 	at	pop the number A off the stack, then put a copy of the list on the A-th position on the stack (0 is the top of the stack, 1 is the list directly below it etc.); program '0@ is equivalent to the instruction :
    . 	dot	read a character from standard input and append it to the beginning of the list on the top of the stack
    > 	greater than	write the first character from the list on the top of the stack to standard output and pop the list off the top of the stack
    ! 	exclamation mark	logical negation: if the top of the stack is an empty list or a list containing a single character '0', replace it with a list containing the character '1'; otherwise replace the list on the top of the stack with a list containing the character '0'
    < 	less than	pop number A off the stack, pop number B off the stack, if B < A put number 1 on the stack, otherwise put number 0 on the stack
    = 	equal	pop number A off the stack, pop number B off the stack, if B = A put number 1 on the stack, otherwise put number 0 on the stack
    ~ 	tilde	put a number equal to the number of this instruction (the value of the instruction pointer) on the stack
    ? 	question mark	conditional jump: pop number T off the stack pop list W off the stack if W is not empty and does not contain only the character '0' put number T into the instruction pointer and do not increment the instruction pointer;
    -	minus	negation: if the last character of the top list is '-' (minus) remove it from the list; otherwise append '-' to the end of the list at the top of the stack
    ^ 	caret	absolute value: if the last character of the top list is '-' (minus) remove it from the list
    $ 	dollar	split: detach the first character from the top list and push it onto the stack
    # 	hash	pop list A off the stack; append A to the end of the list at the top of the stack
    + 	plus	pop number A off the stack; pop number B off the stack; put number A + B on the stack
    & 	ampersand	print the contents of the stack to the standard output in the format:

        n: list in the nth position on the stack
        ...
        1: list in the second position on the stack
        0: list on the top of the stack

    ]		pop number A off the stack; put the character with the ASCII number A on the stack
    [		pop list A off the stack; put the number equal to the ASCII number of the first character from list A on the stack
        remaining characters	append this character to the top of the list at the top of the stack

---

## 🛠️ Tech Stack  
**Languages**:  
- C with elements of C++

**Frameworks & Libraries**:  
- none except for iostream (restricted by assignment to not use STL library)

---

## 🚀 Features  
- **Feature 1**: Own stack structure implementation (using double linked lists)
- **Feature 2**: Own string structure implementation (using linked lists)
- **Feature 3**: Wide use of recursion
- **Feature 4**: Memory & time complexity optimized code

---

## 📂 Project Structure  
```plaintext
project-root/
├── src/                # Source files
├── docs/               # Documentation
├── tests/              # Test files
├── README.md           # Project overview
└── ...                 # Other directories/files

Running the Project:
    compile with g++ or other favourite compiler
📚 Documentation
    all of the code is well-described inside the .cpp

🤝 Contributors

    Piotr Skierka

🎓 Academic note

This project was developed as part of coursework at Politechnika Gdańska.
