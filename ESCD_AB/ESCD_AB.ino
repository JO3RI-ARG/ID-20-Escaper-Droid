/*
  Escaper Droid
  Arduboy version 0.8.1
  
  STARTED by TEAM a.r.g.
  2016 - JO3RI - STG

  CONTINUED by
  2026 - JO3RI - Onebit

  Game License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 20

#include "globals.h"
#include "menu.h"
#include "game.h"
#include "room.h"
#include "elements.h"
#include "inputs.h"
#include "player.h"
#include "song.h"


typedef void (*FunctionPointer) ();
const FunctionPointer PROGMEM  mainGameLoop[] =
{
  stateMenuMain,
  stateMenuConf,
  stateMenuSdfx,
  stateMenuInfo,
  stateMenuPlay,
  stateMenuIntro,
  stateGamePlaying,
  stateGameNextRoom,
  stateGameNextLevel,
  stateGamePause,
  stateGameOver,
  stateGameTransporting,
  stateGameFinished,
};


void setup()
{
  arduboy.boot();
  arduboy.audio.begin();
  arduboy.setFrameRate(60);
  //Serial.begin(9600);
  ATM.play(introMusic);
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
};

