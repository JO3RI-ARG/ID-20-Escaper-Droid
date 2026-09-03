#ifndef INPUTS_H
#define INPUTS_H

#include "globals.h"
#include "player.h"
#include "collision.h"

#define BUTTON_SCHEME_A            0            // position of the first buttonsheme in the array
#define BUTTON_SCHEME_B            4            // position of the second buttonscheme in the array

PROGMEM const unsigned char buttonScheme[] =
{
  UP_BUTTON, RIGHT_BUTTON, DOWN_BUTTON, LEFT_BUTTON,
  LEFT_BUTTON, UP_BUTTON, RIGHT_BUTTON, DOWN_BUTTON
};


void checkInputs()
{
  if (arduboy.everyXFrames(2))
  {
    byte testingTile = tileFromXY(player.x, player.y - currentRoomY);
    if (testingTile < 25) player.isOnTile = tileFromXY(player.x, player.y - currentRoomY);
    if (arduboy.pressed(pgm_read_byte(&buttonScheme[NORTH + buttonSchemeOffset])))
    {
      bitClear(player.characteristics, 0);
      bitClear(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, NORTH))
      {
        if (hitObjects(player.x, player.y - currentRoomY, NORTH, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }


    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[EAST + buttonSchemeOffset])))
    {
      bitSet(player.characteristics, 0);
      bitClear(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, EAST))
      {
        if (hitObjects(player.x, player.y - currentRoomY, EAST, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }

    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[SOUTH + buttonSchemeOffset])))
    {
      bitClear(player.characteristics, 0);
      bitSet(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, SOUTH))
      {
        if (hitObjects(player.x, player.y - currentRoomY, SOUTH, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }

    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[WEST + buttonSchemeOffset])))
    {
      bitSet(player.characteristics, 0);
      bitSet(player.characteristics, 1);
      if (!checkborderHit(player.x, player.y, WEST))
      {
        if (hitObjects(player.x, player.y - currentRoomY, WEST, PLAYER, 0))
        {
          decideOnCollision();
        }
        else walkThroughDoor();
      }
    }
  }

  if (arduboy.justPressed(A_BUTTON))gameState = STATE_GAME_PAUSE;
  if (arduboy.justPressed(B_BUTTON))
  {
    if (currentRoom == exitRoomLocation && player.isOnTile == TILE_IN_MIDDLE)
    {
      gameState = STATE_GAME_NEXT_LEVEL;
    }
    else switch (player.characteristics & 0b00000011)  // check what direction droid is facing
    {
      case NORTH:
        // OPEN A DOOR/LEVELDOOR IF DROID HAS A WHITE/BLACK CARD
        if (player.isOnTile == TILE_INFRONT_DOOR_NORTH);
        {
          playerChecksAndOpensDoor(NORTH);             // OPEN A DOOR IF DROID HAS A WHITE CARD WHITE CARD
          playerChecksAndOpensLevelDoor(NORTH);        // OPEN THE LEVEL DOOR IF DROID HAS A BLACK CARD
        }
        // TRANSPORT
        // Has currentRoom a transporter
        if ((elements[OBJECT].characteristics & 0b00000111)==TELEPORT)
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3) == (player.isOnTile - 5))
          {
            bitSet(player.characteristics,DROID_TRANSPORTING_AT_BIT_7);
            gameState = STATE_GAME_TRANSPORTING;
          }
        }
        // SWITCH ON/OFF
        // Has currentRoom a Switch ON/OFF
        if (((elements[OBJECT].characteristics & 0b00000111) > 0b00000101))
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3) == (player.isOnTile - 5))
          {
            // toggle the switch
            bitToggle(elements[OBJECT].characteristics,0); // we need to keep to switch toggled when leaving the room
            byte inWhatRoomToSwitch = stageRoom[currentRoom].roomNumberInfluencing;
            byte elementsToSwitch = stageRoom[currentRoom].elementsInfluenced;
            stageRoom[inWhatRoomToSwitch].elementsActive ^= elementsToSwitch;
          }
        }
        // SHOOT BULLET
        break;
      case EAST:
        // OPEN A DOOR/LEVELDOOR IF DROID HAS A WHITE/BLACK CARD
        if (player.isOnTile == TILE_INFRONT_DOOR_EAST)
        {
          playerChecksAndOpensDoor(EAST);             // OPEN A DOOR IF DROID HAS A WHITE CARD WHITE CARD
          playerChecksAndOpensLevelDoor(EAST);        // OPEN THE LEVEL DOOR IF DROID HAS A BLACK CARD
        }
        // TRANSPORT
        if ((elements[OBJECT].characteristics & 0b00000111)==TELEPORT)
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3)== (player.isOnTile - 1))
          {
            bitSet(player.characteristics,DROID_TRANSPORTING_AT_BIT_7);
            gameState = STATE_GAME_TRANSPORTING;
          }
        }
        // SWITCH ON/OFF
        if (((elements[OBJECT].characteristics & 0b00000111) > 0b00000101))
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3) == (player.isOnTile - 1))
          {
            bitToggle(elements[OBJECT].characteristics,0); // we need to keep to switch toggled when leaving the room
            byte inWhatRoomToSwitch = stageRoom[currentRoom].roomNumberInfluencing;
            byte elementsToSwitch = stageRoom[currentRoom].elementsInfluenced;
            stageRoom[inWhatRoomToSwitch].elementsActive ^= elementsToSwitch;
          }
        }
        // SHOOT BULLET
        break;
      case SOUTH:
      // OPEN A DOOR/LEVELDOOR IF DROID HAS A WHITE/BLACK CARD
        if (player.isOnTile == TILE_INFRONT_DOOR_SOUTH)
        {
          playerChecksAndOpensDoor(SOUTH);             // OPEN A DOOR IF DROID HAS A WHITE CARD WHITE CARD
          playerChecksAndOpensLevelDoor(SOUTH);        // OPEN THE LEVEL DOOR IF DROID HAS A BLACK CARD
        }
        // TRANSPORT
           if ((elements[OBJECT].characteristics & 0b00000111)==TELEPORT)
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3)== (player.isOnTile + 5))
          {
            bitSet(player.characteristics,DROID_TRANSPORTING_AT_BIT_7);
            gameState = STATE_GAME_TRANSPORTING;
          }
        }
        // SWITCH ON/OFF
        if (((elements[OBJECT].characteristics & 0b00000111) > 0b00000101))
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3) == (player.isOnTile + 5))
          {
            bitToggle(elements[OBJECT].characteristics,0); // we need to keep to switch toggled when leaving the room
            byte inWhatRoomToSwitch = stageRoom[currentRoom].roomNumberInfluencing;
            byte elementsToSwitch = stageRoom[currentRoom].elementsInfluenced;
            stageRoom[inWhatRoomToSwitch].elementsActive ^= elementsToSwitch;
          }
        }
        // SHOOT BULLET
        break;
      case WEST:
      // OPEN A DOOR/LEVELDOOR IF DROID HAS A WHITE/BLACK CARD
        if (player.isOnTile == TILE_INFRONT_DOOR_WEST)
        {
          playerChecksAndOpensDoor(WEST);             // OPEN A DOOR IF DROID HAS A WHITE CARD WHITE CARD
          playerChecksAndOpensLevelDoor(WEST);        // OPEN THE LEVEL DOOR IF DROID HAS A BLACK CARD
        }
        // TRANSPORT
           if ((elements[OBJECT].characteristics & 0b00000111)==TELEPORT)
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3)== (player.isOnTile + 1))
          {
            bitSet(player.characteristics,DROID_TRANSPORTING_AT_BIT_7);
            gameState = STATE_GAME_TRANSPORTING;
          }
        }
        // SWITCH ON/OFF
        if (((elements[OBJECT].characteristics & 0b00000111) > 0b00000101))
        {
          if (((elements[OBJECT].characteristics & 0b11111000)>>3) == (player.isOnTile + 1))
          {
            bitToggle(elements[OBJECT].characteristics,0); // we need to keep to switch toggled when leaving the room
            byte inWhatRoomToSwitch = stageRoom[currentRoom].roomNumberInfluencing;
            byte elementsToSwitch = stageRoom[currentRoom].elementsInfluenced;
            stageRoom[inWhatRoomToSwitch].elementsActive ^= elementsToSwitch;
          }
        }
        // SHOOT BULLET
        break;
    }
    
    // do an action
    // 
    // possible actions are:
    // switch a switch
    // shoot a bullet
    // use a black key card
    // use a white key card
  }
}

