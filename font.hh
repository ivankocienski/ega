
#pragma once

namespace platform {

class Screen;

class Font {
public:
  
  typedef unsigned char char_row_t;

private:

  char_row_t * m_bit_field;

  
public:
  Font();
  ~Font();

  bool load( int, int, char_row_t* );

  void putchar( int, int, int, int );
  void puts( Screen*, int, int, int, int, char* );
};

};

