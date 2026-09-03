#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "font.h"

unsigned char charBox[120];
unsigned char textBox[31];

PROGMEM const char testMessage[] = // each message max 30 characters including spaces
//"MAXIMUM 30 CHARS IS TILL HERE.\0"
//"//////////////////////////////\0"
  "HI MATT ARE YOU IMPRESSED ?   \0"
  "HELLO ARDUBOY\0"
  "HAVE A NICE DAY\0"
  "GAME OVER\0";

void fillTextBox(byte indexMessag)
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
        Serial.print(charBox[i+w],BIN);
        Serial.print(",");
      }
      i = i + 3;
      //charBox[i] = pgm_read_byte(&messages[copyPlace]);
      copyPlace++;
      spacing++;
    }
  }
}

void loadMessage(uint8_t indexMessage) {
  memset(textBox, 0, sizeof(textBox));

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
    // while we put the character in ou textBox, we make sure it corresponds to our font
    textBox[i] = characterOfMessage-FONT_OFFSET;     // karakter
    if (textBox[i] == 240)textBox[i]=255;
    i++;
    //textBox[i++] = 0;      // separator 0
  }
  for (byte i = 0; i< sizeof(textBox); i++)
  {
  Serial.print(textBox[i],DEC);
  Serial.print(",");
  }
  Serial.println();
}

#endif