# Part 2

## Video Demo

[Video Demo](https://youtube.com).

## Minimum Requirements

### Completed

List all the features completed.

1. Default and custom board size
2. Put the Alien in the middle of the board
3. Spawn zombie randomly according to the numbers of zombie
4. Alien can move in all 4 direction until it hit Borders or Obstacle
5. Zombie can move randomly after Alien's turn.
6. Implementation of in-game objects.
7. Randomly generate the attributes for the Alien and Zombies.
8. Alien and Zombie attributes display.
8. Alien can damage the Zombie when they collided.
9. Zombie can attack the Alien when in range.


### To Do

List all the features not yet done. Remove this section if there is no incomplete requirements.

1. Save and load.
2. Damage pod implementation.

## Additional Features

Describe the additional features that has been implemented.

1. N/A.

## Contributions

List down the contribution of each group members.

For example:

### Ahmad Luqman Bin Zakarani

1. Implementing in-game objects.
2. Set the attributes for the aliens and zombies.
3. Zombies can move randomly.
4. Alien and Zombies attributes display.
5. Alien can damage the Zombies when they collided.
6. Zombie can attack the Alien when in range.

### Muhammad Ummar Hisham Bin Ahmad Madzlan

1. 

## Problems Encountered & Solutions

Describe the problems encountered and provide the solutions / plan for the solutions.

1. If zombie restricted with no space to move, it will loop indefinitely. <br />
Solution: make the if else statement that check the location of the zombie n if it is restricted by border or other zombie.

2. Finding range of alien from zombie.<br />
Solution: range = diffX + diffY.

3. The nearest zombie from pod.<br />
Solution: run loop checking for nearest zombie, if nearer it will clear a vector, and put the new nearest zombie, if there are 2 or more that same, it will randomly choosed.

3. Winning the game.<br />
Solution: every time the zombie is killed, zombie kill counter go up by 1, if the zombie killed is equal to zombie count, the alien win.

