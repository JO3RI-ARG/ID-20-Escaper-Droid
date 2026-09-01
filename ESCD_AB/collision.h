#ifndef COLLISION_H
#define COLLISION_H

#include "globals.h"

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
      //Serial.print(tileTesting);
      //Serial.print(" : ");
      //Serial.println(currentlyOnTestingTile);
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
  //Serial.println(testingTile);
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


void decideOnCollision()
{
  switch (currentlyOnTestingTile)
  {
    case ENEMY_ONE:
      break;
    case ENEMY_TWO:
      break;
    case OBJECT:
      checkObjectTypeAndAct();
      break;
    case FLOOR_ONE:
      break;
    case FLOOR_TWO:
      break;
    case FLOOR_THREE:
      break;
    case FLOOR_FOUR:
      break;
    case FLOOR_FIVE:
      break;

  }
}



#endif
