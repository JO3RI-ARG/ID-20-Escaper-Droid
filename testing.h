void prepareTextBox(byte indexMessage)
{
  byte spacing = 0;
  byte copyPlace = 0;
  memset(charBox, 0, sizeof(charBox));
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
  byte k = 0;
  byte textBoxChar = 0;
  char characterOfMessage;
  while ((characterOfMessage = pgm_read_byte(startMessage++)) != 0 && k < 30)
  {
    // while we put the character in our textBox, we make sure it corresponds to our font
    textBoxChar = characterOfMessage-FONT_OFFSET;     // karakter
    if (textBoxChar == 240)textBoxChar=255;
    for (byte w = 0; w<3;w++)
      {
        char test = textBoxChar*3;
        charBox[i+w] = pgm_read_byte(&font[test+w]);
      }
    k++;
  }

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

  ///////////////////////////////////////



unsigned char charBox[120]; 
unsigned char textBox[30];

PROGMEM const char textMessage[] = // each message max 30 characters including spaces
//"MAXIMUM 30 CHARS IS TILL HERE.\0"
//"//////////////////////////////\0"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234\0"
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

  // Find the start of the message
  byte startMessage = 0;
  // So we keep looking as long as indexMessage > 0
  while (indexMessage > 0) {
    // skip current strings (until and including \0)
    while (pgm_read_byte(&textMessage[startMessage]) != 0) startMessage++; //pgm_read_byte(&font[test+w]) (pgm_read_byte(startMessage)
    startMessage++;          // spring over de \0 heen
    indexMessage--;
  }
}

/////////////merged version//////////////

unsigned char charBox[120];

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

    pos += 3;            // advance by one glyph (3 bytes)
  }
}