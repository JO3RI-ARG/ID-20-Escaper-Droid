#ifndef ROOM_H
#define ROOM_H

#include "globals.h"
#include "levels.h"
#include "player.h"
#include "elements.h"

#define UPPERBIT_OFFSET               4
#define LEVEL_OFFSET                  1
#define ROOM_DRAWING_OFFSET           -9         

#define TILE_INFRONT_DOOR_NORTH       2
#define TILE_INFRONT_DOOR_EAST        10
#define TILE_INFRONT_DOOR_SOUTH       22
#define TILE_INFRONT_DOOR_WEST        14
#define TILE_IN_MIDDLE                12

#define NORTH_DOOR_EXISTS             NORTH + UPPERBIT_OFFSET
#define NORTH_DOOR_IS_CLOSSED         NORTH
#define NORTH_LINTEL                  11
#define NORTH_BIG_POST                12
#define NORTH_SMALL_POST              13
#define NORTH_DOOR_CLOSSED            14

#define EAST_DOOR_EXISTS              EAST + UPPERBIT_OFFSET
#define EAST_DOOR_IS_CLOSSED          EAST
#define EAST_LINTEL                   15
#define EAST_BIG_POST                 16
#define EAST_SMALL_POST               17
#define EAST_DOOR_CLOSSED             18

#define SOUTH_DOOR_EXISTS             SOUTH + UPPERBIT_OFFSET
#define SOUTH_DOOR_IS_CLOSSED         SOUTH
#define SOUTH_LINTEL                  19
#define SOUTH_BIG_POST                20
#define SOUTH_SMALL_POST              21
#define SOUTH_DOOR_CLOSSED            22

#define WEST_DOOR_EXISTS              WEST + UPPERBIT_OFFSET
#define WEST_DOOR_IS_CLOSSED          WEST
#define WEST_LINTEL                   23
#define WEST_BIG_POST                 24
#define WEST_SMALL_POST               25
#define WEST_DOOR_CLOSSED             26

#define EMPTY_PLACE                   27


//define how collision works
#define DIFF(A, B) (((A) > (B)) ? ((A) - (B)) : ((B) - (A)))

byte levelUpAnimation = 0;

struct Room {
  public:
    byte doorsClosedActive;
    byte elementsActive;
    byte roomToTransportTo;
    byte roomNumberInfluencing;
    byte roomNumberFromInfluencer;
    byte elementsInfluenced;

