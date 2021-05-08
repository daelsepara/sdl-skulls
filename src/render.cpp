#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

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

Uint8 R(Uint32 c) { return (Uint8)((c & 0xFF0000) >> 16); }
Uint8 G(Uint32 c) { return (Uint8)((c & 0x00FF00) >> 8); }
Uint8 B(Uint32 c) { return (Uint8)(c & 0x0000FF); }
Uint8 A(Uint32 c) { return (Uint8)(c >> 24); }

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

void renderImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y)
{
    if (image && renderer)
    {
        SDL_Rect position;

        position.w = image->w;
        position.h = image->h;
        position.x = x;
        position.y = y;

        auto texture = SDL_CreateTextureFromSurface(renderer, image);

        if (texture)
        {
            SDL_Rect src;

            src.w = image->w;
            src.h = image->h;
            src.x = 0;
            src.y = 0;

            SDL_RenderCopy(renderer, texture, &src, &position);

            SDL_DestroyTexture(texture);

            texture = NULL;
        }
    }
}

// Render a portion of the text (image) on bounded surface within the specified window
void renderText(SDL_Renderer *renderer, SDL_Surface *text, Uint32 bg, int x, int y, int bounds, int offset)
{
    if (renderer)
    {
        if (text && renderer)
        {
            SDL_Rect dst;
            SDL_Rect src;

            // select portion to render
            src.w = text->w;
            src.h = text->h < bounds ? text->h : bounds;
            src.y = offset;
            src.x = 0;

            // specify location within the window
            dst.w = text->w;
            dst.h = text->h < bounds ? text->h : bounds;
            dst.x = x;
            dst.y = y;

            if (bg != 0)
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
                SDL_RenderFillRect(renderer, &dst);
            }

            auto texture = SDL_CreateTextureFromSurface(renderer, text);

            if (texture)
            {
                SDL_RenderCopy(renderer, texture, &src, &dst);

                SDL_DestroyTexture(texture);

                texture = NULL;
            }
        }
    }
}

int main(int argc, char **argv)
{
    auto *introduction = "The sole survivor of an expedition brings news of disaster. Your twin brother is lost in the trackless western sierra. Resolving to find out his fate, you leave the safety of your home far behind. Your quest takes you to lost jungle cities, across mountains and seas, and even into the depths of the underworld.\n\nYou will plunge into the eerie world of Mayan myth. You will confront ghosts and gods, bargain for your life against wily demons, find allies and enemies among both the living and the dead. If you are breave enough to survive the dangers of the spirit-haunted western desert, you must still confront the wizard called Necklace of skulls in a deadly contest whose stakes are nothing less than your own soul.";

    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_INIT_VIDEO, &window, &renderer);

    if (window && renderer)
    {
        auto image = createImage("images/skulls-cover.png");
        auto text = createText(introduction, "fonts/default.ttf", 20, {255, 255, 255}, SCREEN_WIDTH * (1.0 - 3.0 * Margin) - image->w, TTF_STYLE_NORMAL);
        
        auto startx = SCREEN_WIDTH * Margin;
        auto starty = SCREEN_HEIGHT * Margin;

        while (1)
        {
            SDL_SetRenderDrawColor(renderer, R(intDB), G(intDB), B(intDB), A(intDB));
            SDL_RenderClear(renderer);

            renderImage(renderer, image, startx, starty);
            renderText(renderer, text, intDB, startx * 2 + image->w, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);

            // call after rendering everything on screen
            SDL_RenderPresent(renderer);

            SDL_Event event;

            SDL_PollEvent(&event);

            if (event.type == SDL_QUIT)
            {
                break;
            }
        }

        SDL_FreeSurface(text);

        text = NULL;

        SDL_FreeSurface(image);

        image = NULL;

        SDL_DestroyRenderer(renderer);

        renderer = NULL;

        SDL_DestroyWindow(window);

        window = NULL;

        IMG_Quit();

        SDL_Quit();
    }

    return 0;
}