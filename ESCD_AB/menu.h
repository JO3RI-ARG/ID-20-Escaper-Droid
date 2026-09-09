#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "room.h"

// main ticker: "      CONF SDFX INFO PLAY"
// each glyph is 4 columns; words start at charBox x = 24, 44, 64, 84
PROGMEM const byte menuWordX[] = { 16, 36, 64, 84 };

void drawTitleScreen()
{
  if (currentRoomY == 0) currentRoomY = ROOM_DRAWING_OFFSET;
  drawWalls();
}

byte tickerYAt(byte x)
{
  return (x < 58) ? (x >> 1) : (58 - (x >> 1));
}

void drawSelectedWordMask(byte x0)
{
  if (arduboy.everyXFrames(16)) bitToggle(showMask, 0);
  if (!showMask) return;
  for (byte i = 0; i < 8; i++)
  {
    byte x = x0 + (i << 1);
    sprites.drawPlusMask(x, currentRoomY + 38 - tickerYAt(x) + tickerScroll, selector_plus_mask, 0);
  }
}

void statePrepForMainMenu()
{
  currentRoomY = ROOM_DRAWING_OFFSET;
  tickerScroll = 0;
  loadAndFillMessage(0);
  setTicker = TEXT_STAND_STILL;
  showTicker = TRUE;
  player.set();
  player.x = 51;
  player.y = 11;
  gameState = STATE_MENU_MAIN;
}

void stateMenuIntro()
{
  if (arduboy.everyXFrames(120))
  {
    ATM.play(menuSong);
    statePrepForMainMenu();
  }
  sprites.drawSelfMasked(49, 20, T_arg, 0);
}

void stateMenuMain()
{
  drawTitleScreen();
  drawSelectedWordMask(pgm_read_byte(&menuWordX[menuSelection]));
  //drawFloor();
  if (arduboy.everyXFrames(26)) bitToggle(player.characteristics,0);
  drawPlayer();

  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 3)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 0)) menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = menuSelection + 1;
    loadAndFillMessage(menuSelection + 1);
    tickerScroll = 0;
  }
}

void stateMenuConf()
{
  drawTitleScreen();
  // " BUTTON SCHEME    N<>S  E<>W"  N<>S @ char 18 → x72, E<>W @ char 24 → x96
  drawSelectedWordMask(buttonSchemeOffset ? 96 : 72);
  if (arduboy.justPressed(RIGHT_BUTTON)) buttonSchemeOffset = 4;
  if (arduboy.justPressed(LEFT_BUTTON)) buttonSchemeOffset = 0;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    statePrepForMainMenu();
  }
}

void stateMenuInfo()
{
  drawTitleScreen();
  setTicker = TEXT_SCROLL_LEFT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    statePrepForMainMenu();
  }
}

void stateMenuSdfx()
{
  drawTitleScreen();
  // " MUSIC SOUND       ON   OFF"  ON @ char 19 → x76, OFF @ char 24 → x96
  drawSelectedWordMask(arduboy.audio.enabled() ? 76 : 96);
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    statePrepForMainMenu();
  }
}

#endif
