
#include <string>
#include <SDL/SDL.h>
#include <cstring>

#include "screen.hh"
#include "font.hh"
#include "ega-font-bytes.h"

using namespace std;

namespace EGA {
  
SDL_Color Screen::s_palette[] = {
  {   0,   0,   0 },
  {   0,   0, 127 },
  {   0, 127,   0 },
  {   0, 127, 127 },
  { 127,   0,   0 },
  { 127,   0, 127 },
  { 127, 127,   0 },
  { 127, 127, 127 },
  {  64,  64,  64 },
  {  64,  64, 255 },
  {  64, 255,  64 },
  {  64, 255, 255 },
  { 255,  64,  64 },
  { 255,  64, 255 },
  { 255, 255,  64 },
  { 255, 255, 255 },
  {  16,  16,  16 }
};


Screen::Screen() { 
  m_screen     = NULL;
  m_is_running = true;
  m_keys       = NULL;

  m_font.load( 32, 96, ega_font_bytes );
}

Screen::~Screen() { 
  if(m_screen) SDL_Quit();
  if(m_keys) delete m_keys;
  if(m_pixels) delete m_pixels;
}

bool Screen::open( const string &t ) {

  m_pixels = new pixel_t[320 * 200];
  if(!m_pixels) return false;

  m_keys = new int [K_MAX];
  if(!m_keys) return false;

  m_screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE); 
  if( !m_screen ) return false;

  SDL_SetPalette( m_screen, SDL_LOGPAL, s_palette, 0, 17 ); 
  SDL_WM_SetCaption( t.c_str(), 0 );

  return true;
}

bool Screen::is_running() {
  return m_is_running;
}

void Screen::tick() { 

  SDL_Event event;

  scale_flip();
  
  SDL_Flip( m_screen );
  SDL_Delay( 20 );

  while( SDL_PollEvent(&event) ) {
    switch( event.type ) {

      case SDL_QUIT:
        m_is_running = false;
        return;
        break;

      case SDL_KEYDOWN:
        {
          switch( event.key.keysym.sym ) {
            case SDLK_UP:    m_keys[K_UP]    = 1; break;
            case SDLK_DOWN:  m_keys[K_DOWN]  = 1; break;
            case SDLK_LEFT:  m_keys[K_LEFT]  = 1; break;
            case SDLK_RIGHT: m_keys[K_RIGHT] = 1; break;
            default: break;
          }
        
          break;
        }

      case SDL_KEYUP:
        {
          switch( event.key.keysym.sym ) {
            case SDLK_UP:    m_keys[K_UP]    = 0; break;
            case SDLK_DOWN:  m_keys[K_DOWN]  = 0; break;
            case SDLK_LEFT:  m_keys[K_LEFT]  = 0; break;
            case SDLK_RIGHT: m_keys[K_RIGHT] = 0; break;
            default: break;
          }

          break;
        }

      default:
        break;
    }
  }
}

string Screen::error_message() {
  return string(SDL_GetError());
}

void Screen::puts( int x, int y, int c, char *s ) { 
  m_font.puts( this, x, y, c, strlen(s), s );
}

EGA::Font& Screen::font() {
  return m_font;
}

void Screen::scale_flip() {

  pixel_t *vbuff;
  pixel_t *src;
  int c;
  int r = 200;
  pixel_t *row;

  if( SDL_LockSurface( m_screen ) == -1 ) {
    return;
  }

  vbuff = (pixel_t *)m_screen->pixels;
  src   = m_pixels;

  memset( vbuff, 16, 40 * m_screen->w );
  vbuff += 40 * m_screen->w;

  while( r ) {

    c   = 320;
    row = vbuff;

    while( c ) {
      *row = *src; row++;
      *row = *src; row++;

      src++;
      c--;
    }

    memcpy(row, vbuff, 640);
    vbuff += 1280;
    r--;
  }

  memset( vbuff, 16, 40 * m_screen->w );

  SDL_UnlockSurface( m_screen );

  memset( m_pixels, 0, 320 * 200 * sizeof(pixel_t)); 
}

void Screen::pset( int x, int y, int c ) {
  m_pixels[y * 320 + x] = c;
}

Screen::pixel_t *Screen::pixels() {
  return m_pixels;
}

};

