#ifndef COLLISION_H
#define COLLISION_H

#include "globals.h"
#include "elements.h"
#include "player.h"
#include "room.h"

void isoStep(int &x, int &y, byte dir)
{
  dir &= 3;
  x += (dir == EAST || dir == SOUTH) ? 2 : -2;
  y += (dir > EAST) ? 1 : -1;
}

boolean hitBorders(int objectX, int objectY, int directionFacing, bool playerOrEnemy)
{
  if (directionFacing < 0) directionFacing = 3;
  if (directionFacing > 3) directionFacing = 0;
  switch (directionFacing)
  {
    case NORTH:
      if (objectX + (2 * objectY) > 89 + (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case EAST:
      if (objectX - (2 * objectY) < 15 - (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case SOUTH:
      if (objectX + (2 * objectY) < 183 + (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
    case WEST:
      if (objectX - (2 * objectY) > -81 - (playerOrEnemy * (2 * currentRoomY))) return false;
      break;
  }
  return true;
}

void playerChecksAndOpensDoor(byte direction)
{
  if ((player.assets & 0b00011000) && (checkIfLevelDoor() != direction))
  {
    player.assets -= 0b00001000;
    scorePlayer += SCORE_OPEN_DOOR;
    bitClear(stageRoom[currentRoom].doorsClosedActive, direction);
  }
}

void playerChecksAndOpensLevelDoor(byte direction)
{
  if ((player.assets & 0B00100000) && (checkIfLevelDoor() == direction))
  {
    player.assets -= 0b00100000;
    scorePlayer += SCORE_LEVEL_DOOR;
    bitClear(stageRoom[currentRoom].doorsClosedActive, direction);
  }
}

void setPlayerWalkingThroughDoor()
{
  switch (player.characteristics & 0b00000011)
  {
    case NORTH:
      if ((player.isOnTile == TILE_INFRONT_DOOR_NORTH) &&
          (bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_EXISTS)) &&
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_IS_CLOSSED)))
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);
        player.x = translateTileToX(2);
        player.y = translateTileToY(2) + currentRoomY;
      }
      break;
    case EAST:
      if ((player.isOnTile == TILE_INFRONT_DOOR_EAST) &&
          (bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_EXISTS)) &&
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_IS_CLOSSED)))
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);
        player.x = translateTileToX(10);
        player.y = translateTileToY(10) + currentRoomY;
      }
      break;
    case SOUTH:
      if ((player.isOnTile == TILE_INFRONT_DOOR_SOUTH) &&
          (bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_EXISTS)) &&
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_IS_CLOSSED)))
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);
        player.x = translateTileToX(22);
        player.y = translateTileToY(22) + currentRoomY;
      }
      break;
    case WEST:
      if ((player.isOnTile == TILE_INFRONT_DOOR_WEST) &&
          (bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_EXISTS)) &&
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_IS_CLOSSED)))
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);
        player.x = translateTileToX(14);
        player.y = translateTileToY(14) + currentRoomY;
      }
      break;
  }
}

boolean checkborderHit(int objectX, int objectY, byte directionFacing)
{
  if (!hitBorders(objectX, objectY, directionFacing, PLAYER)) return false;
  else setPlayerWalkingThroughDoor();
  return true;
}

byte tileIsOccupied(byte tileTesting, bool playerOrEnemy, bool enemyTwo)
{
  if (tileTesting < 25)
  {
    currentlyOnTestingTile = itemsOrder[tileTesting + ITEMS_ORDER_TILES_START];
    if (currentlyOnTestingTile == EMPTY_PLACE) return false;
    if (playerOrEnemy)
    {
      if (currentlyOnTestingTile == PLAYER_DROID) return false;
    }
    else
    {
      if (!enemyTwo && currentlyOnTestingTile == ENEMY_ONE) return false;
      else if (enemyTwo && currentlyOnTestingTile == ENEMY_TWO) return false;
      else if (currentlyOnTestingTile == PLAYER_DROID) return PLAYER_DROID;
    }
    return true;
  }
  else return false;
}

