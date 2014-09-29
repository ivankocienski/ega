
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void process_char( int x, int y, unsigned char *pixels, int pitch ) {

  int cx;
  int cy;

  x <<= 3;
  y <<= 3;

  pixels += (y * pitch) + x;

  for( cy = 0; cy < 8; cy++ ) {
    for( cx = 0; cx < 8; cx++ ) {

      if( *pixels )
        printf( "1, " );
      else
        printf( "0, " );

      pixels++; 
    }

    printf("\n");
    pixels += (pitch - 8);
  }


}

int main( int argc, char ** argv ) {

  SDL_Surface *screen;
  SDL_Surface *surface;
  int x;
  int y;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
    fprintf(stderr, "E: could not init SDL\n" );
    return -1;
  }

  if( SDL_SetVideoMode( 100, 100, 0, SDL_SWSURFACE ) < 0 ) {
    fprintf(stderr, "E: could not open screen\n" );
    return -1;
  }

  surface = IMG_Load( "/home/ivan/dev/C++/adventure/font.png" );
  if( !surface ) {
    fprintf(stderr, "E: could not load image\n" );
    return -1;
  }

  if( SDL_LockSurface( surface ) < 0 ) {
    fprintf(stderr, "E: could not lock surface\n");
    return -1;
  }

  for(y = 0; y < 6; y++) {
    for(x = 0; x < 16; x++) {

      process_char( x, y, (unsigned char *)surface->pixels, surface->pitch );
    }
  } 

  SDL_UnlockSurface(surface);

  SDL_Quit();

  return 0; 
}

