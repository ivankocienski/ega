
#pragma once

#include <SDL/SDL.h>
#include <string>

#include "font.hh"

namespace EGA {
  
class Screen {
public:

  typedef unsigned char pixel_t;

private:

  static SDL_Color s_palette[];
  
  SDL_Surface *m_screen;
  bool         m_is_running;
  pixel_t     *m_pixels;
  Font         m_font;
  

  void scale_flip();

public:

  enum {
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_ENTER,
    K_MAX
  };

  int *m_keys;

  Screen();
  ~Screen();

  bool open(const std::string &);
  bool is_running();

  void tick();
  void puts( int, int, int, char* );
  void pset( int, int, int );

  Font& font();

  pixel_t *pixels();

  std::string error_message();
};

};

