Project Title: Memory Matching Game (C++ with SFML)

Developed by:
Insharah Irshad  
BS Artificial Intelligence – Hamdard University  

Description:
A fun and interactive card matching game built using C++ and the SFML (Simple and Fast Multimedia Library).  
Players must flip cards and find matching pairs while tracking their score and time.

Objective:
Match all 8 pairs of cards as fast as you can!  
Score increases by 10 points on every successful match.  
Game ends with a victory screen once all cards are matched.

Features:
-  Graphical Interface (SFML)
-  4x4 Grid with 8 unique color-coded pairs
-  Timer and scoring system
-  Start menu and end screen with “Play Again” feature
-  Animated flip logic (with delay)
-  Custom font loading using `arial.ttf`
-  Credit screen

---

Technologies Used:
- Language: C++
- Library: SFML 2.5.1
- IDE: Visual Studio / g++
- Font: Arial.ttf (included in assets)

How to Run:

**Windows (with executable)**:
1. Double-click on "main.exe" in "lib" folder
2. Ensure "arial.ttf" is present in the "assets" folder

**To Compile Yourself:**
1. Install SFML 2.5.1 from: https://www.sfml-dev.org/download.php
2. Configure your compiler (Visual Studio)
3. Link the following libraries:
   - sfml-graphics
   - sfml-window
   - sfml-system
4. Use "main.cpp" from "src/" folder and make sure SFML "include/" and "lib/" paths are added
5. Compile and Run

Controls:

Left Mouse Click: Flip card

Game ends when all cards are matched.

Score is displayed

Time taken is shown

Option to play again

Notes:

Do not remove arial.ttf, it's required for rendering fonts

Project will not run without proper SFML setup