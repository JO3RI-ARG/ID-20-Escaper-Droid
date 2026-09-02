#ifndef DICTIONARY_H
#define DICTIONARY_H

unsigned char textBox[120];

PROGMEM const char testMessage[] = "HELLO WORLD";


PROGMEM const char messages[] = {
0B00011111,
0B00000100,
0B00011111,

0B00011111,
0B00010101,
0B00010001,

0B00011111,
0B00010000,
0B00010000,

0B00011111,
0B00010000,
0B00010000,

0B00011111,
0B00010001,
0B00011111,

0B00000000,
0B00000000,
0B00000000,

0B00000000,
0B00000000,
0B00000000,

0B00011111,
0B00011100,
0B00011111,

0B00011111,
0B00010001,
0B00011111,

0B00011111,
0B00000101,
0B00011010,

0B00011111,
0B00010000,
0B00010000,

0B00011111,
0B00010001,
0B00001110,
  /*
  0,0,0, //
  1,2,3, //D
  1,4,1, //A
  1,5,1, //N
  1,6,7, //G
  1,8,8, //E
  1,9,10,//R
  0,0,0, //
  1,2,3, //D
  1,4,1, //A
  1,5,1, //N
  1,6,7, //G
  1,8,8, //E
  1,9,10,//R
  0,0,0, //
  1,2,3, //D
  1,4,1, //A
  1,5,1, //N
  1,6,7, //G
  1,8,8, //E
  1,9,10,//R
  0,0,0, //
  1,2,3, //D
  1,4,1, //A
  1,5,1, //N
  1,6,7, //G
  1,8,8, //E
  1,9,10,//R
  0,0,0, //
  */
};



void clearTextBox()
{
  memset(textBox, 0, sizeof(textBox));
}

void fillTextBox(byte index)
{
  byte spacing = 0;
  byte copyPlace = 0;
  memset(textBox, 0, sizeof(textBox));
  for (byte i=0; i< sizeof(messages)/3*4;i++)
  { 
    if (spacing > 2)
    {
      spacing = 0;
      textBox[i] = 0;
    }
    else
    {
      textBox[i] = pgm_read_byte(&messages[copyPlace]);
      copyPlace++;
      spacing++;
    }
  }
}

PROGMEM const unsigned char library[] =
{
  1,2,1,2,
};

#endif