    void set()
    {
      doorsClosedActive = 0b00000000;   // this byte holds all the 4 doors characteristics for each room
      //                    ||||||||
      //                    |||||||└->  0  DOOR NORTH  IS CLOSED (0 = false / 1 = true)
      //                    ||||||└-->  1  DOOR EAST   IS CLOSED (0 = false / 1 = true)
      //                    |||||└--->  2  DOOR SOUTH  IS CLOSED (0 = false / 1 = true)
      //                    ||||└---->  3  DOOR WEST   IS CLOSED (0 = false / 1 = true)
      //                    |||└----->  4  DOOR NORTH  EXISTS    (0 = false / 1 = true)
      //                    ||└------>  5  DOOR EAST   EXISTS    (0 = false / 1 = true)
      //                    |└------->  6  DOOR SOUTH  EXISTS    (0 = false / 1 = true)
      //                    └-------->  7  DOOR WEST   EXISTS    (0 = false / 1 = true)

      elementsActive = 0b00000000;
      //                 ||||||||
      //                 |||||||└->  7 => 0 FLOOR  5 EXISTS (0 = false / 1 = true)
      //                 ||||||└-->  6 => 1 FLOOR  4 EXISTS (0 = false / 1 = true)
      //                 |||||└--->  5 => 2 FLOOR  3 EXISTS (0 = false / 1 = true)
      //                 ||||└---->  4 => 3 FLOOR  2 EXISTS (0 = false / 1 = true)
      //                 |||└----->  3 => 4 FLOOR  1 EXISTS (0 = false / 1 = true)
      //                 ||└------>  2 => 5 OBJECT 3 EXISTS (0 = false / 1 = true)
      //                 |└------->  1 => 6 ENEMY  2 EXISTS (0 = false / 1 = true)
      //                 └-------->  0 => 7 ENEMY  1 EXISTS (0 = false / 1 = true)

      roomToTransportTo = 0b00000000;
      //                    |||||||└->  \
      //                    ||||||└-->   |
      //                    |||||└--->   | these 6 bits are used for the roomnumber you'll go to
      //                    ||||└---->   |
      //                    |||└----->   |
      //                    ||└------>  /
      //                    |└-------> NOT USED
      //                    └--------> NOT USED

      roomNumberInfluencing = 0b00000000;
      //                        |||||||└->0  \
      //                        ||||||└-->1   |
      //                        |||||└--->2   | these 6 bits are used for the roomnumber where the elements are influenced
      //                        ||||└---->3   |
      //                        |||└----->4   |
      //                        ||└------>5  /
      //                        |└------->6 NOT USED
      //                        └-------->7 NOT USED

      roomNumberFromInfluencer = 0b00000000;
      //                           |||||||└->0  \
      //                           ||||||└-->1   |
      //                           |||||└--->2   | these 6 bits are used for the roomnumber where the elements are influenced
      //                           ||||└---->3   |
      //                           |||└----->4   |
      //                           ||└------>5  /
      //                           |└------->6 NOT USED
      //                           └-------->7 NOT USED

      elementsInfluenced = 0b00000000;
      //                     ||||||||
      //                     |||||||└->0 FLOOR  1 INFLUENCED (0 = false / 1 = true)
      //                     ||||||└-->1 FLOOR  2 INFLUENCED (0 = false / 1 = true)
      //                     |||||└--->2 FLOOR  3 INFLUENCED (0 = false / 1 = true)
      //                     ||||└---->3 FLOOR  4 INFLUENCED (0 = false / 1 = true)
      //                     |||└----->4 FLOOR  5 INFLUENCED (0 = false / 1 = true)
      //                     ||└------>5 ENEMY  1 INFLUENCED (0 = false / 1 = true)
      //                     |└------->6 ENEMY  2 INFLUENCED (0 = false / 1 = true)
      //                     └-------->7 OBJECT 3 INFLUENCED (0 = false / 1 = true)
    }
};

Room stageRoom[MAX_AMOUNT_OF_ROOMS];


void buildRooms(byte currentLevel)
{
  // let's read out in witch room the exit to the next level is
  exitRoomLocation = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][LEVEL_ROOM_DATA_START_AT_BYTE]);

  byte amountOfRooms = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][AMOUNT_OF_ROOMS_AT_BYTE]);
  int transportDataAtByte = ROOMS_DATA_START_AT_BYTE + (BYTES_USED_FOR_EVERY_ROOM * amountOfRooms);
  int influenceDataAtByte = transportDataAtByte + pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][AMOUNT_OF_TRANSPORTERS_AT_BYTE]);
  byte transporterCounter = 0;
  byte influenceDataCounter = 0;
  // start reading the data out off PROGMEM
  for (byte roomNumber = 0; roomNumber < amountOfRooms; roomNumber++)
  {
    // clear all info
    stageRoom[roomNumber].set();

    // now lets set all the data for each room in the current level from the datasheet
    // first set all the doors and if those are closed or open
    stageRoom[roomNumber].doorsClosedActive = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ROOMS_DATA_START_AT_BYTE + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]);

    // Second thing to do is to set the 8 elements active or inactive in each room (2 enemies, an object and 5 special floor tiles)
    for (byte i = 0; i < 8; i++)
    {
      if (pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]))
      { //0b76543210
        bitSet (stageRoom[roomNumber].elementsActive, 7 - i);    //0b12345678
      }
    }

    // Third thing to do is to set the transporter data in the correct room
    if ((pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + OBJECT + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]) & 0b00000111) == TELEPORT)
    {
      stageRoom[roomNumber].roomToTransportTo = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][transportDataAtByte + transporterCounter]);
      transporterCounter++;
    }
    // Fourth thing to do is to set in which room an element is influenced, where the influencer is and what elementes are influenced
    if ((pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + OBJECT + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]) & 0b00000111) > TELEPORT)
    {
      stageRoom[roomNumber].roomNumberInfluencing = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][influenceDataAtByte + influenceDataCounter]);
      stageRoom[roomNumber].roomNumberFromInfluencer = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][influenceDataAtByte + influenceDataCounter + 1]);
      stageRoom[roomNumber].elementsInfluenced = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][influenceDataAtByte + influenceDataCounter + 2]);
      influenceDataCounter += 3;
      if ((pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + OBJECT + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]) & 0b00000111) == SWITCH_ON)
        bitSet(stageRoom[roomNumber].roomNumberFromInfluencer, 7);
    }
  }
}


