# Snake_carrot_chicken
> This repository is the repository for the software engineering course, and we need to build on the basic code given to refine it

##### introduction

The purpose of this document is to explain the software analysis and design of the Snake game, and its purpose is to provide the reader with a detailed description of the game's features, interface, and implementation. This document is intended for development team members and relevant stakeholders.

##### Rules of the game

When the game starts, the snake has an initial length and grows as it eats food.
The food is randomly generated on the map, and the snake needs to move to eat it.
Each time a food is eaten, the length of the snake increases by one section and the score is updated.
The snake will move faster and faster.
Game end condition: The snake touches a wall or its own body.

##### Architecture design

Game master module: responsible for processing user input, updating game status and interface display.
Map module: responsible for generating maps, collision detection, etc.
Snake module: Responsible for snake movement, length increase and collision detection.
Food module: Responsible for food generation and location detection.
Interface module: Responsible for displaying the game interface and user interaction.

##### Functional design

Game start function: initializes the game state and displays the start interface.
User input function: monitor keyboard events and control the direction of snake movement.
Snake movement function: Updates the position and length of the snake according to user input.
Food generation function: Randomly generate food and detect whether it overlaps with the snake.
Collision detection function: Detect whether the snake is touching the wall or its own body.
End of Game function: Display the end of game screen and record the highest score.

> It took us two weeks to complete the project, and for this project you need to register the first time you run the program so that you can play the game later. When you register, we will store your information in the user table. After registration and re-login, you can play the game.



> Users can use the keyboard to control the snake's movement.
>
> Snakes can eat food and increase their length.
>
> If the snake touches a wall or its own body, the game is over.
