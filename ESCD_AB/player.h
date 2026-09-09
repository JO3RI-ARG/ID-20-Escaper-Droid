#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

#define PLAYER_DROID                      9
#define PLAYER_BULLET                     10
#define PLAYER_IMMUNE_TIME                240
#define PLAYER_TRANSPORTING_TIME          90

#define PLAYER                            1

#define DROID_FACING_NORTH                NORTH
#define DROID_FACING_EAST                 EAST
#define DROID_FACING_SOUTH                SOUTH
#define DROID_FACING_WEST                 WEST

#define DROID_VISIBLE_AT_BIT_2            2
#define DROID_IMMUNE_AT_BIT_3             3
#define DROID_DYING_AT_BIT_4              4
#define DROID_GOES_THROUGH_DOOR_AT_BIT_5  5
#define DROID_COMES_OUT_DOOR_AT_BIT_6     6
#define DROID_TRANSPORTING_AT_BIT_7       7

#define DROID_HAS_BLACK_CARD_AT_BIT_5     5
#define DROID_BATTERY_VISIBLE_AT_BIT_6    6
#define MAX_ROOM_Y_SCROLLING            -28

#define SHOT_STEPS_PER_TILE  6
#define SCORE_ENEMY_HIT      75

struct EscaperDroid
{
  public:
    int x, y;
    byte life;
    byte characteristics;
    byte assets;
    byte isOnTile;
    byte steps;
    byte immuneTimer;
    byte transportTimer;

    void set()
    {
      characteristics = 0b00000110;
      assets = 0b01000000;
      isOnTile = TILE_GAME_STARTS_ON;
      steps = 0;
      life = 3;
      immuneTimer = 0;
      transportTimer = 0;
    }
};

EscaperDroid player;

struct Shot
{
  int x, y;
  byte dir;
  byte steps;
  bool active;
};

Shot playerShot;

void deactivatePlayerShot()
{
  playerShot.active = false;
}

void spawnPlayerShot()
{
  if (playerShot.active) return;
  if ((player.assets & 0b00000111) == 0) return;
  player.assets--;
  playerShot.active = true;
  playerShot.steps = 0;
  playerShot.x = player.x;
  playerShot.y = player.y - currentRoomY;
  playerShot.dir = player.characteristics & 0b00000011;
}

void walkThroughDoor()
{
  if (bitRead(player.characteristics, DROID_GOES_THROUGH_DOOR_AT_BIT_5) || bitRead(player.characteristics, DROID_COMES_OUT_DOOR_AT_BIT_6)) player.steps++;
  switch (player.characteristics & 0b00000011)
  {
    case NORTH:
      if (currentRoomY < 0) currentRoomY++;
      else player.y -= 1;
      player.x -= 2;
      break;
    case EAST:
      if (currentRoomY < 0)currentRoomY++;
      else player.y -= 1;
      player.x += 2;
      break;
    case SOUTH:
      if (currentRoomY > MAX_ROOM_Y_SCROLLING)currentRoomY--;
      else player.y += 1;
      player.x += 2;
      break;
    case WEST:
      if (currentRoomY > MAX_ROOM_Y_SCROLLING)currentRoomY--;
      else player.y += 1;
      player.x -= 2;
      break;
  }
}

void playerLosesLife()
{
  if (!bitRead(player.characteristics, DROID_IMMUNE_AT_BIT_3))
  {
    player.life--;
    bitSet(player.characteristics, DROID_IMMUNE_AT_BIT_3);
    if (player.life < 1)
    {
      bitSet(player.characteristics, DROID_DYING_AT_BIT_4);
      bitClear(player.characteristics, DROID_IMMUNE_AT_BIT_3);
    }
  }
}

void updatePlayer()
{
  if (bitRead(player.characteristics, DROID_IMMUNE_AT_BIT_3))
  {
    player.immuneTimer++;
    if (arduboy.everyXFrames(4)) bitToggle(player.characteristics,DROID_VISIBLE_AT_BIT_2);
    if (player.immuneTimer > PLAYER_IMMUNE_TIME)
    {
      bitClear(player.characteristics, DROID_IMMUNE_AT_BIT_3);
      bitSet(player.characteristics, DROID_VISIBLE_AT_BIT_2);
      player.immuneTimer = 0;
    }
  }
  else if (bitRead(player.characteristics, DROID_DYING_AT_BIT_4))
  {
    currentRoomY = ROOM_DRAWING_OFFSET;
    loadAndFillMessage(5);
    addNumber(scorePlayer,22,6);
    tickerScroll = 0;
    setTicker = TEXT_SCROLL_LEFT;
    showTicker = TRUE;
    player.x = 51;
    player.y = 21;
    gameState = STATE_GAME_OVER;
  }
}

void playerTransporting()
{
  player.transportTimer++;
  if (arduboy.everyXFrames(4))
  {
    if ((player.characteristics & 0b00000011) > 2) player.characteristics = player.characteristics & 0b11111100;
    else if ((player.characteristics & 0b00000011) < 3) player.characteristics++;
  }
  if (player.transportTimer > PLAYER_TRANSPORTING_TIME)
  {
    bitSet(player.characteristics,DROID_VISIBLE_AT_BIT_2);
    bitClear(player.characteristics,DROID_TRANSPORTING_AT_BIT_7);
    player.transportTimer = 0;
    gameState = STATE_GAME_PLAYING;
  }
}

void drawPlayer()
{
  byte counter = player.characteristics & 0b00000011;
  byte counterTwice = 2*counter;
  if (bitRead(player.characteristics, DROID_TRANSPORTING_AT_BIT_7))
  {
    for (byte i = 0; i<17;i=i+8) sprites.drawPlusMask(player.x-4, player.y-6 + i - counterTwice, transportBeams_plus_mask,0);
  }
  if (bitRead(player.characteristics, DROID_DYING_AT_BIT_4)) sprites.drawPlusMask(player.x, player.y, droid_plus_mask, 4);
  else if (bitRead(player.characteristics, DROID_VISIBLE_AT_BIT_2)) sprites.drawPlusMask(player.x, player.y, droid_plus_mask, counter);
  if (bitRead(player.characteristics, DROID_TRANSPORTING_AT_BIT_7))
  {
    for (byte i = 0; i<17;i=i+8)sprites.drawPlusMask(player.x-4, player.y+2 + i - counterTwice, transportBeams_plus_mask,1);
  }
}

void drawBulletPlayer()
{
  if (!playerShot.active) return;
  sprites.drawPlusMask(playerShot.x + 4, playerShot.y + currentRoomY + 6, elements_plus_mask, 18);
}

#endif