boolean hitObjects (int objectX, int objectY, int directionFacing, bool playerOrEnemy, bool enemy)
{
  if (directionFacing < 0) directionFacing = 3;
  if (directionFacing > 3) directionFacing = 0;
  switch (directionFacing)
  {
    case NORTH:
      testingTile = tileFromXY(objectX - 8, objectY - 4);
      break;
    case EAST:
      testingTile = tileFromXY(objectX + 8, objectY - 4);
      break;
    case SOUTH:
      testingTile = tileFromXY(objectX + 6, objectY + 3);
      break;
    case WEST:
      testingTile = tileFromXY(objectX - 6, objectY + 3);
      break;
  }
  byte test = tileIsOccupied(testingTile, playerOrEnemy, enemy);
  if (test > 0)
  {
    if (test == PLAYER_DROID) playerLosesLife();
    return true;
  }
  else return false;
}

void clearElement()
{
  bitClear(stageRoom[currentRoom].elementsActive, 5);
}

void checkObjectTypeAndAct()
{
  switch ((elements[2].characteristics & 0b00000111))
  {
    case PICKUP_BLACK_CARD:
      if (bitRead(player.assets,5) == 0)
      {
        bitSet(player.assets,5);
        clearElement();
        scorePlayer += SCORE_BLACK_CARD;
      }
      break;
    case PICKUP_WHITE_CARD:
      if ((player.assets & 0b00011000) < 0b00011000)
      {
        player.assets += 0b00001000;
        clearElement();
        scorePlayer += SCORE_WHITE_CARD;
      }
      break;
    case PICKUP_BATTERY:
      if (player.life < 3)
      {
        player.life++;
        clearElement();
        scorePlayer += SCORE_LIFE;
      }
      else
      {
        clearElement();
        scorePlayer += SCORE_TO_MUCH_LIFE;
      }
      break;
    case PICKUP_BULLET:
      if ((player.assets & 0b00000111) < 0b00000111)
      {
        player.assets++;
        objectHiddenThisVisit = true;   // gone until you leave the room
        scorePlayer += SCORE_BULLET;
      }
      break;
    case PICKUP_CHIP:
      clearElement();
      scorePlayer += SCORE_CHIP;
      break;
  }
}

void playerTouchesHazard()
{
  playerLosesLife();
}

byte floorKind(byte floorSlot)
{
  return elements[floorSlot].characteristics & 0b00000111;
}

boolean tryPushBox(byte slot, byte dir)
{
  if (floorKind(slot) != FLOOR_BOX) return false;
  dir &= 3;
  if (!checkIfOnCenterTile(elements[slot].x, elements[slot].y)) return false;

  byte src = tileFromXY(elements[slot].x, elements[slot].y);
  if (src >= 25) return false;
  byte col = src % 5;
  if ((dir == NORTH && src < 5) ||
      (dir == SOUTH && src >= 20) ||
      (dir == EAST  && col == 0) ||
      (dir == WEST  && col == 4))
    return false;

  int8_t dest = (int8_t)src;
  if (dir == NORTH) dest -= 5;
  else if (dir == EAST) dest -= 1;
  else if (dir == SOUTH) dest += 5;
  else dest += 1;
  if (dest < 0 || dest > 24) return false;
  if (itemsOrder[dest + ITEMS_ORDER_TILES_START] != EMPTY_PLACE) return false;

  // kick: snap one full tile
  elements[slot].x = translateTileToX(dest);
  elements[slot].y = translateTileToY(dest);
  return true;
}