byte checkIfLevelDoor()
{
  byte test = pgm_read_byte(&levels[level-LEVEL_OFFSET][LEVEL_DOOR_DATA_START_AT_BYTE]);
  if (currentRoom == ((test & 0b1111100)>>2)) return (test & 0b00000011);
}

byte tileFromXY(byte x, byte y)
{
  unsigned int x45 = x * 170;
  unsigned int y45 = y * 341;
  x = (((y45 - x45) >> 8) + 18);
  y = (((y45 + x45) >> 8) - 49);
  return (y >> 4) * 5 + (x >> 4);
}


bool isoCollide(byte ax, byte ay, byte bx, byte by, byte max)
{
  return (DIFF(ax, bx) + DIFF(ay, by)) < max;
}


int translateTileToX (byte currentTile)
{
  return ((((((currentTile / 5) * 6) + 4) - currentTile) * 12) + 3);
}


int translateTileToY (byte currentTile)
{
  return (18 + (currentTile * 6) - ((currentTile / 5) * 24));
}


bool checkIfOnCenterTile (byte coX, byte coY)
{
  for (byte y = 0; y < 5; y++)
  {
    for (byte x = 0; x < 5; x++)
    {
      if (coX == 3 + (48 - (12 * x) + (12 * y)) && (coY == (27 + (6 * x) + (6 * y)) - 9))
      {
        return true;
      }
    }
  }
  return false;
}


/*
bool checkIfOnCenterTile(byte coX, byte coY)
{
  byte tx = 51;   // starting X for y=0, x=0
  byte ty = 18;   // starting Y

  for (byte y = 0; y < 5; y++)
  {
    byte cx = tx;
    byte cy = ty;

    for (byte x = 0; x < 5; x++)
    {
      if (coX == cx && coY == cy)
        return true;

      cx -= 12;
      cy += 6;
    }

    tx += 12;
    ty += 6;
  }

  return false;
}
*/

void enterRoom(byte roomNumber, byte currentLevel)
{
  playerShot.active = false;
  enemyBulletActive = false;
  for (byte i = 0; i < 8; i++)
  {
    elements[i].characteristics = 0;
    if (bitRead (stageRoom[roomNumber].elementsActive, 7 - i))
    {
      byte currentTile = (pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)])) >> 3;
      elements[i].characteristics = ((pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)])));
      if (currentTile > 24) elements[i].characteristics = 0;
      elements[i].x = translateTileToX(currentTile);
      elements[i].y = translateTileToY(currentTile);
    }
  }
  if ((elements[OBJECT].characteristics & 0b00000111) >= SWITCH_OFF)
  {
    if (bitRead(stageRoom[roomNumber].roomNumberFromInfluencer, 7))
      bitSet(elements[OBJECT].characteristics, 0);
    else
      bitClear(elements[OBJECT].characteristics, 0);
  }
}

byte transportToRoom (byte roomNumber)
{
  return stageRoom[roomNumber].roomToTransportTo;
}


