/* ----------------------------------------------------------------------------
 *  A better absolute!
 */
template<typename T> T absT(const T & v) {
  
  return (v < 0) ? -v : v;

}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 *  
 *  So much nicer than a solid line!
 */
void drawHorizontalDottedLine(int x1, int x2, int y) {
    
  for (int z = x1; z <= x2; z+=2) {
    arduboy.drawPixel(z, y, WHITE);
  }
  
}


/* ----------------------------------------------------------------------------
 *  Slow the helicopter down or speed it up expotentially ..
 */
int8_t calcSpeed(int8_t initValue, bool increase) {
      
  switch (initValue) {

    case 8:
      if (!increase) return 4;
    
    case 4: 
      if (increase)  return 4;
      if (!increase) return 2;
    
    case 2:
      if (increase)  return 4;
      if (!increase) return 1;

    case 1:
      if (increase)  return 2;
      if (!increase) return 0;

    case 0:
      if (increase)  return 1;
      if (!increase) return -1;

    case -1:
      if (increase)  return 0;
      if (!increase) return -2;

    case -2:
      if (increase)  return -1;
      if (!increase) return -4;

    case -4:
      if (increase)  return -2;
      if (!increase) return -4;

    case -8:
      if (increase)  return -4;

    default: break;

  }

}


/* ----------------------------------------------------------------------------
 *  Draw the helicopter ..
 */
void drawHelicopter(uint8_t x, int8_t y, int8_t image) {

  uint8_t index = absT(image);
  bool mirror = (image < 0);

  arduboy.drawCompressedMirror(x, y, helicopter[((index - 1) * 6) + 1], BLACK, mirror);
  arduboy.drawCompressedMirror(x, y, helicopter[(index - 1) * 6], WHITE, mirror);
  arduboy.drawCompressedMirror(x, y, helicopter[((index - 1) * 6) + (frame) + 2], WHITE, mirror);

}