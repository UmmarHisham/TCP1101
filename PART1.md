# Part 1

## Video Demo

[Video Demo](https://youtu.be/QB1vPBx6YJU).

## Minimum Requirements

### Completed

List all the features completed.

1. Default and custom board size
2. Put the Alien in the middle of the board
3. Spawn zombie randomly according to the numbers of zombie
4. Alien can move in all 4 direction until it hit Borders or Obstacle
5. 


### To Do

List all the features not yet done. Remove this section if there is no incomplete requirements.

1. Random zombie movement
2. Zombie automatically hit the Alien after their move
3. All the item and prop
4. Alien and Zombie attribute display
5. Alien attack zombie
6. 


## Additional Features

Describe the additional features that has been implemented.

1. Not yet :)


## Contributions

List down the contribution of each group members.

### Ahmad Luqman Bin Zakarani

1. Randomly generate game board.
2. Alien movement
3. Alien stop when hit border or obstacles

### Muhammad Ummar Hisham Bin Ahmad Madzlan

1. Alien spawn in the middle of the map
2. Randomly generate zombie

## Problems Encountered & Solutions

1. how to create zombie randomly according to the zomnbie number 
  
   create vector array of zombie, so any number of zombie will create new row of the array
   and additionally create empty element for zombie attribute to be randomly assign in each of next column.

2. how to not create zombie with same coordinate
   
   create another array to check if the spot already occupied. if yes, it will keep looping to generate another X Y.

3. how to check if there is something that occupied the board when Alien is moving.

   also create another array to check the availability of the said coordinate when the Alien to move.