
#pragma once

#include <string>

#include "screen.hh"

namespace platform {

class TileSet {
private:

  Screen &m_screen;

  int m_tile_width;
  int m_tile_height;
  int m_max_tile;

  Screen::pixel_t *m_surface;
  int m_surface_width;
  int m_surface_height;

public:
  
  TileSet(Screen&);
  ~TileSet();

  bool load( int, int, const std::string& );
  void plot( int, int, int );
  int  count();
};

};

