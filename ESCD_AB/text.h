#ifndef TEXT_H
#define TEXT_H

#include "font.h"

// TEXT CHARACTERISTICS FOR THE TICKER
// NOT ALL COMBINATIONS ARE POSSIBLE.    
// HERE ARE THE DEFINES YOU CAN PICK FROM
#define TEXT_NOT_SHOWN             0
#define TEXT_STAND_STILL           1       
#define TEXT_SCROLL_LEFT           3       
#define TEXT_SCROLL_RIGHT          5       
#define TEXT_SCROLL_UP             9       
#define TEXT_SCROLL_DOWN           17      
#define TEXT_BLINK                 32      
#define TEXT_BLINK_SCROLL_LEFT     35      
#define TEXT_BLINK_SCROLL_RIGHT    37      
#define TEXT_BLINK_SCROLL_UP       40      
#define TEXT_BLINK_SCROLL_DOWN     48                                            

//         byte setTicker = 0b00000000;
//                            ||||||||       
//                            ||||||||   
//                            ||||||||
//                            |||||||└->0 SHOW TEXT    (0 = false / 1 = true)
//                            ||||||└-->1 SCROLL LEFT  (0 = false / 1 = true)
//                            |||||└--->2 SCROLL RIGHT (0 = false / 1 = true)
//                            ||||└---->3 SCROLL UP    (0 = false / 1 = true)
//                            |||└----->4 SCROLL DOWN  (0 = false / 1 = true)
//                            ||└------>5 BLINK        (0 = false / 1 = true)
//                            |└------->6 NOT USED     (0 = false / 1 = true)
//                            └-------->7 NOT USED     (0 = false / 1 = true)

unsigned char charBox[120]; 
//unsigned char textBox[30];

PROGMEM const char textMessage[] = // each message max 30 characters including spaces
//"MAXIMUM 30 CHARS IS TILL HERE.\0"
//"//////////////////////////////\0"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234\0"
  "HELLO ARDUBOY\0"
  "HAVE A NICE DAY\0"
  "GAME OVER\0";

void loadAndFillMessage(uint8_t indexMessage)
{
  memset(charBox, 0, sizeof(charBox));

  // Find the start of the requested message
  const char* msg = textMessage;
  while (indexMessage > 0)
  {
    while (pgm_read_byte(msg) != 0) msg++;
    msg++;               // skip the '\0'
    indexMessage--;
  }

  // Directly convert characters → font data into charBox
  uint8_t pos = 0;       // position in charBox

  while (pos + 2 < sizeof(charBox))
  {
    char c = pgm_read_byte(msg++);
    if (c == 0) break;   // end of string

    uint8_t idx = c - FONT_OFFSET;
    if (idx == 240) idx = 255;   // treat as blank (already zeroed)

    if (idx != 255)
    {
      uint16_t fontOffset = (uint16_t)idx * 3;
      charBox[pos    ] = pgm_read_byte(&font[fontOffset    ]);
      charBox[pos + 1] = pgm_read_byte(&font[fontOffset + 1]);
      charBox[pos + 2] = pgm_read_byte(&font[fontOffset + 2]);
    }

    pos += 4;            // advance by one glyph (4 bytes) adding a spacing between characters
  }
}



#endif