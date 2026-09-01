#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Arduboy2.h>
#include <Sprites.h>
#include <ATMlib.h>
#include "bitmaps.h"
#include "sfx.h"
#include "song.h"

// constants /////////////////////////////////////////////////////////////////

//define menu states (on main menu)
#define STATE_MENU_INTRO             5
#define STATE_MENU_MAIN              0
#define STATE_MENU_PLAY              4
#define STATE_MENU_INFO              3
#define STATE_MENU_CONF              1
#define STATE_MENU_SDFX              2

//define menuSelection states
#define STATE_MENU_SELECT_CONF       0
#define STATE_MENU_SELECT_SDFX       1
#define STATE_MENU_SELECT_INFO       2
#define STATE_MENU_SELECT_PLAY       3

//define game states (on main menu)
#define STATE_GAME_PLAYING           6
#define STATE_GAME_NEXT_ROOM         7
#define STATE_GAME_NEXT_LEVEL        8
#define STATE_GAME_PAUSE             9
#define STATE_GAME_OVER              10
#define STATE_GAME_TRANSPORTING      11
#define STATE_GAME_FINISHED          12

//define facing directions
#define NORTH                        0
#define EAST                         1
#define SOUTH                        2
#define WEST                         3

#define NORTH_SOUTH                  0
#define EAST_WEST                    1

#define TURN_LEFT                    0
#define TURN_RIGHT                   1

#define BIG_FONT                     0
#define THIN_FONT                    1
#define SMALL_FONT                   2

#define SIZE_OF_ITEMSORDER           46
#define ITEMS_ORDER_TILES_START      10

#define LEVEL_TO_START_WITH          1
#define TILE_GAME_STARTS_ON          12

#define FALSE                        0
#define TRUE                         1

// globals ///////////////////////////////////////////////////////////////////

Arduboy2Base arduboy;
Sprites sprites;
ATMsynth ATM;

byte gameState = STATE_MENU_INTRO;   // start the game with the intro logo
byte menuSelection = STATE_MENU_SELECT_PLAY; // PLAY menu item is pre-selected
byte level;
unsigned long scorePlayer;

byte itemsOrder[SIZE_OF_ITEMSORDER];
byte currentRoom;
byte currentlyOnTestingTile;
byte testingTile;
int currentRoomY;
byte selectorX = 0;
byte selectorX2 = 0;
byte amountOfTransporters;
byte buttonSchemeOffset;

#endif
