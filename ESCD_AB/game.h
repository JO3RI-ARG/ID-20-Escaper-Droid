#ifndef GAME_H
#define GAME_H

#include "globals.h"
#include "inputs.h"
#include "text.h"

void stateMenuPlay()
{
  ATM.stop();
  scorePlayer = 0;
  player.set();
  level = LEVEL_TO_START_WITH - 1;
  gameState = STATE_GAME_NEXT_LEVEL;
}

void stateGamePlaying()
{
  checkOrderOfObjects(currentRoom, level);
  drawRoom();
  if (!bitRead(player.characteristics, 5))
  {
    if (!bitRead(player.characteristics, 4)) checkInputs();
  }
  else
  {
    if (player.steps < 5)
    {
      if (arduboy.everyXFrames(2)) walkThroughDoor();
    }
    else 
    {
      player.isOnTile = goToTile(currentRoom, level);
      currentRoomY = setCurrentRoomY(player.isOnTile);
      currentRoom = goToRoom(currentRoom, level);
      player.x = translateTileToX (player.isOnTile) + offsetXAfterDoor(player.isOnTile);
      player.y = translateTileToY (player.isOnTile) + offsetYAfterDoor(player.isOnTile) + currentRoomY ;
      player.steps = 0;
      enterRoom(currentRoom, level);
      bitClear (player.characteristics, 5);
      bitSet (player.characteristics, 6);
      gameState = STATE_GAME_NEXT_ROOM;
      return; //don't update the enemies yet, first go through the door
    }
  }
  updatePlayer();
  if (!bitRead(player.characteristics, 4))
  {
    updateEnemies();
    updatePlayerShot();
    updateEnemyShot();
  }
  drawHUD();
}


void stateGameNextRoom()
{
  checkOrderOfObjects(currentRoom, level);
  drawRoom();

  {
    if (player.steps < 5)
    {
      if (arduboy.everyXFrames(2)) walkThroughDoor();
    }
    else
    {
      player.steps = 0;
      bitClear (player.characteristics, 6);
      statePrepForRoom();
    }
  }
  drawHUD();
}

void stateGameNextLevel()
{
  if (level > (sizeof (levels)/2))
  {
    loadAndFillMessage(3);
    addNumber(scorePlayer,24,6);
    gameState = STATE_GAME_FINISHED;
  }
  else
  {
    currentRoom = 0;
    player.isOnTile = TILE_GAME_STARTS_ON;
    currentRoomY = ROOM_DRAWING_OFFSET;
    player.x = translateTileToX (player.isOnTile);
    player.y = translateTileToY (player.isOnTile) + currentRoomY ;
    buildRooms(level);
    enterRoom(currentRoom, level);
    statePrepForPause();
    gameState = STATE_GAME_PAUSE;
  }
}


void stateGamePause()
{
  drawWalls();
  drawFloor();
  drawHUD();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    statePrepForRoom();
  }
}


void stateGameOver()
{
  drawWalls();
  drawFloor();
  drawPlayer();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    ATM.play(menuSong);
    statePrepForMainMenu();
  }
}

void stateGameFinished()
{
  drawWalls();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) 
  {
    ATM.play(menuSong);
    statePrepForMainMenu();
  }
}

void stateGameTransporting()
{
  playerTransporting();
  drawWalls();
  drawFloor();
  drawHUD();
  drawPlayer();
  if (arduboy.everyXFrames(90))
  {
    currentRoom = transportToRoom(currentRoom);
    player.x = translateTileToX (player.isOnTile) ;
    player.y = translateTileToY (player.isOnTile) + currentRoomY ;
    player.steps = 0;
    enterRoom(currentRoom, level);
  }
}



#endif
