
#include <stdlib.h>

#include "font.hh"
#include "screen.hh"

namespace EGA {

  /* this is a bit memory inefficient */

Font::Font() {
  m_bit_field = NULL;
}

Font::~Font() {
  if(m_bit_field) free(m_bit_field);
}

bool Font::load( int start, int count, char_row_t *bytes ) {


  m_bit_field = (char_row_t *)calloc(16384, sizeof(char_row_t));
  if(!m_bit_field) return false;

  memcpy( m_bit_field + start * 64, bytes, count * 64 );

  return true; 
}

void Font::puts( Screen *screen, int x, int y, int c, int l, char *t ) {

  int dw; // draw width
  int dh; // draw height
  int xo; // x offset of char
  int yo; // y offset of char
  int w;  // width counter
  int h;  // height counter

  char_row_t      *f;
  Screen::pixel_t *p;

  if( x > 319 ) return;
  if( y > 199 ) return;

  if( y < 0 ) { 
    yo = -y;
    dh = 8 + y; 
    y  = 0;

  } else {
    yo = 0;
    dh = 199 - y; 
    if(dh > 8) dh = 8;
  }

  if(x < 0) {
    xo = -x >> 3;
    t += xo;
    l -= xo;
    if(l < 0) return;
    
    x += xo << 3;

    xo = -x;
    dw = 8 - xo; 
    x  = 0;

    f  = m_bit_field + (*t << 6) + (yo << 3) + xo;

    p  = screen->pixels() + (y * 320) + x;
    h  = dh;


    while(h) {
      w = dw;

      while(w) {
        if(*f) *p = c;

        p++;
        f++;
        w--;
      }

      p += 320 - dw;
      f +=   8 - dw;

      h--;
    }

    t++;
    x = dw;
    l--;

    printf("x=%d xo=%d dw=%d l=%d\n", x, xo, dw, l);

    if(l < 1) return;
  }

  p = screen->pixels() + (y * 320) + x;

  while(1) {

    f  = m_bit_field + (*t << 6) + (yo << 3);
    h  = dh;
    dw = 319 - x;
    if(dw > 8) dw = 8;

    while(h) {

      w = dw;

      while(w) {
        if(*f) *p = c;

        p++;
        f++;
        w--;
      }

      p += 320 - dw;
      f +=   8 - dw;

      h--;
    }

    t++;
    x += 8;
    p -= (dh * 320) - dw;
    l--;

    if(!l) return;
    if(x > 319) return;
  }

}

};
