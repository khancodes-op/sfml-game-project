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

---

## 🕹️ Controls

| Key       | Action              |
|-----------|---------------------|
| Arrow Keys | Move player         |
| Escape    | Exit the game       |

---

## 📸 Screenshots

*(You can add gameplay screenshots here to showcase the game visually.)*

---

## 🚀 Getting Started

### Prerequisites

- C++ Compiler (e.g., g++)
- SFML installed on your system ([SFML Setup Guide](https://www.sfml-dev.org/tutorials/))

### Compilation

```bash
g++ main.cpp -o xonix -lsfml-graphics -lsfml-window -lsfml-system
./xonix
