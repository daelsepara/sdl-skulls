// Standard IO
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// JSON library
#include "nlohmann/json.hpp"

#include "controls.hpp"
#include "input.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "story.hpp"

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const double Margin = 0.05;

const SDL_Color clrBK = {0, 0, 0, 0};
const SDL_Color clrDB = {7, 7, 58, 0};
const SDL_Color clrWH = {255, 255, 255, 0};

const Uint32 intBK = 0x00000000;
const Uint32 intRD = 0XFFFF0000;
const Uint32 intWH = 0XFFFFFFFF;
const Uint32 intGR = 0XFF7F7F7F;

// Dark Blue in ARGB format
const Uint32 intDB = 0xFF07073A;

// Beige in ARG format
const Uint32 intBE = 0xFFF5F5DC;

SDL_Surface *createImage(const char *image)
{
    // Load splash image
    auto surface = IMG_Load(image);

    if (surface == NULL)
    {
        std::cerr << "Unable to load image " << image << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    return surface;
}

void createWindow(Uint32 flags, SDL_Window **window, SDL_Renderer **renderer, const char *title, const char *icon)
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
        /*
        SDL_DisplayMode mode;

        SDL_GetCurrentDisplayMode(0, &mode);

        SCREEN_WIDTH = mode.w;
        SCREEN_HEIGHT = mode.h;
        */

        // Create window
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, window, renderer);

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
    }
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
            SDL_Rect dst;
            SDL_Rect src;

            // select portion to render
            src.w = text->w;
            src.h = bounds;
            src.y = offset;
            src.x = 0;

            // specify location within the window
            dst.w = text->w;
            dst.h = bounds;
            dst.x = x;
            dst.y = y;

            if (bg != 0)
            {
                SDL_FillRect(screen, &dst, bg);
            }

            SDL_BlitSurface(text, &src, screen, &dst);

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

void renderTextButtons(SDL_Window *window, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style = TTF_STYLE_NORMAL)
{
    if (controls.size() > 0)
    {
        auto screen = SDL_GetWindowSurface(window);

        for (auto i = 0; i < controls.size(); i++)
        {
            auto text = createText(controls[i].Text, ttf, fontsize, fg, controls[i].W, style);

            int x = controls[i].X + (controls[i].W - text->w) / 2;
            int y = controls[i].Y + (controls[i].H - text->h) / 2;

            SDL_Rect rect;

            rect.w = controls[i].W;
            rect.h = controls[i].H;
            rect.x = controls[i].X;
            rect.y = controls[i].Y;

            SDL_FillRect(screen, &rect, i == selected ? bgSelected : bg);

            renderText(window, text, bg, x, y, fontsize, 0);

            SDL_FreeSurface(text);

            text = NULL;
        }

        SDL_FreeSurface(screen);

        screen = NULL;
    }
}

