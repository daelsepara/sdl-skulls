// Using SDL and standard IO
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void createWindow(Uint32 flags, SDL_Window** window, SDL_Renderer** renderer, const char* title, int width, int height)
{
    // The window we'll be rendering to
    *window = NULL;
    *renderer = NULL;

    // Initialize SDL
    if (SDL_Init(flags) < 0)
    {
        std::cerr <<  "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Create window
        SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer);
        
        if (window == NULL || renderer == NULL)
        {
            std::cerr <<  "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);
        }
    }
}

void fillWindow(SDL_Window* window, SDL_Rect* rect, Uint32 color)
{
    if (window)
    {
        // Get the surface contained by the window
        SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

        // Fill the surface with color
        SDL_FillRect(screenSurface, rect, color);

        // Update the surface
        SDL_UpdateWindowSurface(window);
    }
}

void waitForEvent(SDL_Window* window, Uint32 event)
{
    SDL_Event result;

    while (1)
    {
        SDL_PollEvent(&result);

        if (result.type == event)
        {
           break; 
        }

        // Update the surface
        SDL_UpdateWindowSurface(window);
    }

    return;
}

SDL_Surface* loadImage(SDL_Window* window, const char* image)
{
    //Loading success flag
    bool success = true;

    //Load splash image
    SDL_Surface* surface = IMG_Load(image);
    
    if(surface == NULL)
    {
        std::cerr <<  "Unable to load image" << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

void renderImage(SDL_Window* window, SDL_Surface* image, int x, int y)
{
    if (window)
    {
        // Get the surface contained by the window
        SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

        if (image && screenSurface)
        {
            SDL_Rect position;

            position.w = image->w;
            position.h = image->h;
            position.x = x;
            position.y = y;

            SDL_BlitSurface(image, NULL, screenSurface, &position);
        }
    }
}

SDL_Surface* createText(const char* text, int font_size, SDL_Color textColor, SDL_Color bg)
{
    TTF_Init();

    TTF_Font *font = TTF_OpenFont("fonts/default.ttf", font_size);
    SDL_Surface *surface = TTF_RenderText_Shaded(font, text, textColor, bg);

    TTF_Quit();

    return surface;
}

SDL_Surface* createSurface(int width, int height)
{
    /* Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
       as expected by OpenGL for textures */
    SDL_Surface* surface;
    Uint32 rmask, gmask, bmask, amask;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    surface = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    return surface;
}

int main(int argc, char** argsv)
{
    // The window we'll be rendering to
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    createWindow(SDL_INIT_VIDEO, &window, &renderer, "Necklace of Skulls", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (window)
    {
        // Fill the surface with white color
        fillWindow(window, NULL, 0xFFFFFF);

        SDL_Surface* splashScreen = loadImage(window, "images/skulls-cover.png");
        SDL_Surface* text = createText("Necklace of Skulls", 24, {0, 0, 0, 0}, {255, 255, 255, 255});
        
        // Render the image
        if (splashScreen)
        {
            renderImage(window, text, SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.05);
            renderImage(window, splashScreen, SCREEN_WIDTH * 0.05, SCREEN_HEIGHT * 0.10);
        } 

        waitForEvent(window, SDL_QUIT);

        if (splashScreen)
        {
            SDL_FreeSurface(splashScreen);
        }

        // Destroy window
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
    }

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
