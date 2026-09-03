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
unsigned char textBox[30];

PROGMEM const char testMessage[] = // each message max 30 characters including spaces
//"MAXIMUM 30 CHARS IS TILL HERE.\0"
//"//////////////////////////////\0"
  "HI MATT ARE YOU IMPRESSED ?\0"
  "HELLO ARDUBOY\0"
  "HAVE A NICE DAY\0"
  "GAME OVER\0";

void fillTextBox()
{
  byte spacing = 0;
  byte copyPlace = 0;
  memset(charBox, 0, sizeof(charBox));
  for (byte i=0; i< sizeof(charBox);i++)
  { 
    {
      for (byte w = 0; w<3;w++)
      {
        char test = textBox[copyPlace]*3;
        charBox[i+w] = pgm_read_byte(&font[test+w]);
      }
      i = i + 3;
      copyPlace++;
      spacing++;
    }
  }
}

void loadMessage(uint8_t indexMessage) {
  memset(textBox, 255, sizeof(textBox));

  // Find the start of the message
  const char* startMessage = testMessage;
  // So we keep looking as long as indexMessage > 0
  while (indexMessage > 0) {
    // skip current strings (until and including \0)
    while (pgm_read_byte(startMessage) != 0) startMessage++;
    startMessage++;          // spring over de \0 heen
    indexMessage--;
  }

  // Now startMessage points to the start of the message we want with indexMessage
  uint8_t i = 0;
  char characterOfMessage;
  while ((characterOfMessage = pgm_read_byte(startMessage++)) != 0 && i < 36)
  {
    // while we put the character in our textBox, we make sure it corresponds to our font
    textBox[i] = characterOfMessage-FONT_OFFSET;     // karakter
    if (textBox[i] == 240)textBox[i]=255;
    i++;
  }
}



#endif