void renderButtons(SDL_Window *window, std::vector<Button> controls, int current, int fg, int bg, int pts)
{
    if (controls.size() > 0)
    {
        auto screen = SDL_GetWindowSurface(window);

        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            rect.w = controls[i].W + 2 * pts;
            rect.h = controls[i].H + 2 * pts;
            rect.x = controls[i].X - pts;
            rect.y = controls[i].Y - pts;

            SDL_FillRect(screen, &rect, i == current ? fg : bg);

            renderImage(window, controls[i].Surface, controls[i].X, controls[i].Y);
        }

        SDL_FreeSurface(screen);

        screen = NULL;
    }
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int buttonh, int startx, int starty)
{
    std::vector<TextButton> controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin1 = (1.0 - Margin);
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin);
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto spacew = width / num;
        auto buttonw = spacew - pixels;
        auto space = pixels / 2;

        for (auto i = 0; i < num; i++)
        {
            int left = i > 0 ? i - 1 : i;
            int right = i < num - 1 ? i + 1 : i;
            int up = i;
            int down = i;

            auto x = startx + i * (buttonw + space * 2) + space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, starty, buttonw, buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

bool aboutScreen(SDL_Window *window)
{
    auto quit = false;

    auto *about = "Virtual Reality Adventure Games are solo adventures with a big difference. They're not random. Whether you live or die doesn't depend on a dice roll -- it's up to you.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills: these skills will decide which options are available to you.\n\nAlso note the Life Points and possessions of the character. Life Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during the adventure, but you can never have more Life Points that you started with. You can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventure. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.";

    auto splash = createImage("images/pyramid.png");
    auto text = createText(about, "fonts/default.ttf", 18, clrWH, SCREEN_WIDTH * (1.0 - 3 * Margin) - splash->w);
    auto startx = SCREEN_WIDTH * Margin;
    auto starty = SCREEN_HEIGHT * Margin;

    // Render the image
    if (window && splash && text)
    {
        // Fill the surface with background color
        fillWindow(window, NULL, intDB);

        renderImage(window, splash, startx, starty);

        renderText(window, text, intDB, startx * 2 + splash->w, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;

        SDL_SetWindowTitle(window, "About the game");

        auto selected = false;
        auto current = -1;

        auto buttonw = 150;
        auto buttonh = 48;
        auto space = 10;

        auto starty = (int)(SCREEN_HEIGHT * (1 - Margin) - buttonh);

        auto controls = std::vector<TextButton>();

        controls.push_back(TextButton(0, "Back", 0, 0, 0, 0, startx, starty, buttonw, buttonh, ControlType::BACK));

        while (!quit)
        {
            renderTextButtons(window, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            quit = getInput(window, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == ControlType::BACK)
            {
                break;
            }
        }
    }

    return quit;
}

template <typename T>
void processStory(T story)
{
}

bool storyScreen(SDL_Window *window)
{
    auto quit = false;

    auto splash = createImage(prologue.Image);

    auto startx = (SCREEN_WIDTH * Margin);
    auto textx = (SCREEN_WIDTH * Margin) * 2 + splash->w;
    auto texty = (SCREEN_HEIGHT * Margin);

    auto buttonw = 64;
    auto buttonh = 64;
    auto space = 20;
    auto gridsize = buttonw + space;
    auto pts = 8;
    auto arrows = 32;

    auto buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrows + space);

    auto text = createText(prologue.Text, "fonts/default.ttf", 20, clrDB, textwidth, TTF_STYLE_NORMAL);

    // Render the image
    if (window && splash && text)
    {
        // Fill the surface with background color
        fillWindow(window, NULL, intWH);

        renderImage(window, splash, startx, texty);

        SDL_FreeSurface(splash);

        splash = NULL;

        SDL_SetWindowTitle(window, "Necklace of Skulls: Prologue");

        auto selected = false;
        auto current = -1;

        auto controls = std::vector<Button>();

        auto offset = 0;

        auto bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - space * 2;

        controls.push_back(Button(0, "images/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrows, texty + pts, ControlType::SCROLL_UP));
        controls.push_back(Button(1, "images/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrows, texty + bounds - arrows - pts, ControlType::SCROLL_DOWN));
        controls.push_back(Button(2, "images/map.png", 1, 3, 1, 2, startx, buttony, ControlType::MAP));
        controls.push_back(Button(3, "images/disk.png", 2, 4, 1, 3, startx + gridsize, buttony, ControlType::GAME));
        controls.push_back(Button(4, "images/next.png", 3, 5, 1, 4, startx + 2 * gridsize, buttony, ControlType::NEXT));
        controls.push_back(Button(5, "images/exit.png", 4, 5, 1, 5, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, ControlType::QUIT));

        auto screen = SDL_GetWindowSurface(window);

        SDL_FreeSurface(screen);

        screen = NULL;

        auto scrollSpeed = 20;
        auto hold = false;

        while (!quit)
        {
            renderText(window, text, intBE, textx, texty, bounds, offset);

            renderButtons(window, controls, current, intGR, intWH, pts);

            bool scrollUp = false;
            bool scrollDown = false;

            quit = getInput(window, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == ControlType::SCROLL_UP || (controls[current].Type == ControlType::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;
                    }

                    if (offset < 0)
                    {
                        offset = 0;
                    }
                }
                else if (controls[current].Type == ControlType::SCROLL_DOWN || (controls[current].Type == ControlType::SCROLL_DOWN && hold) || scrollDown)
                {
                    if (offset < text->h - bounds)
                    {
                        offset += scrollSpeed;
                    }

                    if (offset > text->h - bounds)
                    {
                        offset = text->h - bounds;
                    }
                }
                else if (controls[current].Type == ControlType::QUIT && !hold)
                {
                    break;
                }
            }
        }

        SDL_FreeSurface(text);

        text = NULL;
    }

    return quit;
}

bool mainScreen(SDL_Window *window)
{
    auto *introduction = "The sole survivor of an expedition brings news of disaster. Your twin brother is lost in the trackless western sierra. Resolving to find out his fate, you leave the safety of your home far behind. Your quest takes you to lost jungle cities, across mountains and seas, and even into the depths of the underworld.\n\nYou will plunge into the eerie world of Mayan myth. You will confront ghosts and gods, bargain for your life against wily demons, find allies and enemies among both the living and the dead. If you are breave enough to survive the dangers of the spirit-haunted western desert, you must still confront the wizard called Necklace of skulls in a deadly contest whose stakes are nothing less than your own soul.";

    auto splash = createImage("images/skulls-cover.png");
    auto text = createText(introduction, "fonts/default.ttf", 20, clrWH, SCREEN_WIDTH * 0.85 - splash->w);
    auto title = "Necklace of Skulls";
    auto startx = SCREEN_WIDTH * Margin;
    auto starty = SCREEN_HEIGHT * Margin;

    // Render window
    if (window && splash && text)
    {
        SDL_SetWindowTitle(window, title);

        // Fill the surface with background color
        fillWindow(window, NULL, intDB);

        renderImage(window, splash, startx, starty);
        renderText(window, text, intDB, startx * 2 + splash->w, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return false;
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

template <typename Function>
bool renderWindow(SDL_Window *window, Function displayScreen)
{
    auto result = displayScreen(window);

    SDL_Delay(100);

    return result;
}

int main(int argc, char **argsv)
{
    // The window we'll be rendering to
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Necklace of Skulls";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "images/maya.png");

    auto numGamePads = initializeGamePads();
    auto quit = false;
    auto startx = SCREEN_WIDTH * Margin;

    if (window)
    {
        quit = renderWindow(window, mainScreen);

        const char *choices[4] = {"New Game", "Load Game", "About", "Exit"};

        auto current = -1;

        auto selected = false;

        auto buttonh = 48;

        auto controls = createHTextButtons(choices, 4, buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

        controls[0].Type = ControlType::NEW;
        controls[1].Type = ControlType::LOAD;
        controls[2].Type = ControlType::ABOUT;
        controls[3].Type = ControlType::QUIT;

        while (!quit)
        {
            renderTextButtons(window, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 22, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            quit = getInput(window, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case ControlType::NEW:

                    quit = renderWindow(window, storyScreen);

                    current = -1;

                    selected = false;

                    break;

                case ControlType::ABOUT:

                    quit = renderWindow(window, aboutScreen);

                    current = -1;

                    selected = false;

                    break;

                case ControlType::QUIT:

                    quit = true;

                    break;

                default:

                    selected = false;
                    quit = false;

                    break;
                }

                if (!quit)
                {
                    quit = renderWindow(window, mainScreen);
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
    IMG_Quit();

    SDL_Quit();

    return 0;
}
