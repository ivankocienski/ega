
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <vector>

#include "screen.hh"
#include "tile_set.hh"

using namespace EGA;
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

  void animate( Screen& scr ) {

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

class Star {
private:

  int m_xpos;
  int m_ypos;
  int m_color;
  int m_twinkle;
  int m_twinkle_len;

public:

  Star() {
    m_xpos = rand() % 320;
    m_ypos = rand() % 200; 
    m_twinkle_len = rand();

    if(m_twinkle_len & 1) {
      m_twinkle_len = 50 + rand() % 150;
      m_twinkle = rand() % m_twinkle_len;
    }
  }

  void animate( Screen& scr ) {
    int c = 15;

    if( m_twinkle_len ) {
      if( m_twinkle > 5 ) {
        c = 8;
      }

      if( m_twinkle > -1 ) {
        m_twinkle--;
      } else {
        m_twinkle = m_twinkle_len;
      }
    }

    scr.pset( m_xpos,  m_ypos, c );
  }
};

typedef vector<Bouncer> bounce_vector_t;
typedef vector<Star>    star_vector_t;

int main( int argc, char** argv ) {

  Screen screen;
  screen.open( "EGA Demo" );

  TileSet sprite_tiles(screen);
  sprite_tiles.load( 32, 32, "data/sprites.png" );

  star_vector_t stars;
  for( int i = 0; i < 100; i++ ) {
    stars.push_back( Star() );
  }

  bounce_vector_t bouncers;
  for( int i = 0; i < 10; i++ ) {
    bouncers.push_back( Bouncer(sprite_tiles) );
  }

  srand( time( NULL ));

  while(screen.is_running()) {
    screen.tick();

    for( star_vector_t::iterator it = stars.begin(); it != stars.end(); it++ ) 
      (*it).animate(screen);

    for( bounce_vector_t::iterator it = bouncers.begin(); it != bouncers.end(); it++ )  
      (*it).animate(screen);

    screen.puts( 10, 10, 15, "///  EGA DEMO \\\\\\" );

  }

  return 0; 
}


