# platformer-creator-console
Create your own platformer level in the console using a .txt file

## About

This project allows you to create your own platformer by modifying a .txt file, like Super Mario Maker but in the console.

![image](https://user-images.githubusercontent.com/98898166/177667224-f5fa709d-b6a7-4cc5-a332-b04f96708348.png)

## Compatibility

This program only works on Windows with consoles that support ANSI escape codes.

## Setup

### Downloading the latest release

Most of the time, there should be a zipped folder under the releases section with a compiled binary and other neccecary files to run the program. If there is not, follow the instructions for compiling the project yourself and open an issue so I can fix it.

After navigating to the latest release, download the zipped folder with the compiled executable file and other neccecary files for the program to run (do not download Source code). After, run the .exe file.

### Compiling the project yourself

Just a note before proceeding: this project is made in Visual Studio 2022. If you have any issues with the g++ compiler, please check if it compiles in Visual Studio.

To compile, use `g++ ConsolePlatformer.cpp level_utils.cpp`

### Creating your own level

Create a new text file in the same directory as the executable file. Then, draw out your level with spaces and "X"s. Use P to spawn a player object. The program may not work without a player.

## Known Issues

- The leftmost column of a level is not visible
- Player cancels velocity before completely touching floor (medium severity)
- 10+ compiler warnings in Visual Studio (medium-low severity)

## Planned Features

- Colors
- Enemies
- Powerups
