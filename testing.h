void prepareTextBox(byte indexMessage)
{
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