byte goToRoom(byte roomNumber, byte currentLevel)
{
  // we know which door the player goes through by the direction the droid is facing
  byte door = player.characteristics & 0b00000011;
  return (pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][DOORS_DATA_START_AT_BYTE + door + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]) >> 2);
};


byte goToTile(byte roomNumber, byte currentLevel)
{
  // we know which door the player goes through by the direction the droid is facing
  byte door = player.characteristics & 0b00000011;
  byte doorGoingTo = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][DOORS_DATA_START_AT_BYTE + door + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]) & 0b00000011;
  switch (doorGoingTo)
  {
    case NORTH:
      return TILE_INFRONT_DOOR_NORTH;
      break;
    case EAST:
      return TILE_INFRONT_DOOR_EAST;
      break;
    case SOUTH:
      return TILE_INFRONT_DOOR_SOUTH;
      break;
    case WEST:
      return TILE_INFRONT_DOOR_WEST;
      break;
  }
}

int setCurrentRoomY(byte currentTile)
{
  // SOUTH or WEST → -35, otherwise -9
  return (currentTile == TILE_INFRONT_DOOR_SOUTH ||
          currentTile == TILE_INFRONT_DOOR_WEST) ? -35 : -9;
}

int offsetXAfterDoor(byte currentTile)
{
  // EAST or SOUTH → 10, otherwise -10
  return (currentTile == TILE_INFRONT_DOOR_EAST ||
          currentTile == TILE_INFRONT_DOOR_SOUTH) ? 10 : -10;
}

int offsetYAfterDoor(byte currentTile)
{
  // SOUTH or WEST → 5, otherwise -5
  return (currentTile == TILE_INFRONT_DOOR_SOUTH ||
          currentTile == TILE_INFRONT_DOOR_WEST) ? 5 : -5;
}

/////////////////  DRAW ROOM    ///////////////////
///////////////////////////////////////////////////
void drawNothing()
{
}

void drawFloor()
{
  for (byte y = 0; y < 5; y++)
  {
    for (byte x = 0; x < 5; x++)
    {
      if (x==2 && y == 2 && currentRoom == exitRoomLocation) 
      {
        // byte test = pgm_read_byte(&levels[currentLevel - LEVEL_OFFSET][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * roomNumber)]);
        // find in wath room the level exit is and only draw that there
        if ((arduboy.everyXFrames(8))) levelUpAnimation = (++levelUpAnimation % 3);
        sprites.drawPlusMask(48 - (12 * x) + (12 * y), currentRoomY + 27 + (6 * x) + (6 * y), floorTile_plus_mask, 5 + levelUpAnimation);
      }
      else sprites.drawPlusMask(48 - (12 * x) + (12 * y), currentRoomY + 27 + (6 * x) + (6 * y), floorTile_plus_mask, 0);
    }
  }
}

void drawTicker()
{
  //if (bitRead(setTicker,0)==1)
  {
    byte y=0;
    byte x=0;
    for (byte  w= 0;w<59;w++ )
    {
      for (byte z = 0;z<2;z++)
      {
        sprites.drawSelfMasked(x, currentRoomY + 38 - y, letterPartsNew, charBox[x]);
        x++;
      }
      (w < 29) ? y++ : y--;
    }
   // Serial.println(setTicker);
    //if (setTicker == TEXT_SCROLL_LEFT)
    
    if ((arduboy.everyXFrames(8)))
    {
      //if (setTicker == TEXT_SCROLL_LEFT)
      {
        byte tempChar = charBox[0];          // save the first byte
          for (int i = 0; i < 119; i++)
          {
            charBox[i] = charBox[i + 1];
          }
          charBox[119] = tempChar;
      }
    }
    /*
  if ((arduboy.everyXFrames(8)))
  {
    switch (setTicker)
      {
        case TEXT_SCROLL_LEFT:
          {
            byte tempChar = charBox[0];          // save the first byte
            for (int i = 0; i < 119; i++)
            {
              charBox[i] = charBox[i + 1];
            }
            charBox[119] = tempChar;                        // put the saved byte at the end
          }
          break;
        case TEXT_SCROLL_RIGHT:
          {
            byte tempChar = charBox[120];          // save the first byte
            for (int i = 119; i > 0; i--)
            {
              charBox[i] = charBox[i - 1];
            }
            charBox[0] = tempChar;                        // put the saved byte at the end
          }
          break;
        }
      }
      */
  }
}