void decideOnCollision()
{
  switch (currentlyOnTestingTile)
  {
    case ENEMY_ONE:
    case ENEMY_TWO:
      playerTouchesHazard();
      break;
    case OBJECT:
      checkObjectTypeAndAct();
      break;
    case FLOOR_ONE:
    case FLOOR_TWO:
    case FLOOR_THREE:
    case FLOOR_FOUR:
    case FLOOR_FIVE:
      {
        byte kind = floorKind(currentlyOnTestingTile);
        if (kind == FLOOR_BOX)
        {
          // walking into a box just stops you; kick it with B
        }
        else if (kind == FLOOR_SPIKE)
          playerTouchesHazard();
        // pits no longer hurt the droid
      }
      break;
  }
}

void stepShot(int &sx, int &sy, byte dir)
{
  isoStep(sx, sy, dir);
}

void killEnemy(byte enemySlot)
{
  bitClear(stageRoom[currentRoom].elementsActive, 7 - enemySlot);
  elements[enemySlot].characteristics = 0;
  scorePlayer += SCORE_ENEMY_HIT;
}

byte tileOccupant(int ox, int oy)
{
  byte t = tileFromXY(ox, oy);
  if (t >= 25) return EMPTY_PLACE;
  return itemsOrder[t + ITEMS_ORDER_TILES_START];
}

bool shotHitsBlockingFloor(byte occupant)
{
  if (occupant < FLOOR_ONE || occupant > FLOOR_FIVE) return false;
  byte kind = floorKind(occupant);
  if (kind == FLOOR_PIT) return false;
  if (kind == FLOOR_PIRAMIDE)
  {
    bitClear(stageRoom[currentRoom].elementsActive, 7 - occupant);
    elements[occupant].characteristics = 0;
    return true;
  }
  return (kind == FLOOR_BOX || kind == FLOOR_SPIKE);
}

bool resolveShotOnTile(int sx, int sy, byte dir, bool fromPlayer)
{
  if (hitBorders(sx, sy, dir, ENEMY))
    return true;

  byte occupant = tileOccupant(sx, sy);
  if (fromPlayer && (occupant == ENEMY_ONE || occupant == ENEMY_TWO))
  {
    if ((elements[occupant].characteristics & 0b00000111) == ENEMY_MOVER)
      return false;
    killEnemy(occupant);
    return true;
  }
  if (!fromPlayer && occupant == PLAYER_DROID)
  {
    playerLosesLife();
    return true;
  }
  if (shotHitsBlockingFloor(occupant) || occupant == OBJECT)
    return true;

  return false;
}

void updatePlayerShot()
{
  if (!playerShot.active) return;
  if (!arduboy.everyXFrames(2)) return;

  stepShot(playerShot.x, playerShot.y, playerShot.dir);
  playerShot.steps++;
  if (playerShot.steps < SHOT_STEPS_PER_TILE) return;
  playerShot.steps = 0;

  if (resolveShotOnTile(playerShot.x, playerShot.y, playerShot.dir, true))
    deactivatePlayerShot();
}

void spawnEnemyShot(byte enemySlot)
{
  if (enemyBulletActive) return;
  enemyBulletActive = true;
  elements[ENEMY_BULLET].x = elements[enemySlot].x;
  elements[ENEMY_BULLET].y = elements[enemySlot].y;
  elements[ENEMY_BULLET].characteristics = elements[enemySlot].characteristics & 0b00011000;
  elements[ENEMY_BULLET].frame = 0;
}

void deactivateEnemyShot()
{
  enemyBulletActive = false;
}

void updateEnemyShot()
{
  if (!enemyBulletActive) return;
  if (!arduboy.everyXFrames(2)) return;

  byte dir = (elements[ENEMY_BULLET].characteristics & 0b00011000) >> 3;
  stepShot(elements[ENEMY_BULLET].x, elements[ENEMY_BULLET].y, dir);
  elements[ENEMY_BULLET].frame++;
  if (elements[ENEMY_BULLET].frame < SHOT_STEPS_PER_TILE) return;
  elements[ENEMY_BULLET].frame = 0;

  if (resolveShotOnTile(elements[ENEMY_BULLET].x, elements[ENEMY_BULLET].y, dir, false))
    deactivateEnemyShot();
}

#endif
