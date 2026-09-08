#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "room.h"

void drawTitleScreen()
{
  /*
  sprites.drawSelfMasked(0, 0, titleScreen, 0);
  sprites.drawSelfMasked(17, 56, mainMenus, gameState);
  */
  drawWalls();
}

void drawMask()
{
  if (arduboy.everyXFrames(4))
  {
    if (showMask==0)showMask=1;
    else showMask=0;
  }
  //if (showMask) 
  sprites.drawPlusMask(18+(menuSelection * 24), 30, selector_plus_mask, 0);
  sprites.drawPlusMask(20+(menuSelection * 24), 30, selector_plus_mask, 0);
  sprites.drawPlusMask(22+(menuSelection * 24), 30, selector_plus_mask, 0);
}

void moveSelectors()
{
  /*
if (arduboy.everyXFrames(2))
  {
    selectorX++;
    selectorX2--;
  }
  if (selectorX > 31)selectorX = 20;
  if (selectorX2 < 21)selectorX2 = 32;
  */
}

void stateMenuIntro()
{
  if (arduboy.everyXFrames(120))
  { 
    ATM.stop();
    ATM.play(menuSong);
    buttonSchemeOffset = FALSE;
    loadAndFillMessage(0);
    setTicker = TEXT_STAND_STILL;
    gameState = STATE_MENU_MAIN;
  }
  sprites.drawSelfMasked(49, 20, T_arg, 0);

}

void stateMenuMain()
{
  // show the titleScreen art
  drawTitleScreen();
  //moveSelectors();
  if (arduboy.everyXFrames(16))
  {
    bitToggle(showMask,0);
  }
  if (showMask){
  sprites.drawPlusMask(24+(menuSelection * 24), 25, selector_plus_mask, 0);
  sprites.drawPlusMask(26+(menuSelection * 24), 23, selector_plus_mask, 0);
  sprites.drawPlusMask(28+(menuSelection * 24), 21, selector_plus_mask, 0);
  }

  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < 3)) menuSelection++;
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 0)) menuSelection--;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) 
  {
    gameState = menuSelection+1;
    loadAndFillMessage(menuSelection+1);
  }
}

void stateMenuConf()
{
  byte offSet = 65 + (12 * buttonSchemeOffset);
  drawTitleScreen();
  //moveSelectors();
  //sprites.drawPlusMask(selectorX + offSet, 56, selector_plus_mask, 0);
  //sprites.drawPlusMask(selectorX2 + offSet, 56, selector_plus_mask, 0);
  if (arduboy.justPressed(RIGHT_BUTTON)) buttonSchemeOffset = 4;
  if (arduboy.justPressed(LEFT_BUTTON)) buttonSchemeOffset = 0;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    loadAndFillMessage(0);
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuInfo()
{
  drawTitleScreen();
  setTicker=TEXT_SCROLL_LEFT;
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    loadAndFillMessage(0);
    setTicker=TEXT_STAND_STILL;
    gameState = STATE_MENU_MAIN;
  }
}

void stateMenuSdfx()
{
  byte offSet = 44 + (arduboy.audio.enabled() * 18);
  drawTitleScreen();
  //moveSelectors();
  //sprites.drawPlusMask(selectorX + offSet, 56, selector_plus_mask, 0);
  //sprites.drawPlusMask(selectorX2 + offSet, 56, selector_plus_mask, 0);
  if (arduboy.justPressed(RIGHT_BUTTON)) arduboy.audio.on();
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    loadAndFillMessage(0);
    gameState = STATE_MENU_MAIN;
  }
}

#endif