void drawWalls()
{
  for (byte x = 0; x < 6; x++)
  {
    sprites.drawSelfMasked( -2 + (10 * x), currentRoomY + 25 - (5 * x), wallParts, NORTH);
    sprites.drawSelfMasked(60 + (10 * x), currentRoomY + (5 * x), wallParts, EAST);
  }
  drawTicker();
}




///////////////// DRAW DOOR NORTH  ////////////////
///////////////////////////////////////////////////
void drawDoorLintelNorth()
{
  sprites.drawPlusMask(16, currentRoomY + 5, doorLintel_plus_mask, NORTH);
}

void drawDoorPostBigNorth()
{
  sprites.drawPlusMask(24, currentRoomY + 21, doorPostBig_plus_mask, NORTH);
}

void drawDoorPostSmallNorth()
{
  sprites.drawPlusMask(16, currentRoomY + 21, doorPostSmall_plus_mask, NORTH);
}

void drawDoorClossedNorth()
{
  sprites.drawPlusMask(24, currentRoomY + 15, doorClossed_plus_mask, NORTH);
  if (checkIfLevelDoor() == NORTH) sprites.drawPlusMask(24, currentRoomY + 16, doorClossed_plus_mask, NORTH);  // draw the clossed door 2 times for level door

}



/////////////////  DRAW DOOR EAST  ////////////////
///////////////////////////////////////////////////
void drawDoorLintelEast()
{
  sprites.drawPlusMask(80, currentRoomY + 5, doorLintel_plus_mask, EAST);
}

void drawDoorPostBigEast()
{
  sprites.drawPlusMask(80, currentRoomY + 21, doorPostBig_plus_mask, EAST);
}

void drawDoorPostSmallEast()
{
  sprites.drawPlusMask(95, currentRoomY + 21, doorPostSmall_plus_mask, EAST);
}

void drawDoorClossedEast()
{
  sprites.drawPlusMask(85, currentRoomY + 15, doorClossed_plus_mask, EAST);
if (checkIfLevelDoor() == EAST) sprites.drawPlusMask(85, currentRoomY + 16, doorClossed_plus_mask, EAST);  // draw the clossed door 2 times for level door

}



/////////////////  DRAW DOOR SOUTH  ///////////////
///////////////////////////////////////////////////
void drawDoorLintelSouth()
{
  sprites.drawPlusMask(81, currentRoomY + 38, doorLintel_plus_mask, SOUTH);
}

void drawDoorPostBigSouth()
{
  sprites.drawPlusMask(89, currentRoomY + 54, doorPostBig_plus_mask, SOUTH);
}

void drawDoorPostSmallSouth()
{
  sprites.drawPlusMask(81, currentRoomY + 54, doorPostSmall_plus_mask, SOUTH);
}

void drawDoorClossedSouth()
{
  sprites.drawPlusMask(89 , currentRoomY + 48, doorClossed_plus_mask, SOUTH);
  if (checkIfLevelDoor() == SOUTH) sprites.drawPlusMask(89 , currentRoomY + 49, doorClossed_plus_mask, SOUTH);  // draw the clossed door 2 times for level door
}



/////////////////   DRAW DOOR WEST  ///////////////
///////////////////////////////////////////////////
void drawDoorLintelWest()
{
  sprites.drawPlusMask(14, currentRoomY + 38, doorLintel_plus_mask, WEST);
}

void drawDoorPostBigWest()
{
  sprites.drawPlusMask(14, currentRoomY + 54, doorPostBig_plus_mask, WEST);
}

