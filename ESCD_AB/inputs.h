#ifndef INPUTS_H
#define INPUTS_H

#include "globals.h"
#include "player.h"
#include "collision.h"

#define BUTTON_SCHEME_A  0
#define BUTTON_SCHEME_B  4

PROGMEM const unsigned char buttonScheme[] = {
  UP_BUTTON, RIGHT_BUTTON, DOWN_BUTTON, LEFT_BUTTON,
  LEFT_BUTTON, UP_BUTTON, RIGHT_BUTTON, DOWN_BUTTON
};

// Relative tile offsets for teleport / switch when facing a direction
// NORTH=-5, EAST=-1, SOUTH=+5, WEST=+1
PROGMEM const int8_t dirTileOffset[] = { -5, -1, 5, 1 };

// Door tiles in front of the player for each direction
PROGMEM const byte doorTile[] = {
  TILE_INFRONT_DOOR_NORTH,
  TILE_INFRONT_DOOR_EAST,
  TILE_INFRONT_DOOR_SOUTH,
  TILE_INFRONT_DOOR_WEST
};

void checkInputs()
{
  // ----- Movement (every 2 frames) -----
  if (arduboy.everyXFrames(2))
  {
    byte testingTile = tileFromXY(player.x, player.y - currentRoomY);
    if (testingTile < 25) player.isOnTile = testingTile;

    byte dir = 255;   // 255 = no direction pressed

    if      (arduboy.pressed(pgm_read_byte(&buttonScheme[NORTH + buttonSchemeOffset]))) dir = NORTH;
    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[EAST  + buttonSchemeOffset]))) dir = EAST;
    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[SOUTH + buttonSchemeOffset]))) dir = SOUTH;
    else if (arduboy.pressed(pgm_read_byte(&buttonScheme[WEST  + buttonSchemeOffset]))) dir = WEST;

    if (dir != 255)
    {
      // Set facing direction (lowest 2 bits)
      player.characteristics = (player.characteristics & 0b11111100) | dir;

      if (!checkborderHit(player.x, player.y, dir))
      {
        if (hitObjects(player.x, player.y - currentRoomY, dir, PLAYER, 0))
          decideOnCollision();
        else
          walkThroughDoor();
      }
    }
  }

  // ----- A button = Pause -----
  if (arduboy.justPressed(A_BUTTON))
    gameState = STATE_GAME_PAUSE;

  // ----- B button = Action -----
  if (arduboy.justPressed(B_BUTTON))
    {
    // Special case: stand on exit tile → next level
    if (currentRoom == exitRoomLocation && player.isOnTile == TILE_IN_MIDDLE)
    {
      gameState = STATE_GAME_NEXT_LEVEL;
      return;
    }

    byte dir = player.characteristics & 0b00000011;   // current facing

    // Open normal / level door if standing in front of it
    if (player.isOnTile == pgm_read_byte(&doorTile[dir]))
    {
      playerChecksAndOpensDoor(dir);
      playerChecksAndOpensLevelDoor(dir);
    }

    // Teleport or Switch
    byte objType = elements[OBJECT].characteristics & 0b00000111;
    byte objTile = (elements[OBJECT].characteristics & 0b11111000) >> 3;
    int8_t neededTile = player.isOnTile + (int8_t)pgm_read_byte(&dirTileOffset[dir]);

    if (objTile == neededTile)
    {
      if (objType == TELEPORT)
      {
        bitSet(player.characteristics, DROID_TRANSPORTING_AT_BIT_7);
        gameState = STATE_GAME_TRANSPORTING;
      }
      else if (objType > 5)   // SWITCH_OFF or SWITCH_ON
      {
        bitToggle(elements[OBJECT].characteristics, 0);
        byte targetRoom = stageRoom[currentRoom].roomNumberInfluencing;
        byte mask       = stageRoom[currentRoom].elementsInfluenced;
        stageRoom[targetRoom].elementsActive ^= mask;
      }
    }

    // (future: shoot bullet here)
  }
}

//// Moving the Enemies ////
void moveEnemies(int enemyX, int enemyY, byte directionFacing, bool enemy)
{
  switch (directionFacing)
  {
    case NORTH: elements[enemy].y -= 1; elements[enemy].x -= 2; break;
    case EAST:  elements[enemy].y -= 1; elements[enemy].x += 2; break;
    case SOUTH: elements[enemy].y += 1; elements[enemy].x += 2; break;
    case WEST:  elements[enemy].y += 1; elements[enemy].x -= 2; break;
  }
}

void enemyTurn(bool enemy, bool leftOrRight)
{
  byte test = (((elements[enemy].characteristics & 0b00011000) >> 3) - 1 + (leftOrRight * 2)) & 0b00000011;
  elements[enemy].characteristics = (elements[enemy].characteristics & 0b11100111) | (test << 3);
}

void updateEnemies()
{
  if (!arduboy.everyXFrames(6)) return;

  for (byte i = 0; i < 2; i++)
  {
    elements[i].frame = (elements[i].frame + 1) & 3;   // cheaper than % 4

    if (!pgm_read_byte(&levels[level - 1][ELEMENTS_DATA_START_AT_BYTE + i + (BYTES_USED_FOR_EVERY_ROOM * currentRoom)]))
      continue;

    byte dir = (elements[i].characteristics & 0b00011000) >> 3;
    byte type = elements[i].characteristics & 0b00000111;

    bool canMove = !hitBorders(elements[i].x, elements[i].y, dir, ENEMY) &&
                   !hitObjects(elements[i].x, elements[i].y, dir, ENEMY, i);

    switch (type)
    {
      case ENEMY_BOX:
        if (canMove) moveEnemies(elements[i].x, elements[i].y, dir, i);
        else         enemyTurn(i, TURN_RIGHT);
        break;

      case ENEMY_JUMPER:
        if (canMove)
        {
          moveEnemies(elements[i].x, elements[i].y, dir, i);
          if (checkIfOnCenterTile(elements[i].x, elements[i].y))
          {
            byte leftDir = (dir - 1) & 3;
            if (!hitBorders(elements[i].x, elements[i].y, leftDir, ENEMY) &&
                !hitObjects(elements[i].x, elements[i].y, leftDir, ENEMY, i))
            {
              enemyTurn(i, TURN_LEFT);
            }
          }
        }
        else enemyTurn(i, TURN_RIGHT);
        break;

      case ENEMY_MOVER:
        if (canMove) moveEnemies(elements[i].x, elements[i].y, dir, i);
        else         enemyTurn(i, TURN_LEFT);
        break;

      case ENEMY_SHOOTER:
        break;
    }
  }
}

#endif