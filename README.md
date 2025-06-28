# My Journey Creating This Game 🚀

This game is the result of **3 days and 4 nights of dedication**, where I spent about **5 hours each day coding and debugging**. It started as a simple idea—to replicate the classic XONIX game using SFML—but quickly became an immersive learning experience.

During this time, I faced and overcame multiple challenges: understanding SFML's rendering loop, dynamically managing memory in C++, and troubleshooting unexpected behaviors in game physics. Every bug felt like a boss fight, and every working frame felt like a reward.

I’d especially like to thank **coffee** ☕ and the **endless amount of cries** 😭 that somehow gave me the strength to keep going. Each crash, glitch, or logical error taught me something new, and every small win brought joy that made it all worth it.

I didn’t have prior experience building complete games, so this was my first real journey from an empty `main.cpp` file to a working game loop, animation, and player interaction. The joy of seeing things come alive on screen after hours of debugging made every late night worth it.

> 💬 *If you're a beginner like me, I hope this project inspires you to dive in. Building games is hard—but it's a fun kind of hard.*

---

# SFML XONIX Clone 🎮

Welcome to my first game developed using **SFML (Simple and Fast Multimedia Library)**! This is a beginner-friendly attempt to recreate the classic arcade game **XONIX**.

> 🚧 **Note:** This version includes most core features except head-to-head collision detection.

---

## 🎯 Game Overview

The player controls a block that moves across the screen, trying to claim territory by drawing lines. When an enclosed area is captured, it becomes part of the claimed space. Avoid enemies while you're outside the safe zone!

---

## 📁 Project Structure

- `main.cpp`: Contains the core logic and all functionality of the game.
- SFML library: Used for graphics, input, and game loop handling.

---

## 🧠 Concepts Demonstrated

This project is a great learning showcase for foundational C++ programming concepts:

- ✅ **Loops** – For game flow and input polling  
- ✅ **Pointers** – For dynamic entity management  
- ✅ **Dynamic Memory** – Used to manage game objects at runtime  
- ✅ **Structs** – To define entities and game components  
- ✅ **Enums** – For direction handling and game state
- ✅ **File Handling** – For keeping track of score and updating it 

---

## 🕹️ Controls

| Key        | Action        |
|------------|---------------|
| Arrow Keys | Move player   |
| Escape     | Exit the game |

---

## 🚀 Getting Started

### Prerequisites

- C++ Compiler (e.g., g++)
- SFML installed on your system ([SFML Setup Guide](https://www.sfml-dev.org/tutorials/))
