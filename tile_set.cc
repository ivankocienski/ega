
#include "screen.hh"
#include "tile_set.hh"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

using namespace std;

namespace EGA {

TileSet::TileSet( Screen& s) : m_screen(s) {
  m_surface = NULL;
}

TileSet::~TileSet() {
  if(m_surface) delete m_surface;
}

bool TileSet::load( int tw, int th, const std::string& fn ) {

  SDL_Surface *sfc = NULL;
  bool okay = false;

  for(;;) {
    sfc = IMG_Load( fn.c_str() );
    if(!sfc) break;

    if(sfc->h < th) break;
    if(sfc->w < tw) break;

    if( sfc->format->BitsPerPixel != 8 ) break;

    if( SDL_LockSurface(sfc) == -1 ) break;

    m_surface = new Screen::pixel_t [ sfc->w * sfc->h ];
    if( !m_surface ) break;

    // naive copy ATM.
    memcpy( m_surface, (Screen::pixel_t *)sfc->pixels, sfc->w * sfc->h );

    SDL_UnlockSurface( sfc ); 

    m_max_tile       = sfc->w / tw;
    m_tile_width     = tw;
    m_tile_height    = th;
    m_surface_width  = sfc->w;
    m_surface_height = sfc->h;

    okay = true;
    break;
  }

  if(sfc) SDL_FreeSurface( sfc );
  return okay;
}

void TileSet::plot( int x, int y, int n ) {

  Screen::pixel_t *vbuff;
  Screen::pixel_t *src;

  if( x < -m_tile_width ) return;
  if( y < -m_tile_height ) return;
  if( x > 319 ) return;
  if( y > 199 ) return;
  if( n < 0 || n >= m_max_tile ) return;

  int dw;
  int dh;
  int xo;
  int yo;
  int w;

  if( x < 0 ) {
    // subtract x from width
    xo = -x;
    dw = m_tile_width + x;
    x  = 0;

  } else {

    xo = 0;

    if( x > ( 319 - m_tile_width )) 
      dw = 319 - x;

    else 
      dw  = m_tile_width;
  }

  if( y < 0 ) {

    yo = -y;
    dh = m_tile_height + y;
    y  = 0;

  } else {
    yo = 0;

    if( y > ( 199 - m_tile_height )) 
      dh = 199 - y;

    else
      dh = m_tile_height;
  }
  

  vbuff = m_screen.pixels() + y * 320 + x;
  src   = m_surface + (yo * m_surface_width) + (n * m_tile_width) + xo;

  while(dh) {

    w = dw;

    while(w) {
      if( *src != 255 )
        *vbuff = *src;

      vbuff++;
      src++;
      w--;
    }

    vbuff += 320 - dw;
    src   += m_surface_width - dw;

    dh--;
  }

}

int TileSet::count() {
  return m_max_tile;
}

};
