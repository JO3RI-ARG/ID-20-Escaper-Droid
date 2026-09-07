#ifndef COLLISION_H
#define COLLISION_H

#include "globals.h"
#include "elements.h"
#include "player.h"
#include "room.h"

boolean hitBorders(int objectX, int objectY, int directionFacing, bool playerOrEnemy)
{
  // check the borders of the room
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
  if ((player.assets & 0B00100000) && (checkIfLevelDoor() == direction)) // also still need to check if it is a level door
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
      if ((player.isOnTile == TILE_INFRONT_DOOR_NORTH) &&                                   // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_EXISTS)) &&         // the door exists in this room         
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, NORTH_DOOR_IS_CLOSSED))       // the door is not closed
         )
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);                 // if all above, set the droid is walking through the door
        player.x = translateTileToX(2);
        player.y = translateTileToY(2) + currentRoomY;
      }
      break;
    case EAST:
      if ((player.isOnTile == TILE_INFRONT_DOOR_EAST) &&                                    // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_EXISTS)) &&          // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, EAST_DOOR_IS_CLOSSED))        // the door is not closed
         )
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);                  // if all above, set the droid is walking through the door
        player.x = translateTileToX(10);
        player.y = translateTileToY(10) + currentRoomY;
      }
      break;
    case SOUTH:
      if ((player.isOnTile == TILE_INFRONT_DOOR_SOUTH) &&                                   // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_EXISTS)) &&         // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, SOUTH_DOOR_IS_CLOSSED))       // the door is not closed
         )
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);                  // if all above, set the droid is walking through the door
        player.x = translateTileToX(22);
        player.y = translateTileToY(22) + currentRoomY;
      }
      break;
    case WEST:
      if ((player.isOnTile == TILE_INFRONT_DOOR_WEST) &&                                                        // droid is on the tile for a door
          (bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_EXISTS)) &&          // the door exists in this room
          (!bitRead(stageRoom[currentRoom].doorsClosedActive, WEST_DOOR_IS_CLOSSED))        // the door is not closed
         )
      {
        bitSet (player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5);                                                  // if all above, set the droid is walking through the door
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
    case PICKUP_BLACK_CARD: // opens level door
      if (bitRead(player.assets,5) == 0) //check if droid has a black card
      {
        bitSet(player.assets,5);         //add the card to the droid
        clearElement();                  //remove the black card from the room
        scorePlayer += SCORE_BLACK_CARD;
      }
      break;
    case PICKUP_WHITE_CARD: // opens normal door
      if ((player.assets & 0b00011000) < 0b00011000) //check if droid has not more than 3 white cards
      {
        player.assets += 0b00001000;                 //add 1 white card to the droid
        clearElement();                              //remove the white card from the room 
        scorePlayer += SCORE_WHITE_CARD;
      }
      break;
    case PICKUP_BATTERY:    // 1 extra life
      if (player.life < 3)                            //check if the player has less than 3 lifes
      {
        player.life++;                                //add 1 life
        clearElement();                               //remove the life from the room
        scorePlayer += SCORE_LIFE;
      }
      else 
      {
        clearElement();                               //remove the life from the room
        scorePlayer += SCORE_TO_MUCH_LIFE;
      }
      break;
    case PICKUP_BULLET:     // 1 shot
      if ((player.assets & 0b00000111) < 0b00000111) //check if the player has less than 7 bullets
      {
        player.assets++;                             //add 1 bullet
        clearElement();                              //remove 1 bullet from the room
        scorePlayer += SCORE_BULLET;
      }
      break;
    case PICKUP_CHIP:       // extra points
      clearElement();                                //remove the chip from the room
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
        if (kind == FLOOR_SPIKE || kind == FLOOR_PIT)
          playerTouchesHazard();
      }
      break;
  }
}

void stepShot(int &sx, int &sy, byte dir)
{
  switch (dir & 0b00000011)
  {
    case NORTH: sy -= 1; sx -= 2; break;
    case EAST:  sy -= 1; sx += 2; break;
    case SOUTH: sy += 1; sx += 2; break;
    case WEST:  sy += 1; sx -= 2; break;
  }
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