void drawDoorPostSmallWest()
{
  sprites.drawPlusMask(29, currentRoomY + 54, doorPostSmall_plus_mask, WEST);
}

void drawDoorClossedWest()
{
  sprites.drawPlusMask(19, currentRoomY + 48, doorClossed_plus_mask, WEST);
  if (checkIfLevelDoor() == WEST) sprites.drawPlusMask(19, currentRoomY + 49, doorClossed_plus_mask, WEST); // draw the clossed door 2 times for level door
}







typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM  updateElementsInRoom[] =
{
  drawEnemyOne,                     // 0
  drawEnemyTwo,                     // 1
  drawObject,                       // 2
  drawFloorOne,                     // 3
  drawFloorTwo,                     // 4
  drawFloorThree,                   // 5
  drawFloorFour,                    // 6
  drawFloorFive,                    // 7
  drawBulletEnemy,                  // 8

  drawPlayer,                       // 9
  drawBulletPlayer,                 // 10

  drawDoorLintelNorth,              // 11
  drawDoorPostBigNorth,             // 12
  drawDoorPostSmallNorth,           // 13
  drawDoorClossedNorth,             // 14

  drawDoorLintelEast,               // 15
  drawDoorPostBigEast,              // 16
  drawDoorPostSmallEast,            // 17
  drawDoorClossedEast,              // 18

  drawDoorLintelSouth,              // 19
  drawDoorPostBigSouth,             // 20
  drawDoorPostSmallSouth,           // 21
  drawDoorClossedSouth,             // 22

  drawDoorLintelWest,               // 23
  drawDoorPostBigWest,              // 24
  drawDoorPostSmallWest,            // 25
  drawDoorClossedWest,              // 26

  drawNothing,                      // 27
};


void drawRoom()
{
  drawWalls();
  drawFloor();
  for (byte i = 0; i < SIZE_OF_ITEMSORDER; i++)
  {
    ((FunctionPointer) pgm_read_word (&updateElementsInRoom[itemsOrder[i]]))();
  }
  drawBulletPlayer();
  drawBulletEnemy();
}


void checkOrderOfObjects(byte roomNumber, byte currentLevel)
{
  // clear out the itemsOrder
  memset(itemsOrder, EMPTY_PLACE, SIZE_OF_ITEMSORDER);

  //draw door NORTH
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_EXISTS))
  {
    itemsOrder[0] = NORTH_LINTEL;
    itemsOrder[1] = NORTH_BIG_POST;
    itemsOrder[3] = NORTH_SMALL_POST;
  }
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_IS_CLOSSED)) itemsOrder[4] = NORTH_DOOR_CLOSSED;


  //draw door EAST
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_EXISTS))
  {
    itemsOrder[5] = EAST_LINTEL;
    itemsOrder[6] = EAST_BIG_POST;
    itemsOrder[8] = EAST_SMALL_POST;
  }
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_IS_CLOSSED)) itemsOrder[9] = EAST_DOOR_CLOSSED;


  //draw door SOUTH
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_EXISTS))
  {
    itemsOrder[35] = SOUTH_LINTEL;
    itemsOrder[36] = SOUTH_BIG_POST;
    itemsOrder[38] = SOUTH_SMALL_POST;
  }
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_IS_CLOSSED)) itemsOrder[39] = SOUTH_DOOR_CLOSSED;


  //draw door WEST
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_EXISTS))
  {
    itemsOrder[40] = WEST_LINTEL;
    itemsOrder[41] = WEST_BIG_POST;
    itemsOrder[43] = WEST_SMALL_POST;
  }
  if (bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_IS_CLOSSED)) itemsOrder[44] = WEST_DOOR_CLOSSED;


  //******************************
  //determine what is on the tiles
  //******************************
  // check what tile the player is on (so that we can determine what order things need to be displayed)
  // elements first, then the droid so a box on the same tile cannot hide him
  for (byte i = 0; i < 8; i++)
  {
   if (bitRead(stageRoom[currentRoom].elementsActive, 7 - i))itemsOrder[tileFromXY(elements[i].x, elements[i].y) + ITEMS_ORDER_TILES_START] = i;
  }

  if (!bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5) && !bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6))
  {
    itemsOrder[player.isOnTile + ITEMS_ORDER_TILES_START] = PLAYER_DROID;
  }
  else
  {
    switch (player.characteristics & 0b00000011)
    {
      case NORTH:
        if (bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5)) itemsOrder[2] = PLAYER_DROID;
        if (bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6)) itemsOrder[37] = PLAYER_DROID;
        break;
      case EAST:
        if (bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5)) itemsOrder[7] = PLAYER_DROID;
        if (bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6)) itemsOrder[37] = PLAYER_DROID;
        break;
      case SOUTH:
        if (bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5)) itemsOrder[37] = PLAYER_DROID;
        if (bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6)) itemsOrder[2] = PLAYER_DROID;
        break;
      case WEST:
        if (bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5)) itemsOrder[42] = PLAYER_DROID;
        if (bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6)) itemsOrder[7] = PLAYER_DROID;
        break;
    }
  }
}

