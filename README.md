# platformer-creator-console
Create your own platformer level in the console using a .txt file

## About

This project allows you to create your own platformer by modifying a .txt file, like Super Mario Maker but in the console.

![image](https://user-images.githubusercontent.com/98898166/177667224-f5fa709d-b6a7-4cc5-a332-b04f96708348.png)

## Compatibility

This program only works on: 
- Windows
- consoles that support ANSI escape codes
- C++17 (if you are compiling yourself)

## Setup

### Downloading the latest release

Most of the time, there should be a zipped folder under the releases section with a compiled binary and other neccecary files to run the program. If there is not, follow the instructions for compiling the project yourself and open an issue so I can fix it.

After navigating to the latest release, download the zipped folder with the compiled executable file and other neccecary files for the program to run (do not download Source code). After, run the .exe file.

### Compiling the project yourself

Just a note before proceeding: this project is made in Visual Studio 2022. If you have any issues with the g++ compiler, please check if it compiles in Visual Studio.

To compile, use `g++ ConsolePlatformer.cpp level_utils.cpp`

## Creating your own level

In the levels directory, create a new folder. The folder name will be your level name. Inside, create two files: `level.txt` and `colors.txt`. The level will not work without these two files.

### level.txt

In `level.txt`, create your level using spaces as air and other characters as ground. The "X" character is recommended as ground. Some characters have special behavior:

- O (uppercase o, not zero): Player
- G: goal

**Every level needs a player to be ran. Otherwise, it will crash.** 

Some things to keep in mind:
- Multiple players can be spawned. This can make some interesting game mechanics.
- A goal allows the player to win upon touching it.

An example of `level.txt` looks like this:
![image](https://user-images.githubusercontent.com/98898166/178369871-d2cdf371-f7ec-4e41-9af9-b06b1545dedf.png)

### colors.txt

The `colors.txt` file has the object that is colored and a corresponding ANSI escape code. Keep in mind that the entire ANSI escape code is not written, only everything after the `[`. For example, if the ANSI escape code is `\u001b[31;1m`, only `31;1m` will be typed in the colors.txt file. An example of the colors.txt file looks like this:
```
O 34;1m
G 32m
```

## Known Issues

- The leftmost column of a level is not visible
- 10+ compiler warnings in Visual Studio

## Planned Features

- Default levels
- Enemies
- Death objects/spikes
- Powerups
