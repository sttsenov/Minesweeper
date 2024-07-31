
# Minesweeper

This project aims to create a Minesweeper clone using the native Win32 GUI functionality. From the extensive playing of the game, I've realised that there's a missing feature that, in my opinion, can allow for faster solve time. 

The feature I am talking about is the use of the keyboard. I believe that a page can be taken from the book of Vim and similar editors that mainly rely on keyboard shortcuts as opposed to mouse events. 

Currently, this project does **NOT** support keyboard shortcuts, except for the "space" key that can be used to place flags once the game is started. The code as it is now is solely responsible for creating a clone of the game with little to no novelty. That being said, one day when I decide to return to this project (fingers crossed), the goal will be to implement the aforementioned feature.

## Notes

There are no options to choose a difficulty. Running the application means that you are always playing on the expert difficulty. This means that every game will be played on a __30x16__ grid and will contain __99__ mines.

The timer that is displayed on the top right side of the window doesn't get recorded anywhere after you win a game. That timer is there purely for curiousity's sake and has an upper limit of around __1000__ seconds.

All of the assets have been created by me and can be found in the __assets__ folder. All of the macros and constants can be found in the `stdafx.h` file.

## Screenshots

__Game Running:__

![Minesweeper Game Running](https://gcdnb.pbrd.co/images/pLqpCZWMyeni.png?o=1)

\
__Game Over:__

![Minesweeper Game Over](https://gcdnb.pbrd.co/images/op6aaf7iPtDm.png?o=1)

