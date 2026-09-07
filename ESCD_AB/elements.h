#ifndef ELEMENTS_H
#define ELEMENTS_H

#include "globals.h"

#define ENEMY_ONE                 0
#define ENEMY_TWO                 1
#define OBJECT                    2
#define FLOOR_ONE                 3
#define FLOOR_TWO                 4
#define FLOOR_THREE               5
#define FLOOR_FOUR                6
#define FLOOR_FIVE                7
#define ENEMY_BULLET              8

#define ENEMY                     0

#define ENEMY_BOX                 0   // enemies_plus_mask frames 0-3  (N,E,S,W)
#define ENEMY_FLYER               1   // enemies_plus_mask frames 4-7  (N,E,S,W)
#define ENEMY_MOVER               2   // enemies_plus_mask frames 8-11 (N,E,S,W)
#define ENEMY_SHOOTER             3   // enemies_plus_mask frames 12-15(N,E,S,W)

#define PICKUP_BLACK_CARD         0     // opens level door
#define PICKUP_WHITE_CARD         1     // opens normal door
#define PICKUP_BATTERY            2     // 1 extra life
#define PICKUP_BULLET             3     // 1 shot
#define PICKUP_CHIP               4     // extra points
#define TELEPORT                  5     // TELEPORT
#define SWITCH_OFF                6     // TOGGLE ELEMENTS IS OFF
#define SWITCH_ON                 7     // TOGGLE ELEMENTS IS ON

#define FLOOR_NORMAL              0     
#define FLOOR_BOX                 1     // (not changeable by switch) and will hurt the player - bullet can NOT pass
#define FLOOR_SPIKE               2     // (not changeable by switch)- bullet can NOT pass
#define FLOOR_PIRAMIDE            3     // (not changeable by switch) - bullet removes piramide
#define FLOOR_PIT                 4     // (can be removed by switch) - bullet can pass

#define SCORE_BLACK_CARD          100
#define SCORE_WHITE_CARD          10
#define SCORE_LIFE                20
#define SCORE_TO_MUCH_LIFE        50
#define SCORE_BULLET              30
#define SCORE_CHIP                200
#define SCORE_OPEN_DOOR           50
#define SCORE_LEVEL_DOOR          1000


struct Element
{
  public:
    int x, y;
    byte characteristics = 0b00000000;   //this byte holds all the enemies / object / floors / enemie bullet characteristics
    //                       ||||||||
    //                       |||||||└->  0 \
    //                       ||||||└-->  1  | these 3 bits are used to determine kind of sprite to use
    //                       |||||└--->  2 /
    //                       ||||└---->  3 \  these 2 bits are used for direction
    //                       |||└----->  4 /  00 = NORTH / 01 = EAST / 10 = SOUTH / 11 = WEST
    //                       ||└------>  5 this enemy/object can hurt you         (0 = false / 1 = true)
    //                       |└------->  6 this object is movable by player       (0 = false / 1 = true)
    //                       └-------->  7 this object can be picked up by player (0 = false / 1 = true)
    byte frame = 0;
};

///// PREPARE ENEMY / ELEMENT / FLOOR ///////
/////////////////////////////////////////////
// create 9 places for the elements to be stored
// place 0 ENEMY_ONE      4 different types
// place 1 ENEMY_TWO      4 different types
// place 2 OBJECT         8 different types
// place 3 FLOOR_ONE      4 different types
// place 4 FLOOR_TWO      4 different types
// place 5 FLOOR_THREE    4 different types
// place 6 FLOOR_FOUR     4 different types
// place 7 FLOOR_FIVE     4 different types
// place 8 ENEMY_BULLET   4 different types
// slots 0-7 are room elements; slot 8 is the live enemy shot
Element elements[9];
bool enemyBulletActive = false;

void drawEnemies(bool i)
{
  byte ch = elements[i].characteristics;
  sprites.drawPlusMask(elements[i].x, elements[i].y + currentRoomY, enemies_plus_mask,
                       ((ch & 0b00000111) << 2) | ((ch & 0b00011000) >> 3));
}

void drawEnemyOne() { drawEnemies(ENEMY_ONE); }
void drawEnemyTwo() { drawEnemies(ENEMY_TWO); }



////////////////// DRAW OBJECT //////////////
/////////////////////////////////////////////
// We have ... different object types and only 1 object we can draw
void drawObject()
{
  if (arduboy.everyXFrames(8)) elements[OBJECT].frame = (++elements[OBJECT].frame) % 6;
  sprites.drawPlusMask(elements[OBJECT].x + 4, elements[OBJECT].y + currentRoomY + 6, elements_plus_mask, elements[OBJECT].frame + (6 * ((elements[OBJECT].characteristics & 0b00000111))));
}

///////////////// DRAW SPECIAL FLOOR ////////
/////////////////////////////////////////////
// We have 4 different floor types and 5 floor tiles we can draw
void drawFloor(byte floor)
{
  if (elements[floor].characteristics > 0b00000000)
  sprites.drawPlusMask(elements[floor].x - 3, elements[floor].y + currentRoomY + 9, floorTile_plus_mask, (elements[floor].characteristics & 0b00000111));
}

// Thin wrappers required by the function-pointer table
void drawFloorOne()   { drawFloor(FLOOR_ONE);   }
void drawFloorTwo()   { drawFloor(FLOOR_TWO);   }
void drawFloorThree() { drawFloor(FLOOR_THREE); }
void drawFloorFour()  { drawFloor(FLOOR_FOUR);  }
void drawFloorFive()  { drawFloor(FLOOR_FIVE);  }


///////////////// DRAW ENEMIE BULLET ////////
/////////////////////////////////////////////
// We have 1 different bullet types and 1 bullet tile/sprite we can draw
void drawBulletEnemy()
{
  if (!enemyBulletActive) return;
  sprites.drawPlusMask(elements[ENEMY_BULLET].x + 4, elements[ENEMY_BULLET].y + currentRoomY + 6, elements_plus_mask, 18);
}


#endif