// Standard IO
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const char* instructions = "Virtual Reality Adventure Books are solo adventures with a big difference. They're not random. Whether you live or die doesn't depend on a dice roll -- it's up to you.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills: these skills will decide which options are available to you.\n\nAlso note the Life Points and possessions of the character. Life Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during the adventure, but you can never have more Life Points that you started with. You can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventure. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.";

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
        auto screen = SDL_GetWindowSurface(window);

        // Fill the surface with color
        SDL_FillRect(screen, rect, color);

        // Update the surface
        SDL_UpdateWindowSurface(window);

        SDL_FreeSurface(screen);

        screen = NULL;
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
}

SDL_Surface* loadImage(SDL_Window* window, const char* image)
{
    //Loading success flag
    bool success = true;

    //Load splash image
    auto surface = IMG_Load(image);
    
    if (surface == NULL)
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
        auto screen = SDL_GetWindowSurface(window);

        if (image && screen)
        {
            SDL_Rect position;

            position.w = image->w;
            position.h = image->h;
            position.x = x;
            position.y = y;

            SDL_BlitSurface(image, NULL, screen, &position);
            
            SDL_FreeSurface(screen);

            screen = NULL;
        }
    }
}

void renderText(SDL_Window* window, SDL_Surface* text, int x, int y, int bounds, int offset)
{
    if (window)
    {
        // Get the surface contained by the window
        auto screen = SDL_GetWindowSurface(window);

        if (text && screen)
        {
            SDL_Rect position;
            SDL_Rect src;

            src.w = text->w;
            src.h = bounds;
            src.x = 0;
            src.y = offset;

            position.w = text->w;
            position.h = bounds;
            position.x = x;
            position.y = y;

            SDL_BlitSurface(text, &src, screen, &position);
            
            SDL_FreeSurface(screen);

            screen = NULL;
        }
    }
}

SDL_Surface* createText(const char* text, int font_size, SDL_Color textColor, int wrap)
{
    TTF_Init();

    auto font = TTF_OpenFont("fonts/default.ttf", font_size);
    auto surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);

    TTF_CloseFont(font);

    font  = NULL;

    TTF_Quit();

    return surface;
}

SDL_Surface* createSurface(int width, int height)
{
    // Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
    // as expected by OpenGL for textures
    Uint32 rmask, gmask, bmask, amask;

    // SDL interprets each pixel as a 32-bit number, so our masks must depend
    // on the endianness (byte order) of the machine
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

    return SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);
}

const double Margin = 0.05;
const int Top = SCREEN_HEIGHT * Margin;
const int Left = SCREEN_WIDTH * Margin;

const SDL_Color clrBL = {0, 0, 0, 0};
const SDL_Color clrDB = {7, 7, 58, 0};

void displaySplashScreen(SDL_Window *window)
{
    auto splash = loadImage(window, "images/skulls-cover.png");
    auto instructionsText = createText(instructions, 16, clrBL, SCREEN_WIDTH * 0.85 - splash->w);

    // Render the image
    if (window && splash && instructionsText)
    {
        // Fill the surface with white color
        fillWindow(window, NULL, 0xFFFFFF);

        renderImage(window, splash, Left, Top);
        renderText(window, instructionsText, Left * 2 + splash->w, Top, instructionsText->h, 0);

        SDL_FreeSurface(splash);
        SDL_FreeSurface(instructionsText);

        splash = NULL;
        instructionsText = NULL;
    }
}

int main(int argc, char** argsv)
{
    // The window we'll be rendering to
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    
    createWindow(SDL_INIT_VIDEO, &window, &renderer, "Necklace of Skulls", SCREEN_WIDTH, SCREEN_HEIGHT);

    if (window)
    {
        displaySplashScreen(window);

        waitForEvent(window, SDL_QUIT);

        // Destroy window
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);

        renderer = NULL;
        window = NULL;
    }

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