////Moving the Enemies /////
////////////////////////////
void moveEnemies(int enemyX, int enemyY, byte directionFacing, bool enemy)
{
  switch (directionFacing)
  {
    case NORTH:
      elements[enemy].y -= 1;
      elements[enemy].x -= 2;
      break;
    case EAST:
      elements[enemy].y -= 1;
      elements[enemy].x += 2;
      break;
    case SOUTH:
      elements[enemy].y += 1;
      elements[enemy].x += 2;
      break;
    case WEST:
      elements[enemy].y += 1;
      elements[enemy].x -= 2;
      break;
  }
}

void enemyTurn(bool enemy, bool leftOrRight)
{
  byte test = (((elements[enemy].characteristics & 0b00011000) >> 3) - 1 + (leftOrRight * 2)) & 0b00000011;;
  elements[enemy].characteristics = (elements[enemy].characteristics & 0b11100111) + (test << 3);
}


void updateEnemies()
{
  if (arduboy.everyXFrames(6))
  {
    for (byte i = 0; i < 2; i++)
    {
      elements[i].frame = ((++elements[i].frame) % 4);
      if (pgm_read_byte(&levels[level - 1][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * currentRoom)]))
      {
        switch (elements[i].characteristics & 0b00000111)
        {
          case ENEMY_BOX:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
            }
            else enemyTurn(i, TURN_RIGHT);
            break;
          case ENEMY_JUMPER:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
              if (checkIfOnCenterTile (elements[i].x, elements[i].y))
              {
                // check if left is an empty tile
                if (!hitBorders(elements[i].x, elements[i].y, ((elements[i].characteristics & 0b00011000) >> 3) - 1, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, ((elements[i].characteristics & 0b00011000) >> 3) - 1, ENEMY, i)))
                {
                  enemyTurn(i, TURN_LEFT);
                }
              }
            }
            else enemyTurn(i, TURN_RIGHT);
            break;
          case ENEMY_MOVER:
            if (!hitBorders(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY) && (!hitObjects(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, ENEMY, i)))
            {
              moveEnemies(elements[i].x, elements[i].y, (elements[i].characteristics & 0b00011000) >> 3, i);
            }
            else enemyTurn(i, TURN_LEFT);
            break;
          case ENEMY_SHOOTER:
            break;
        }
      }
    }
  }
}


#endif