void drawNumbers(byte x, byte y, unsigned long numbers, byte fontType)
{
  char buf[10];
  ltoa(numbers, buf, 10);
  //itoa(arduboy.cpuLoad(), buf, 10);
  char charLen = strlen(buf);
  char pad = (7 - (5 * fontType)) - charLen;

  if (fontType == BIG_FONT) for (byte i = 0; i < pad; i++) sprites.drawSelfMasked(43 + (6 * i), 54, numbersBig, 0);
  else if (pad > 0) sprites.drawSelfMasked(121, 0, numbersThin, 0);


  //draw remaining digits
  for (byte i = 0; i < charLen; i++)
  {
    char digit = buf[i];
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }
    switch (fontType)
    {
      case BIG_FONT:
        sprites.drawSelfMasked(x + (pad * 6) + (6 * i), 54, numbersBig, digit);
        break;
      case THIN_FONT:
        sprites.drawSelfMasked(x + (pad * 4) + (4 * i), y, numbersThin, digit);
        break;
      case SMALL_FONT:
        sprites.drawSelfMasked(x + (4 * i), y, numbersSmall, digit);
    }

  }
}

void drawHUD()
{
  //draw HUD mask
  for (byte y = 0; y < 8; y++) sprites.drawPlusMask(118, y * 8, hudMask_plus_mask, 0);

  //draw room number
  drawNumbers(121, 0, currentRoom, THIN_FONT);

  //draw amount of bullets
  drawNumbers(123, 23, (player.assets & 0b00000111), SMALL_FONT);
  sprites.drawSelfMasked(122, 29, hudBullet, 0);

  //draw amount of white cards
  drawNumbers(123, 38, (player.assets & 0b00011000) >> 3, SMALL_FONT);
  sprites.drawSelfMasked(121, 44, hudWhiteCard, 0);

  //draw amount of black cards
  //drawNumbers(123, 53, (player.assets & 0b00100000) >> 5, SMALL_FONT);
  drawNumbers(123, 53,((bitRead(player.assets,DROID_HAS_BLACK_CARD_AT_BIT_5)) == 0) ? 0 : 1, SMALL_FONT);
  sprites.drawSelfMasked(121, 59, hudBlackCard, 0);

  //draw life
  if bitRead(player.characteristics,DROID_DYING_AT_BIT_4) bitSet(player.assets,DROID_BATTERY_VISIBLE_AT_BIT_6);
  else if (arduboy.everyXFrames(20) && (player.life < 2)) bitToggle(player.assets,DROID_BATTERY_VISIBLE_AT_BIT_6);
  if (bitRead(player.assets, DROID_BATTERY_VISIBLE_AT_BIT_6)) sprites.drawSelfMasked(122, 11, hudLife, player.life);
}


#endif
