/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, standard IO, and strings
#include<SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Key press surfaces constants
enum KeyPressSurfaces
{
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image
SDL_Surface* loadSurface( std::string path );

SDL_Texture* loadTexture( std::string path );


//-------objects------
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed image
SDL_Surface* gStretchedSurface = NULL;

SDL_Surface* gSurfaceToDisplay = NULL;

SDL_Surface* gCurrentPressedSurfaceStretched[4];

SDL_Renderer* gRenderer = NULL;

SDL_Texture* gTexture = NULL;

bool init()
{

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        return false;
	}

    //Create window
    gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    if( gWindow == NULL )
    {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }

    //create renderer
    gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
    if( gRenderer == NULL )
    {
        printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
        return false;
    }
    //Initialize renderer color
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );


    //initialize png loading
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("failed to load png flags!\n");
        return false;
    }
    //Get window surface
    gScreenSurface = SDL_GetWindowSurface( gWindow );

    return true;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load stretching surface
	gTexture = loadTexture( "05_optimized_surface_loading_and_soft_stretching/texture.png" );
	if( gTexture == NULL )
	{
		printf( "Failed to load texture image!\n" );
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gTexture );
	gTexture = NULL;


	//destroy renderer
	SDL_DestroyRenderer( gRenderer );
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}



/*
* Texture loader function, will be used instead of surface loader
*/
SDL_Texture* loadTexture( std::string path )
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), IMG_GetError() );
		return NULL;
	}

	//Texture is created from surface pixels
	newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
    if( newTexture == NULL)
    {
        printf("Unable to create texture! error: %s\n", SDL_GetError() );
        return NULL;
    }
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
		exit(1);
	}


    //Load media
    if( !loadMedia() )
    {
        printf( "Failed to load media!\n" );
        exit(1);
    }





	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//While application is running
	while( !quit )
	{
	    //handle queued events
	    while( SDL_PollEvent( &e ) != 0)
        {
            if(e.type == SDL_QUIT) quit = true;
        }

        //clear
        SDL_RenderClear( gRenderer );
        //render texture
        SDL_RenderCopy( gRenderer, gTexture, NULL, NULL) ;
        //update
        SDL_RenderPresent( gRenderer );
	}
	close();

	return 0;
}
