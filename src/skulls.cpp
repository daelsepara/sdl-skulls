// Standard IO
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// JSON library
#include "nlohmann/json.hpp"

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const double Margin = 0.05;
const double MarginSmall = 0.05;
const int Top = SCREEN_HEIGHT * Margin;
const int Left = SCREEN_WIDTH * Margin;

const SDL_Color clrBL = {0, 0, 0, 0};
const SDL_Color clrDB = {7, 7, 58, 0};
const SDL_Color clrWH = {255, 255, 255, 0};

SDL_Surface *createImage(const char *image)
{
    // Load splash image
    auto surface = IMG_Load(image);

    if (surface == NULL)
    {
        std::cerr << "Unable to load image" << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon, int width, int height)
{
    // The window we'll be rendering to
    *window = NULL;
    *renderer = NULL;

    // Initialize SDL
    if (SDL_Init(flags) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }
    else
    {
        // Create window
        SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN, window, renderer);

        if (window == NULL || renderer == NULL)
        {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        }
        else if (window)
        {
            SDL_SetWindowTitle(*window, title);
        }

        auto surface = createImage(icon);

        if (surface)
        {
            // The icon is attached to the window pointer
            SDL_SetWindowIcon(*window, surface);

            // ...and the surface containing the icon pixel data is no longer required.
            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void fillWindow(SDL_Window *window, SDL_Rect *rect, Uint32 color)
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

void waitForEvent(SDL_Window *window, Uint32 event)
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

bool getHTextMenuChoice(SDL_Window *window, int &current, int num, bool &selected, int buttonh)
{
    SDL_Event result;

    auto quit = false;

    selected = false;

    // some computations for mouse motions
    auto margin1 = (1 - Margin);
    auto margin2 = 2.0 * Margin;
    auto marginleft = (1.0 - margin2);

    auto width = (int)(SCREEN_WIDTH * marginleft);
    auto spacew = width / num;
    auto buttonw = (int)(spacew - Margin * SCREEN_WIDTH);

    while (1)
    {
        SDL_PollEvent(&result);

        if (result.type == SDL_QUIT)
        {
            quit = true;

            break;
        }
        else if (result.type == SDL_KEYDOWN)
        {
            if (result.key.keysym.sym == SDLK_LEFT)
            {
                if (current > 0)
                {
                    current--;
                }
            }
            else if (result.key.keysym.sym == SDLK_RIGHT)
            {
                if (current < num - 1)
                {
                    current++;
                }
            }
            else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
            {
                selected = true;
            }

            SDL_Delay(200);

            break;
        }
        else if (result.type == SDL_CONTROLLERAXISMOTION)
        {
            if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (current == -1)
                {
                    current = 0;
                }
                else if (result.caxis.value < -32000)
                {
                    if (current > 0)
                    {
                        current--;
                    }
                }
                else if (result.caxis.value > 32000)
                {
                    if (current < num - 1)
                    {
                        current++;
                    }
                }

                SDL_Delay(100);

                break;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONUP)
        {
            if (current == -1)
            {
                current = 0;
            }
            else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
            {
                if (current > 0)
                {
                    current--;
                }
            }
            else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
            {
                if (current < num - 1)
                {
                    current++;
                }
            }
            else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
            {
                selected = true;
            }

            SDL_Delay(100);

            break;
        }
        else if (result.type == SDL_MOUSEMOTION)
        {
            auto y = SCREEN_HEIGHT * margin1 - buttonh;

            for (auto i = 0; i < num; i++)
            {
                auto x = spacew * i + 3 * SCREEN_WIDTH * Margin / 2;

                if (result.motion.x >= x && result.motion.x <= x + buttonw - 1 && result.motion.y >= y && result.motion.y <= y + buttonh - 1)
                {
                    current = i;

                    break;
                }
                else
                {
                    current = -1;
                }
            }

            break;
        }
        else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
        {
            if (current >= 0 && current < num)
            {
                selected = true;

                break;
            }
        }

        // Update the surface
        SDL_UpdateWindowSurface(window);
    }

    return quit;
}

void renderImage(SDL_Window *window, SDL_Surface *image, int x, int y)
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

// Render a portion of the text (image) on bounded surface within the specified window
void renderText(SDL_Window *window, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset)
{
    if (window)
    {
        // Get the surface contained by the window
        auto screen = SDL_GetWindowSurface(window);

        if (text && screen)
        {
            SDL_Rect position;
            SDL_Rect src;

            // select portion to render
            src.w = text->w;
            src.h = bounds;
            src.x = 0;
            src.y = offset;

            // specify location within the window
            position.w = text->w;
            position.h = bounds;
            position.x = x;
            position.y = y;

            if (bg != 0)
            {
                SDL_FillRect(screen, &position, bg);
            }

            SDL_BlitSurface(text, &src, screen, &position);

            SDL_FreeSurface(screen);

            screen = NULL;
        }
    }
}

// create text image with line wrap limit
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style = TTF_STYLE_NORMAL)
{
    SDL_Surface *surface;

    TTF_Init();

    auto font = TTF_OpenFont(ttf, font_size);

    if (font)
    {
        TTF_SetFontStyle(font, style);

        surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);
    }

    TTF_CloseFont(font);

    font = NULL;

    TTF_Quit();

    return surface;
}

SDL_Surface *createSurface(int width, int height)
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

void displaySplashScreen(SDL_Window *window)
{
    auto *instructions = "Virtual Reality Adventure Games are solo adventures with a big difference. They're not random. Whether you live or die doesn't depend on a dice roll -- it's up to you.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills: these skills will decide which options are available to you.\n\nAlso note the Life Points and possessions of the character. Life Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during the adventure, but you can never have more Life Points that you started with. You can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventure. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.";
    
    auto *introduction = "The sole survivor of an expedition brings news of disaster. Your twin brother is lost in the trackless western sierra. Resolving to find out his fate, you leave the safety of your home far behind. Your quest takes you to lost jungle cities, across mountains and seas, and even into the depths of the underworld.\n\nYou will plunge into the eerie world of Mayan myth. You will confront ghosts and gods, bargain for your life against wily demons, find allies and enemies among both the living and the dead. If you are breave enough to survive the dangers of the spirit-haunted western desert, you must still confront the wizard called Necklace of skulls in a deadly contest whose stakes are nothing less than your own soul.";

    auto splash = createImage("images/skulls-cover.png");
    auto text = createText(introduction, "fonts/default.ttf", 20, clrWH, SCREEN_WIDTH * 0.85 - splash->w);

    // Dark Blue in ARGB format
    Uint32 bg = 0xFF07073A;

    // Render the image
    if (window && splash && text)
    {
        // Fill the surface with background color
        fillWindow(window, NULL, bg);

        renderImage(window, splash, Left, Top);
        renderText(window, text, bg, Left * 2 + splash->w, Top, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }
}

void renderHTextMenu(SDL_Window *window, const char **choices, const char *ttf, int num, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int buttonh, int fontsize, int style = TTF_STYLE_NORMAL)
{
    if (num > 0)
    {
        auto margin1 = (1 - Margin);
        auto margin2 = 2.0 * Margin;
        auto marginpixels = (int)(SCREEN_WIDTH * Margin);
        auto marginleft = (1.0 - margin2);

        auto width = (int)(SCREEN_WIDTH * marginleft);
        auto spacew = width / num;
        auto buttonw = (int)(spacew - Margin * SCREEN_WIDTH);
        auto screen = SDL_GetWindowSurface(window);

        for (auto i = 0; i < num; i++)
        {
            auto text = createText(choices[i], ttf, fontsize, fg, buttonw, style);
            int x = Left + i * spacew + (buttonw - text->w + marginpixels) / 2;
            int y = SCREEN_HEIGHT * margin1 - buttonh + (buttonh - text->h) / 2;

            SDL_Rect rect;
            rect.w = buttonw;
            rect.h = buttonh;
            rect.x = spacew * i + 3 * SCREEN_WIDTH * Margin / 2;
            rect.y = SCREEN_HEIGHT * margin1 - buttonh;

            SDL_FillRect(screen, &rect, i == selected ? bgSelected : bg);

            renderText(window, text, bg, x, y, fontsize, 0);

            SDL_FreeSurface(text);

            text = NULL;
        }

        SDL_FreeSurface(screen);
    }
}

int initializeGamePads()
{
    if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != 1)
    {
        if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::cerr << "SDL could not initialize gamecontroller! SDL_Error: " << SDL_GetError() << std::endl;
        }
    }

    auto nJoysticks = SDL_NumJoysticks();
    auto numGamepads = 0;

    // Count how many controllers there are
    for (auto i = 0; i < nJoysticks; i++)
    {
        if (SDL_IsGameController(i))
        {
            numGamepads++;
        }
    }

    // If we have some controllers attached
    if (numGamepads > 0)
    {
        for (int i = 0; i < numGamepads; i++)
        {
            // Open the controller and add it to our list
            auto pad = SDL_GameControllerOpen(i);

            if (SDL_GameControllerGetAttached(pad) != 1)
            {
                std::cerr << "Game pad not attached! SDL_Error: " << SDL_GetError() << std::endl;
            }
        }

        SDL_GameControllerEventState(SDL_ENABLE);
    }

    return numGamepads;
}

int main(int argc, char **argsv)
{
    // The window we'll be rendering to
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Necklace of Skulls";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "images/maya.png", SCREEN_WIDTH, SCREEN_HEIGHT);

    auto numGamePads = initializeGamePads();

    if (window)
    {
        displaySplashScreen(window);

        const char *choices[4] = {"New Game", "Load Game", "About", "Exit"};

        auto current = -1;

        auto quit = false;
        auto selected = false;

        while (!quit)
        {
            renderHTextMenu(window, choices, "fonts/default.ttf", 4, current, clrWH, 0, 0xFFFF0000, 48, 22, TTF_STYLE_NORMAL);

            quit = getHTextMenuChoice(window, current, 4, selected, 48);

            if (selected)
            {
                switch (current)
                {
                case 3:

                    quit = true;

                    break;

                default:

                    // redraw main menu screen
                    SDL_SetWindowTitle(window, title);
                    displaySplashScreen(window);
                    SDL_UpdateWindowSurface(window);

                    selected = false;
                    quit = false;

                    break;
                }
            }
        }

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
