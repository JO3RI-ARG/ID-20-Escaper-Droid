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
//"---------------|--------------\Ø"
//"//////////////////////////////\0"
  "      CONF SDFX INFO PLAY\0"           //00
  " BUTTON SCHEME    N<>S  E<>W\0"        //01
  " MUSIC SOUND       ON   OFF\0"         //02
  " CREATED BY STG ONEBIT JO3RI\0"        //03
  "LEVEL    SCORE:\0"                     //04
  "GAME OVER  :<  SCORE:\0"               //05
  "DROID ESCAPED :>  SCORE:\0"            //06
  "ALERT ALERT DROID ESCAPING\0";         //07

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

void addNumber(unsigned long number, byte charIndex, byte amountLeadingZeros)
{
  if (amountLeadingZeros > 6) amountLeadingZeros = 6;

  // Count how many digits the number really has
  unsigned long temp = number;
  byte actualDigits = (number == 0) ? 1 : 0;
  while (temp)
  {
    actualDigits++;
    temp /= 10;
  }

  // Total digits to write = max(actualDigits, amountLeadingZeros)
  byte totalDigits = (actualDigits > amountLeadingZeros) ? actualDigits : amountLeadingZeros;

  // Start writing from the rightmost character
  int writePos = (charIndex + totalDigits - 1) * 4;

  for (byte i = 0; i < totalDigits; i++)
  {
    uint8_t digit = number % 10;
    number /= 10;

    uint8_t idx = (digit + '0') - FONT_OFFSET;
    uint16_t fontOffset = (uint16_t)idx * 3;

    if (writePos + 2 < sizeof(charBox))
    {
      charBox[writePos    ] = pgm_read_byte(&font[fontOffset    ]);
      charBox[writePos + 1] = pgm_read_byte(&font[fontOffset + 1]);
      charBox[writePos + 2] = pgm_read_byte(&font[fontOffset + 2]);
    }

    writePos -= 4;   // previous character
  }
}



#endif