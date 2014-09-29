
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <vector>

#include "screen.hh"
#include "tile_set.hh"
#include "font.hh"

#include "ega-font-bytes.h"

using namespace platform;
using namespace std;

inline double frand() {
  return (double)rand() / (double)RAND_MAX; 
}

class Bouncer {
private:

  TileSet *m_tiles;

  float    m_x;
  float    m_y;
  float    m_xinc;
  float    m_yinc;
  int      m_spr; 

public:

  Bouncer() { }

  Bouncer( TileSet &ts ) {
    m_tiles = &ts;
    m_xinc  = (0.5 - frand()) * 2;
    m_yinc  = (0.5 - frand()) * 2;
    m_x     = frand() * 320;
    m_y     = frand() * 200;
    m_spr   = rand() % 3;
  }

  void grind( Screen& scr ) {

    m_x += m_xinc;
    m_y += m_yinc;

    if( m_x < -40 || m_x > 330 ) {
      m_xinc = -m_xinc;
      m_yinc = (0.5 - frand()) * 2; 
    }

    if( m_y < -40 || m_y > 330 ) {
      m_yinc = -m_yinc;
      m_xinc = (0.5 - frand()) * 2; 
    }

    m_tiles->plot( (int)m_x, (int)m_y, m_spr ); 
  }
};

typedef vector<Bouncer> bounce_vector_t;

int main( int argc, char** argv ) {


  Font font;
  font.load( 32, 96, ega_font_bytes );

  Screen screen;
  screen.open( "EGA Demo" );

  TileSet sprite_tiles(screen);
  sprite_tiles.load( 32, 32, "data/sprites.png" );

  bounce_vector_t bouncers;

  for( int i = 0; i < 20; i++ ) {
    bouncers.push_back( Bouncer(sprite_tiles) );
  }

  srand( time( NULL ));

  int offset = 30;

  while(screen.is_running()) {
    screen.tick();

    for( bounce_vector_t::iterator it = bouncers.begin(); it != bouncers.end(); it++ )  
      (*it).grind(screen);

    //font.puts( &screen, offset+1, ,  1, 12, "Hello, World" );
    font.puts( &screen, offset, 196, 15, 12, "Hello, World" );

    offset++;
    if(offset > 330) offset = 200;
    
    //charset.plot( 10, 10, 0 ); 
    
/*     screen.pset(  10,  10, 15 );
 *     screen.pset( 100,  10, 13 );
 *     screen.pset( 100, 100, 11 );
 *     screen.pset(  10, 100,  9 );
 */
  }

  return 0; 
}


#if 0

#include <iostream>
#include <cstdlib>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

/* not sure what this is supposed to be... */

using namespace std;

static SDL_Surface *screen  = NULL;
static SDL_Surface *sprites = NULL;
static bool run_loop        = true;
static int *key_state       = NULL;

static void cleanup() {
  if(screen) SDL_Quit();
}

void draw_sprite( int x, int y, int n ) {

  SDL_Rect dst = { x, y, 0, 0 };
  SDL_Rect src = { n * 19, 0, 19, 22 };

  SDL_BlitSurface( sprites, &src, screen, &dst ); 
}

int main( int argc, char** argv ) {

  unsigned short anim_count = 0;
  SDL_Event event;

  atexit( cleanup );

  screen = SDL_SetVideoMode(640, 480, 8, SDL_SWSURFACE); 
  if( !screen ) {
    cerr << "Boobs! failed to init video" << endl;
    return -1; 
  }

  sprites = IMG_Load( "data/char-test.png" );
  if( !sprites ) {
    cerr << "Boobs! failed to load sprites" << endl;
    return -1; 
  }
  
  SDL_SetColorKey(sprites, SDL_SRCCOLORKEY, 255);

  key_state = new int[SDLK_LAST];

  SDL_SetPalette( screen, SDL_LOGPAL, palette, 0, 16 );

  while(run_loop) {

    while( SDL_PollEvent(&event) ) {

      switch( event.type ) {

        case SDL_QUIT:
          run_loop = false;
          break;

        case SDL_KEYDOWN:
          key_state[(int)event.key.keysym.sym] = 1;
          break;


        case SDL_KEYUP:
          key_state[(int)event.key.keysym.sym] = 0;
          break;
      } 
    }

    SDL_FillRect( screen, NULL, 4 );

    draw_sprite( 10, 10, (anim_count >> 1) & 7 );

    SDL_Flip(screen);
    SDL_Delay( 20 ); 

    anim_count++;
  }


  return 0;
}

#endif

