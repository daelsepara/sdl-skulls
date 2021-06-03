// Standard IO
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Using SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// JSON library
#include "nlohmann/json.hpp"

#include "constants.hpp"
#include "config.hpp"
#include "controls.hpp"
#include "input.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "character.hpp"
#include "story.hpp"

#if defined(_WIN32)

#include <windows.h>
#include <shlobj.h>

#else

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#endif

// Forward declarations
bool aboutScreen(SDL_Window *window, SDL_Renderer *renderer);
bool characterScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
bool donateScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player);
bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, std::vector<Item::Type> &Items, Control::Type mode, int limit);
bool glossaryScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Skill::Base> Skills);
bool loseSkills(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int limit);
bool mainScreen(SDL_Window *window, SDL_Renderer *renderer);
bool mapScreen(SDL_Window *window, SDL_Renderer *renderer);
bool processStory(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int id);
bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Type> items, int TakeLimit);
bool tradeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Item::Type mine, Item::Type theirs);

int eatScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Type> items, int limit);
int giftScreen(SDL_Window *window, SDL_Renderer *renderer, Story::Base *story, Character::Base &player, std::vector<std::pair<Item::Type, int>> gifts, int default_destination);

Character::Base customCharacter(SDL_Window *window, SDL_Renderer *renderer);
Character::Base selectCharacter(SDL_Window *window, SDL_Renderer *renderer);
Control::Type gameScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, bool save_botton);

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);
Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story);

void renderAdventurer(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, Character::Base &player);

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
        SDL_DisplayMode mode;

        SDL_GetCurrentDisplayMode(0, &mode);

        SCREEN_WIDTH = (mode.w) * 0.8;
        SCREEN_HEIGHT = (mode.h) * 0.8;

        Recompute();

        // Create window and renderer
        SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_RENDERER_ACCELERATED, window, renderer);

        SDL_SetRenderDrawBlendMode(*renderer, SDL_BLENDMODE_NONE);

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

        SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
        SDL_RenderClear(*renderer);
        SDL_RenderPresent(*renderer);

        SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
        SDL_RenderClear(*renderer);
        SDL_RenderPresent(*renderer);
    }
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

int fitImage(SDL_Renderer *renderer, SDL_Surface *image, int x, int y, int w, int h)
{
    int splash_h = image->h;
    int splash_w = w;

    if (image && renderer)
    {
        SDL_Rect position;

        if (w != image->w)
        {
            splash_h = (int)((double)w / image->w * image->h);
            splash_w = w;

            if (splash_h > h)
            {
                splash_h = h;
                splash_w = (int)((double)h / image->h * image->w);
            }
        }

        position.w = splash_w;
        position.h = splash_h;
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

    return splash_h;
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

// create text image with line wrap limit
SDL_Surface *createText(const char *text, const char *ttf, int font_size, SDL_Color textColor, int wrap, int style = TTF_STYLE_NORMAL)
{
    SDL_Surface *surface = NULL;

    TTF_Init();

    auto font = TTF_OpenFont(ttf, font_size);

    if (font)
    {
        TTF_SetFontStyle(font, style);

        surface = TTF_RenderText_Blended_Wrapped(font, text, textColor, wrap);

        TTF_CloseFont(font);

        font = NULL;
    }

    TTF_Quit();

    return surface;
}

void fillRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderFillRect(renderer, &rect);
}

void drawRect(SDL_Renderer *renderer, int w, int h, int x, int y, int color)
{
    SDL_Rect rect;

    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderDrawRect(renderer, &rect);
}

void putText(SDL_Renderer *renderer, const char *text, TTF_Font *font, int space, SDL_Color fg, Uint32 bg, int style, int w, int h, int x, int y)
{
    if (renderer)
    {
        TTF_SetFontStyle(font, style);

        auto surface = TTF_RenderText_Blended_Wrapped(font, text, fg, w - 2 * space);

        if (surface)
        {
            fillRect(renderer, w, h, x, y, bg);

            renderText(renderer, surface, bg, x + space, y + space, h - 2 * space, 0);

            SDL_FreeSurface(surface);

            surface = NULL;
        }
    }
}

void fillWindow(SDL_Renderer *renderer, Uint32 color)
{
    SDL_SetRenderDrawColor(renderer, R(color), G(color), B(color), A(color));
    SDL_RenderClear(renderer);
}

void renderTextButtons(SDL_Renderer *renderer, std::vector<TextButton> controls, const char *ttf, int selected, SDL_Color fg, Uint32 bg, Uint32 bgSelected, int fontsize, int style = TTF_STYLE_NORMAL)
{
    if (controls.size() > 0)
    {
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

            if (i == selected)
            {
                SDL_SetRenderDrawColor(renderer, R(bgSelected), G(bgSelected), B(bgSelected), A(bgSelected));
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, R(bg), G(bg), B(bg), A(bg));
            }

            SDL_RenderFillRect(renderer, &rect);

            renderText(renderer, text, bg, x, y, 2 * fontsize, 0);

            SDL_FreeSurface(text);

            text = NULL;
        }
    }
}

void renderButtons(SDL_Renderer *renderer, std::vector<Button> controls, int current, int fg, int space, int pts)
{
    if (controls.size() > 0)
    {
        for (auto i = 0; i < controls.size(); i++)
        {
            SDL_Rect rect;

            for (auto size = pts; size >= 0; size--)
            {
                rect.w = controls[i].W + 2 * (space - size);
                rect.h = controls[i].H + 2 * (space - size);
                rect.x = controls[i].X - space + size;
                rect.y = controls[i].Y - space + size;

                if (i == current)
                {
                    SDL_SetRenderDrawColor(renderer, R(fg), G(fg), B(fg), A(fg));
                    SDL_RenderDrawRect(renderer, &rect);
                }
            }

            renderImage(renderer, controls[i].Surface, controls[i].X, controls[i].Y);
        }
    }
}

std::vector<TextButton> createHTextButtons(const char **choices, int num, int text_buttonh, int text_x, int text_y)
{
    auto controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin2 = (2.0 * Margin);
        auto marginleft = (1.0 - margin2);

        auto pixels = (int)(SCREEN_WIDTH * Margin);
        auto width = (int)(SCREEN_WIDTH * marginleft);

        auto text_spacew = width / num;
        auto text_buttonw = text_spacew - pixels;
        auto text_space = pixels / 2;

        for (auto i = 0; i < num; i++)
        {
            int left = i > 0 ? i - 1 : i;
            int right = i < num - 1 ? i + 1 : i;
            int up = i;
            int down = i;

            auto x = text_x + i * (text_buttonw + text_space * 2) + text_space;

            auto button = TextButton(i, choices[i], left, right, up, down, x, text_y, text_buttonw, text_buttonh);

            controls.push_back(button);
        }
    }

    return controls;
}

bool aboutScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto done = false;

    auto *about = "Critical IF are gamebooks with a difference. The outcomes are not random. Whether you live or die is a matter not of luck, but of judgement.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills; these will decide which options are available to you. Also note your Life Points and your possessions.\n\nLife Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during your adventure, but you can never have more Life Points than you started with.\n\nYou can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventrue. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.\n\nThat's all you need to know. Now choose your character.";

    auto splash = createImage("images/skulls-vr.png");

    auto text = createText(about, FONT_FILE, 18, clrWH, SCREEN_WIDTH * (1.0 - 3 * Margin) - splashw);

    // Render the image
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, "About the game");

        auto font_size = 20;
        auto selected = false;
        auto current = -1;
        auto about_buttonw = 150;
        auto about_buttonh = 48;
        auto about_buttony = (int)(SCREEN_HEIGHT * (1 - Margin) - buttonh);

        std::vector<TextButton> controls = {TextButton(0, "Back", 0, 0, 0, 0, startx, about_buttony, about_buttonw, about_buttonh, Control::Type::BACK)};

        while (!done)
        {
            // Fill the surface with background color
            fillWindow(renderer, intDB);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            renderText(renderer, text, intDB, startx * 2 + splashw, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, font_size, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return done;
}

bool mapScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto done = false;

    auto splash = createImage("images/map-one-world.png");

    // Render the image
    if (window && renderer && splash)
    {
        SDL_SetWindowTitle(window, "Map: One World");

        auto selected = false;
        auto current = -1;

        auto marginw = (1.0 - 2.0 * Margin) * SCREEN_WIDTH;

        std::vector<Button> controls = {Button(0, "icons/back-button.png", 0, 0, 0, 0, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK)};

        int offset_x = (marginw - (double)text_bounds / splash->h * splash->w) / 2;

        while (!done)
        {
            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx + offset_x, starty, marginw, text_bounds);

            renderButtons(renderer, controls, current, intGR, 8, 4);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
        }

        SDL_FreeSurface(splash);

        splash = NULL;
    }

    return done;
}

void renderAdventurer(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, Character::Base &player)
{
    const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
    const int profileh = 0.12 * SCREEN_HEIGHT;

    auto headerw = 0.8 * splashw;
    auto headerh = 0.07 * SCREEN_HEIGHT;
    auto text_space = 8;

    auto marginw = Margin * SCREEN_WIDTH;
    auto marginh = Margin * SCREEN_HEIGHT / 2;

    std::string skills;

    for (auto i = 0; i < player.Skills.size(); i++)
    {
        if (i > 0)
        {
            skills += ", ";
        }

        skills += player.Skills[i].Name;
    }

    auto boxw = (profilew - marginw) / 2;
    auto boxh = headerh;

    std::string possessions;

    for (auto i = 0; i < player.Items.size(); i++)
    {
        if (i > 0)
        {
            possessions += ", ";
        }

        possessions += Item::Descriptions.at(player.Items[i]);
    }

    // Fill the surface with background color
    fillWindow(renderer, intWH);

    auto name_string = player.Name;

    if (player.IsImmortal || player.IsBlessed)
    {
        name_string += " (";

        if (player.IsImmortal)
        {
            name_string += "Immortal";

            if (player.IsBlessed)
            {
                name_string += ", ";
            }
        }

        if (player.IsBlessed)
        {
            name_string += "Blessed";
        }

        name_string += ")";
    }

    putText(renderer, name_string.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, (player.IsImmortal ? headerw * 2 : headerw), headerh, startx, starty);
    putText(renderer, player.Description.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + headerh);

    putText(renderer, "Skills", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + headerh + marginh);
    putText(renderer, skills.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, boxh, startx, starty + profileh + 2 * headerh + marginh);

    putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, std::to_string(player.Life).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx, starty + profileh + 3 * headerh + 2 * marginh + boxh);

    putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx + boxw + marginw, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, (std::to_string(player.Money) + " cacao").c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx + boxw + marginw, starty + profileh + 3 * headerh + 2 * marginh + boxh);

    putText(renderer, "Possessions", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 3 * headerh + 3 * marginh + 2 * boxh);
    putText(renderer, player.Items.size() > 0 ? possessions.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + profileh + 4 * headerh + 3 * marginh + 2 * boxh);
}

SDL_Surface *createHeaderButton(SDL_Window *window, const char *text, SDL_Color color, Uint32 bg, int w, int h, int x)
{
    auto button = SDL_CreateRGBSurface(0, w, h, arrow_size, 0, 0, 0, 0);
    auto text_surface = createText(text, FONT_FILE, 18, color, w, TTF_STYLE_NORMAL);

    if (button && text_surface)
    {
        SDL_Rect src;

        src.w = text_surface->w;
        src.h = text_surface->h;
        src.x = 0;
        src.y = 0;

        SDL_Rect dst;

        dst.w = button->w;
        dst.h = button->h;
        dst.x = 0;
        dst.y = 0;

        SDL_FillRect(button, &dst, bg);

        dst.x = x < 0 ? (button->w - text_surface->w) / 2 : x;
        dst.y = (button->h - text_surface->h) / 2;

        SDL_BlitSurface(text_surface, &src, button, &dst);
    }

    if (text_surface)
    {
        SDL_FreeSurface(text_surface);

        text_surface = NULL;
    }

    return button;
}

bool glossaryScreen(SDL_Window *window, SDL_Renderer *renderer, std::vector<Skill::Base> Skills)
{
    std::string title = "Necklace of Skulls: Skills Glossary";

    if (window && renderer)
    {
        auto text_space = 8;
        auto font_size = 20;

        const int glossary_width = SCREEN_WIDTH * (1.0 - 2.0 * Margin) - arrow_size - 2 * text_space;

        std::string text;

        for (auto i = 0; i < Skills.size(); i++)
        {
            if (i > 0)
            {
                text += "\n" + std::string(glossary_width / 10, '-') + "\n";
            }

            text += std::string(Skills[i].Name) + "\n\n";
            text += std::string(Skills[i].Description) + "\n";
        }

        auto glossary = createText(text.c_str(), FONT_FILE, font_size, clrBK, glossary_width - 2 * text_space, TTF_STYLE_NORMAL);

        auto quit = false;

        auto controls = std::vector<Button>();

        controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
        controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
        controls.push_back(Button(2, "icons/back-button.png", 1, 2, 1, 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        auto scrollSpeed = 20;
        auto hold = false;

        auto selected = false;
        auto current = -1;
        auto offset = 0;

        while (!quit)
        {
            SDL_SetWindowTitle(window, title.c_str());

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fillRect(renderer, glossary_width, text_bounds, startx, starty, intBE);
            renderText(renderer, glossary, intBE, startx + text_space, starty + text_space, text_bounds - 2 * text_space, offset);

            renderButtons(renderer, controls, current, intGR, border_space, border_pts);

            bool scrollUp = false;
            bool scrollDown = false;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (glossary->h >= text_bounds - 2 * text_space)
                    {
                        if (offset < glossary->h - text_bounds + 2 * text_space)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > glossary->h - text_bounds + 2 * text_space)
                        {
                            offset = glossary->h - text_bounds + 2 * text_space;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    quit = true;

                    break;
                }
            }
        }

        if (glossary)
        {
            SDL_FreeSurface(glossary);

            glossary = NULL;
        }
    }

    return false;
}

bool greenMirror(SDL_Window *window, SDL_Renderer *renderer, Character::Base player, Story::Base *story)
{
    std::string title = "Necklace of Skulls: GREEN MIRROR";

    if (window && renderer)
    {
        auto text_space = 8;
        auto font_size = 20;

        const int future_width = SCREEN_WIDTH * (1.0 - 2.0 * Margin) - arrow_size - 2 * text_space;

        std::string text;

        auto done = false;

        auto controls = std::vector<Button>();

        controls.push_back(Button(0, "icons/up-arrow.png", 0, 1, 0, 1, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));
        controls.push_back(Button(1, "icons/down-arrow.png", 0, 2, 0, 2, (1 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));
        controls.push_back(Button(2, "icons/back-button.png", 1, 2, 1, 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        auto scrollSpeed = 20;
        auto hold = false;

        auto selected = false;
        auto current = -1;
        auto offset = 0;

        std::string future_text = "You used the GREEN MIRROR to look into your future...\n\n";

        if (story->Choices.size() > 0)
        {
            for (auto i = 0; i < story->Choices.size(); i++)
            {
                if (i > 0)
                {
                    future_text += "\n" + std::string(future_width / 10, '-') + "\n\n";
                }

                auto simPlayer = player;

                future_text += "[" + std::string(story->Choices[i].Text) + "]\n\n";

                // simulate going into the future
                auto future_story = (Story::Base *)findStory(story->Choices[i].Destination);

                auto background = 0;

                while (background != -1)
                {
                    background = future_story->Background(simPlayer);

                    if (background >= 0)
                    {
                        future_story = (Story::Base *)findStory(background);
                    }
                }

                future_story->Event(simPlayer);

                future_text += future_story->Text;
            }
        }
        else
        {
            auto simPlayer = player;

            // simulate going into the future
            auto future_story = (Story::Base *)findStory(story->Continue(simPlayer));

            auto background = 0;

            while (background != -1)
            {
                background = future_story->Background(simPlayer);

                if (background >= 0)
                {
                    future_story = (Story::Base *)findStory(background);
                }
            }

            future_story->Event(simPlayer);

            future_text += "... " + std::string(future_story->Text);
        }

        auto future = createText(future_text.c_str(), FONT_FILE, font_size, clrBK, future_width - 2 * text_space, TTF_STYLE_NORMAL);

        while (!done)
        {
            SDL_SetWindowTitle(window, title.c_str());

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fillRect(renderer, future_width, text_bounds, startx, starty, intBE);

            renderText(renderer, future, intBE, startx + text_space, starty + text_space, text_bounds - 2 * text_space, offset);

            renderButtons(renderer, controls, current, intGR, border_space, border_pts);

            bool scrollUp = false;
            bool scrollDown = false;

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
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
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (future->h >= text_bounds - 2 * text_space)
                    {
                        if (offset < future->h - text_bounds + 2 * text_space)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > future->h - text_bounds + 2 * text_space)
                        {
                            offset = future->h - text_bounds + 2 * text_space;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
        }

        if (future)
        {
            SDL_FreeSurface(future);

            future = NULL;
        }
    }

    return false;
}

bool characterScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    std::string title = "Necklace of Skulls: Adventure Sheet";

    auto done = false;

    // Render the image
    if (window && renderer)
    {
        auto selected = false;
        auto current = -1;

        const int back_buttonh = 48;
        const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
        const int profileh = 0.12 * SCREEN_HEIGHT;

        auto controls = std::vector<Button>();

        auto marginw = Margin * SCREEN_WIDTH;
        auto marginh = Margin * SCREEN_HEIGHT / 2;

        auto headerw = 0.8 * splashw;
        auto headerh = 0.07 * SCREEN_HEIGHT;
        auto text_space = 8;
        auto font_size = 18;
        auto boxh = headerh;

        auto skills = createHeaderButton(window, "Skills", clrWH, intDB, headerw, headerh, text_space);
        auto possessions = createHeaderButton(window, "Possessions", clrWH, intDB, headerw, headerh, text_space);

        controls.push_back(Button(0, skills, 0, 1, 0, 1, startx, starty + profileh + headerh + marginh, Control::Type::GLOSSARY));
        controls.push_back(Button(1, possessions, 0, 2, 0, 2, startx, starty + profileh + 3 * headerh + 3 * marginh + 2 * boxh, Control::Type::ACTION));
        controls.push_back(Button(2, "icons/back-button.png", 1, 2, 1, 2, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        std::string codewords;

        for (auto i = 0; i < player.Codewords.size(); i++)
        {
            if (i > 0)
            {
                codewords += ", ";
            }

            codewords += Codeword::Descriptions.at(player.Codewords[i]);
        }

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        if (font)
        {
            while (!done)
            {
                SDL_SetWindowTitle(window, title.c_str());

                // Fill the surface with background color
                fillWindow(renderer, intWH);

                renderAdventurer(window, renderer, font, player);

                if (player.Codewords.size() > 0)
                {
                    putText(renderer, "Codewords", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + 2 * profileh + 4 * headerh + 4 * marginh + 2 * boxh);
                    putText(renderer, codewords.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_ITALIC, profilew - buttonw - 2 * text_space, boxh, startx, starty + 2 * profileh + 5 * headerh + 4 * marginh + 2 * boxh);
                }

                renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

                bool scrollUp = false;
                bool scrollDown = false;
                bool hold = false;

                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::BACK)
                    {
                        break;
                    }
                    else if (controls[current].Type == Control::Type::GLOSSARY)
                    {
                        glossaryScreen(window, renderer, player.Skills);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::ACTION)
                    {
                        inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                        current = -1;

                        selected = false;
                    }
                }
            }
        }

        if (skills)
        {
            SDL_FreeSurface(skills);

            skills = NULL;
        }

        if (possessions)
        {
            SDL_FreeSurface(possessions);

            possessions = NULL;
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

std::vector<Button> skillsList(SDL_Window *window, SDL_Renderer *renderer, int start, int last, int limit)
{
    auto font_size = 20;
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    for (int i = 0; i < last - start; i++)
    {
        auto index = start + i;

        std::string item_string = Skill::ALL[index].Name;

        auto text = createText(item_string.c_str(), FONT_FILE, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

        auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

        controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[i].W = textwidth - 4 * text_space;

        controls[i].H = text->h;
    }

    auto idx = controls.size();

    if (Skill::ALL.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", 0, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (Skill::ALL.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", 0, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, (texty + 2 * text_bounds / 3 - arrow_size - border_space), Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    idx = controls.size();

    auto button_width = 100;
    auto button_space = 25;
    auto button_height = 48;

    controls.push_back(Button(idx, createHeaderButton(window, "Glossary", clrWH, intBK, button_width, button_height, -1), idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::GLOSSARY));
    controls.push_back(Button(idx + 1, createHeaderButton(window, "Start", clrWH, intBK, button_width, button_height, -1), idx, idx + 2, idx - 1, idx + 1, startx + button_width + button_space, buttony, Control::Type::NEW));
    controls.push_back(Button(idx + 2, createHeaderButton(window, "Back", clrWH, intBK, button_width, button_height, -1), idx + 1, idx + 2, idx - 1, idx + 2, startx + 2 * (button_width + button_space), buttony, Control::Type::BACK));

    return controls;
}

Character::Base customCharacter(SDL_Window *window, SDL_Renderer *renderer)
{
    std::string title = "Down Among the Dead Men: Create Character";

    auto done = false;

    Character::Base player = Character::Base();

    // Render the image
    if (window && renderer)
    {
        auto flash_message = false;
        auto flash_color = intRD;

        std::string message = "";

        Uint32 start_ticks = 0;
        Uint32 duration = 5000;

        auto selected = false;
        auto current = -1;
        auto character = 0;
        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);
        auto Limit = (2 * text_bounds / 3 - text_space) / (font_size + 7 * text_space / 2);

        auto offset = 0;

        auto last = offset + Limit;

        if (last > Skill::ALL.size())
        {
            last = Skill::ALL.size();
        }

        auto controls = skillsList(window, renderer, offset, last, Limit);

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        bool scrollUp = false;
        bool scrollDown = false;
        bool hold = false;
        auto scrollSpeed = 1;

        auto selection = std::vector<int>();

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.150 * SCREEN_HEIGHT;
        auto box_space = 10;

        while (!done)
        {
            last = offset + Limit;

            if (last > Skill::ALL.size())
            {
                last = Skill::ALL.size();
            }

            SDL_SetWindowTitle(window, title.c_str());

            fillWindow(renderer, intWH);

            std::string selection_string = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        selection_string += ", ";
                    }

                    std::string description = Skill::ALL[selection[i]].Name;

                    selection_string += description;
                }
            }

            putText(renderer, "SELECTED", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            putText(renderer, selection.size() > 0 ? selection_string.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);

            fillRect(renderer, textwidth, 2 * text_bounds / 3, textx, texty, intBE);

            fillRect(renderer, textwidth, text_bounds / 3 - texty, textx, 2 * (texty + text_bounds / 3), intDB);

            if (current >= 0 && current < Limit)
            {
                auto text = createText(Skill::ALL[current + offset].Description, FONT_FILE, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);

                renderText(renderer, text, intDB, textx + text_space, 2 * (texty + text_bounds / 3) + text_space, text_bounds / 3 - texty, 0);

                SDL_FreeSurface(text);

                text = NULL;
            }

            renderButtons(renderer, controls, current, intGR, 8, 4);

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (Skill::FIND_LIST(selection, offset + i) >= 0)
                    {
                        drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                    }
                }
            }

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (!flash_message)
            {
                putText(renderer, "SELECT 4 Skills for your character.", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + Limit;

                        if (last > Skill::ALL.size())
                        {
                            last = Skill::ALL.size();
                        }

                        controls.clear();

                        controls = skillsList(window, renderer, offset, last, Limit);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (Skill::ALL.size() - last > 0)
                    {
                        if (offset < Skill::ALL.size() - Limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > Skill::ALL.size() - Limit)
                        {
                            offset = Skill::ALL.size() - Limit;
                        }

                        last = offset + Limit;

                        if (last > Skill::ALL.size())
                        {
                            last = Skill::ALL.size();
                        }

                        controls.clear();

                        controls = skillsList(window, renderer, offset, last, Limit);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (Skill::ALL.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if ((current + offset >= 0) && (current + offset) < Skill::ALL.size())
                    {
                        auto result = Skill::FIND_LIST(selection, current + offset);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (selection.size() < player.SKILLS_LIMIT)
                            {
                                selection.push_back(offset + current);
                            }
                        }
                    }

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::NEW)
                {
                    if (selection.size() == player.SKILLS_LIMIT)
                    {
                        Character::CUSTOM.Skills.clear();
                        Character::CUSTOM.Items.clear();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            Character::CUSTOM.Skills.push_back(Skill::ALL[selection[i]]);

                            if (Skill::ALL[selection[i]].Type == Skill::Type::TARGETING)
                            {
                                Character::CUSTOM.Items.push_back(Item::Type::BLOWGUN);
                            }
                            else if (Skill::ALL[selection[i]].Type == Skill::Type::SWORDPLAY)
                            {
                                Character::CUSTOM.Items.push_back(Item::Type::SWORD);
                            }
                            else if (Skill::ALL[selection[i]].Type == Skill::Type::CHARMS)
                            {
                                Character::CUSTOM.Items.push_back(Item::Type::MAGIC_AMULET);
                            }
                            else if (Skill::ALL[selection[i]].Type == Skill::Type::SPELLS)
                            {
                                Character::CUSTOM.Items.push_back(Item::Type::MAGIC_WAND);
                            }
                        }

                        Character::CUSTOM.Money = 10;
                        Character::CUSTOM.Life = 10;

                        player = Character::CUSTOM;

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }
                    else
                    {
                        flash_message = true;

                        flash_color = intRD;

                        start_ticks = SDL_GetTicks();

                        message = "Please select " + std::to_string(player.SKILLS_LIMIT) + " skills.";

                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::GLOSSARY)
                {
                    glossaryScreen(window, renderer, Skill::ALL);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK)
                {
                    player = Character::Base();

                    current = -1;

                    selected = false;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return player;
}

Character::Base selectCharacter(SDL_Window *window, SDL_Renderer *renderer)
{
    std::string title = "Necklace of Skulls: Select Character";

    auto done = false;

    Character::Base player = Character::WARRIOR;

    // Render the image
    if (window && renderer)
    {
        auto selected = false;
        auto current = -1;
        auto character = 0;
        auto main_buttonh = 48;
        auto font_size = 18;
        auto font20 = 20;

        const char *choices[6] = {"Previous", "Next", "Glossary", "Custom", "Start", "Back"};

        auto controls = createHTextButtons(choices, 6, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::BACK;
        controls[1].Type = Control::Type::NEXT;
        controls[2].Type = Control::Type::GLOSSARY;
        controls[3].Type = Control::Type::CUSTOM;
        controls[4].Type = Control::Type::NEW;
        controls[5].Type = Control::Type::QUIT;

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        if (font)
        {
            while (!done)
            {
                SDL_SetWindowTitle(window, title.c_str());

                renderAdventurer(window, renderer, font, Character::Classes[character]);

                renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, font20, TTF_STYLE_NORMAL);

                bool scrollUp = false;
                bool scrollDown = false;
                bool hold = false;

                Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size())
                {
                    if (controls[current].Type == Control::Type::NEW)
                    {
                        player = Character::Classes[character];

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }
                    else if (controls[current].Type == Control::Type::BACK)
                    {
                        if (character > 0)
                        {
                            character--;
                        }
                    }
                    else if (controls[current].Type == Control::Type::NEXT)
                    {
                        if (character < Character::Classes.size() - 1)
                        {
                            character++;
                        }
                    }
                    else if (controls[current].Type == Control::Type::GLOSSARY)
                    {
                        glossaryScreen(window, renderer, Skill::ALL);

                        current = -1;
                    }
                    else if (controls[current].Type == Control::Type::CUSTOM)
                    {
                        player = customCharacter(window, renderer);

                        if (player.Skills.size() == player.SKILLS_LIMIT)
                        {
                            current = -1;

                            selected = false;

                            done = true;

                            break;
                        }
                        else
                        {
                            current = -1;
                        }
                    }
                    else if (controls[current].Type == Control::Type::QUIT)
                    {
                        player = Character::Base();

                        player.StoryID = -1;

                        current = -1;

                        selected = false;

                        done = true;

                        break;
                    }

                    selected = false;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return player;
}

std::vector<Button> createItemList(SDL_Window *window, SDL_Renderer *renderer, std::vector<Item::Type> list, int start, int last, int limit, bool confirm_button)
{
    auto font_size = 20;
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

    auto controls = std::vector<Button>();

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            auto index = start + i;

            std::string item_string = Item::Descriptions[list[index]];

            auto text = createText(item_string.c_str(), FONT_FILE, font_size, clrBK, textwidth - 4 * text_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth - 4 * text_space;

            controls[i].H = text->h;
        }
    }

    auto idx = controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    if (confirm_button)
    {
        idx = controls.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

std::vector<Button> createItemControls(std::vector<Item::Type> Items)
{
    auto text_space = 8;
    auto font_size = 16;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

    auto controls = std::vector<Button>();

    controls.clear();

    for (auto idx = 0; idx < Items.size(); idx++)
    {
        auto text = createText(Item::Descriptions[Items[idx]], FONT_FILE, font_size, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

        auto y = (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H + 3 * text_space : texty + 2 * text_space);

        controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < Items.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[idx].W = textwidth + button_space;

        controls[idx].H = text->h;
    }

    auto idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, idx - 1, idx, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story, std::vector<Item::Type> &Items, Control::Type mode, int limit)
{
    if (Items.size() > 0)
    {
        auto text_space = 8;
        auto font_size = 20;
        auto scrollSpeed = 1;
        auto display_limit = ((text_bounds - text_space) / (font_size + 7 * text_space / 2));

        auto offset = 0;

        auto last = offset + display_limit;

        if (last > Items.size())
        {
            last = Items.size();
        }

        const char *message = NULL;

        std::string temp_message = "";

        auto flash_message = false;
        auto flash_color = intRD;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto done = false;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

        auto controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.150 * SCREEN_HEIGHT;
        auto box_space = 10;
        auto messageh = 0.25 * SCREEN_HEIGHT;

        auto mirror_text = createText("The GREEN MIRROR disappears after one use. Do you wish to continue?", FONT_FILE, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        auto message_x = (SCREEN_WIDTH - textwidth) / 2;
        auto message_y = (SCREEN_HEIGHT - messageh) / 2;

        auto message_controls = std::vector<Button>();
        message_controls.push_back(Button(0, "icons/yes.png", 0, 1, 0, 0, message_x + button_space, message_y + messageh - buttonh - button_space, Control::Type::YES));
        message_controls.push_back(Button(1, "icons/no.png", 0, 1, 1, 1, message_x + textwidth - button_space - buttonw, message_y + messageh - buttonh - button_space, Control::Type::NO));

        auto trigger_mirror = false;

        while (!done)
        {
            last = offset + display_limit;

            if (last > Items.size())
            {
                last = Items.size();
            }

            SDL_SetWindowTitle(window, "Necklace of Skulls: Possessions");

            fillWindow(renderer, intWH);

            if (flash_message)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    flash_message = false;
                }
            }

            if (!flash_message)
            {
                if (mode == Control::Type::DROP)
                {
                    putText(renderer, "You are carrying too many items. Select item(s) to DROP.", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::USE)
                {
                    putText(renderer, "Select an item to USE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else if (mode == Control::Type::STEAL)
                {
                    std::string stolen_message = "You were robbed. DROP item(s) until only " + std::to_string(limit) + " item" + std::string(limit > 1 ? "s" : "") + " remains.";

                    putText(renderer, stolen_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
                else
                {
                    putText(renderer, "You are carrying these items", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
                }
            }

            putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            for (auto idx = offset; idx < last; idx++)
            {
                auto i = idx - offset;

                drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
            }

            renderButtons(renderer, controls, trigger_mirror ? -1 : current, intGR, text_space, text_space / 2);

            if (!trigger_mirror)
            {
                done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);
            }
            else
            {
                fillRect(renderer, textwidth, messageh, message_x, message_y, intLB);

                renderImage(renderer, mirror_text, (SCREEN_WIDTH - mirror_text->w) / 2, message_y + text_space);

                renderButtons(renderer, message_controls, current, intWH, border_space, border_pts);

                done = Input::GetInput(renderer, message_controls, current, selected, scrollUp, scrollDown, hold);
            }

            if (selected && !trigger_mirror)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                        SDL_Delay(50);
                    }

                    if (offset <= 0)
                    {
                        current = -1;

                        selected = false;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (Items.size() - last > 0)
                    {
                        if (offset < Items.size() - display_limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > Items.size() - display_limit)
                        {
                            offset = Items.size() - display_limit;
                        }

                        last = offset + display_limit;

                        if (last > Items.size())
                        {
                            last = Items.size();
                        }

                        controls.clear();

                        controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                        SDL_Delay(50);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (Items.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if ((current + offset >= 0) && (current + offset) < Items.size())
                    {
                        auto item = Items[current + offset];

                        if (mode == Control::Type::DROP)
                        {
                            Items.erase(Items.begin() + (current + offset));

                            if (offset > 0)
                            {
                                offset--;
                            }

                            last = offset + display_limit;

                            if (last > Items.size())
                            {
                                last = Items.size();
                            }

                            controls.clear();

                            controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                            temp_message = std::string(Item::Descriptions[item]) + " DROPPED!";

                            message = temp_message.c_str();

                            flash_color = intRD;

                            start_ticks = SDL_GetTicks();

                            flash_message = true;
                        }
                        else if (mode == Control::Type::STEAL)
                        {
                            if (Items.size() > limit)
                            {
                                Items.erase(Items.begin() + (current + offset));

                                Character::LOSE_ITEMS(player, {item});

                                if (offset > 0)
                                {
                                    offset--;
                                }

                                last = offset + display_limit;

                                if (last > Items.size())
                                {
                                    last = Items.size();
                                }

                                controls.clear();
                                controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                                temp_message = std::string(Item::Descriptions[item]) + " STOLEN!";

                                message = temp_message.c_str();

                                flash_color = intRD;

                                start_ticks = SDL_GetTicks();

                                flash_message = true;
                            }
                        }
                        else if (mode == Control::Type::USE)
                        {
                            if (item == Item::Type::MAGIC_DRINK)
                            {
                                if (player.Life < player.MAX_LIFE_LIMIT)
                                {
                                    Character::GAIN_LIFE(player, 5);

                                    Items.erase(Items.begin() + (current + offset));

                                    if (offset > 0)
                                    {
                                        offset--;
                                    }

                                    last = offset + display_limit;

                                    if (last > Items.size())
                                    {
                                        last = Items.size();
                                    }

                                    controls.clear();

                                    controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                                    message = "You RECOVER 5 Life Points.";

                                    flash_color = intLB;
                                }
                                else
                                {
                                    message = "You are not INJURED!";

                                    flash_color = intRD;
                                }

                                start_ticks = SDL_GetTicks();

                                flash_message = true;
                            }
                            else if (item == Item::Type::PAPAYA || item == Item::Type::MAIZE_CAKES)
                            {
                                if (player.Life < player.MAX_LIFE_LIMIT)
                                {
                                    Character::GAIN_LIFE(player, 1);

                                    Items.erase(Items.begin() + (current + offset));

                                    if (offset > 0)
                                    {
                                        offset--;
                                    }

                                    last = offset + display_limit;

                                    if (last > Items.size())
                                    {
                                        last = Items.size();
                                    }

                                    controls.clear();

                                    controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                                    message = "You RECOVER 1 Life Point.";

                                    flash_color = intLB;
                                }
                                else
                                {
                                    message = "You are not INJURED!";

                                    flash_color = intRD;
                                }

                                start_ticks = SDL_GetTicks();

                                flash_message = true;
                            }
                            else if (item == Item::Type::GREEN_MIRROR)
                            {
                                if (!trigger_mirror)
                                {
                                    trigger_mirror = true;
                                }
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
            else if (selected && trigger_mirror)
            {
                if (message_controls[current].Type == Control::Type::YES && !hold)
                {
                    greenMirror(window, renderer, player, story);

                    trigger_mirror = false;

                    Item::REMOVE(Items, Item::Type::GREEN_MIRROR);

                    if (offset > 0)
                    {
                        offset--;
                    }

                    last = offset + display_limit;

                    if (last > Items.size())
                    {
                        last = Items.size();
                    }

                    controls.clear();

                    controls = createItemList(window, renderer, Items, offset, last, display_limit, false);

                    message = "The GREEN MIRROR vanishes without a trace!";

                    flash_color = intLB;

                    start_ticks = SDL_GetTicks();

                    flash_message = true;
                }
                else if (message_controls[current].Type == Control::Type::NO && !hold)
                {
                    current = -1;

                    selected = false;

                    trigger_mirror = false;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

int giftScreen(SDL_Window *window, SDL_Renderer *renderer, Story::Base *story, Character::Base &player, std::vector<std::pair<Item::Type, int>> gifts, int default_destination)
{
    int storyID = default_destination;

    if (player.Items.size() > 0)
    {
        const char *message = NULL;

        auto error = false;
        auto done = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createItemControls(player.Items);

        auto idx = player.Items.size();

        controls.erase(controls.begin() + idx);
        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::GIVE));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selected_item = -1;

        while (!done)
        {
            SDL_SetWindowTitle(window, "Necklace of Skulls: Possessions");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                putText(renderer, "Select an item to GIVE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            putText(renderer, "Selected", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));

            if (selected_item >= 0 && selected_item < player.Items.size())
            {
                putText(renderer, (std::string(Item::Descriptions[player.Items[selected_item]])).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);
            }
            else
            {
                fillRect(renderer, splashw, boxh, startx, starty + text_bounds - boxh, intBE);
            }

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < player.Items.size(); i++)
            {
                if (selected_item == i)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < player.Items.size())
                    {
                        if (selected_item == current)
                        {
                            selected_item = -1;
                        }
                        else
                        {
                            selected_item = current;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::GIVE && !hold)
                {
                    if (selected_item >= 0 && selected_item < player.Items.size())
                    {
                        auto item = player.Items[selected_item];

                        Character::LOSE_ITEMS(player, {item});

                        for (auto i = 0; i < gifts.size(); i++)
                        {
                            if (gifts[i].first == item)
                            {
                                storyID = gifts[i].second;

                                break;
                            }
                        }

                        done = true;

                        break;
                    }
                    else
                    {
                        start_ticks = SDL_GetTicks();

                        message = "You have not selected anything!";

                        error = true;
                    }
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    storyID = story->ID;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return storyID;
}

bool takeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Type> items, int TakeLimit)
{
    auto done = false;

    if (TakeLimit > 0)
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createItemControls(items);

        auto idx = items.size();

        controls.erase(controls.begin() + idx);

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selection = std::vector<int>();

        while (!done)
        {
            SDL_SetWindowTitle(window, "Necklace of Skulls");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string take_message = "";

                if (items.size() > 1)
                {
                    if (TakeLimit > 1)
                    {
                        if (items.size() == TakeLimit)
                        {
                            take_message = "You can TAKE any number of items.";
                        }
                        else
                        {
                            take_message = "You can TAKE up to " + std::to_string(TakeLimit) + " items.";
                        }
                    }
                    else
                    {
                        take_message = "Choose an item to KEEP.";
                    }
                }
                else
                {
                    take_message = "KEEP this item?";
                }

                putText(renderer, take_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string take = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        take += ", ";
                    }

                    take += std::string(Item::Descriptions[items[selection[i]]]);
                }
            }

            putText(renderer, "SELECTED", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (3 * boxh + infoh));
            putText(renderer, selection.size() > 0 ? take.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 3 * boxh, startx, starty + text_bounds - 3 * boxh);

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < items.size(); i++)
            {
                if (std::find(selection.begin(), selection.end(), i) != selection.end())
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < items.size())
                    {
                        auto result = Item::FIND(selection, current);

                        if (result >= 0)
                        {
                            selection.erase(selection.begin() + result);
                        }
                        else
                        {
                            if (selection.size() < TakeLimit)
                            {
                                selection.push_back(current);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    auto take = std::vector<Item::Type>();

                    for (auto i = 0; i < selection.size(); i++)
                    {
                        take.push_back(items[i]);
                    }

                    Character::GET_ITEMS(player, take);

                    current = -1;

                    selected = false;

                    done = true;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

std::vector<Button> createSkillControls(std::vector<Skill::Base> Skills)
{
    auto font_size = 20;
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

    auto controls = std::vector<Button>();

    controls.clear();

    for (auto idx = 0; idx < Skills.size(); idx++)
    {
        auto text = createText(Skills[idx].Name, FONT_FILE, font_size, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

        auto y = (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H + 3 * text_space : texty + 2 * text_space);

        controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < Skills.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[idx].W = textwidth + button_space;

        controls[idx].H = text->h;
    }

    return controls;
}

bool loseSkills(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int limit)
{
    auto done = false;

    if (player.Skills.size() > limit)
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createSkillControls(player.Skills);

        auto idx = player.Skills.size();

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selection = std::vector<Skill::Base>();

        while (!done)
        {
            SDL_SetWindowTitle(window, "Necklace of Skulls");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string lose_message = "Select " + std::string((player.SKILLS_LIMIT - limit) > 1 ? std::string(std::to_string(player.SKILLS_LIMIT - limit) + " skills") : "a skill") + " to LOSE.";

                putText(renderer, lose_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string to_lose = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        to_lose += ", ";
                    }

                    to_lose += std::string(selection[i].Name);
                }
            }

            putText(renderer, "SKILLS", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, selection.size() > 0 ? to_lose.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < player.Skills.size(); i++)
            {
                if (Skill::VERIFY(selection, player.Skills[i]))
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < player.Skills.size())
                    {
                        if (Skill::VERIFY(selection, player.Skills[current]))
                        {
                            Skill::REMOVE(selection, player.Skills[current]);
                        }
                        else
                        {
                            if (selection.size() < (player.SKILLS_LIMIT - limit))
                            {
                                Skill::ADD(selection, player.Skills[current]);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    if (selection.size() > 0 && ((player.SKILLS_LIMIT - selection.size()) <= limit))
                    {
                        auto skills = std::vector<Skill::Type>();

                        for (auto i = 0; i < selection.size(); i++)
                        {
                            skills.push_back(selection[i].Type);
                        }

                        Character::LOSE_SKILLS(player, skills);

                        done = true;

                        break;
                    }
                    else
                    {
                        message = "Please complete your selection";

                        start_ticks = SDL_GetTicks();

                        error = true;
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

int eatScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, std::vector<Item::Type> items, int limit)
{
    auto consumed = 0;
    auto done = false;

    auto filtered_items = std::vector<Item::Type>();

    for (auto i = 0; i < player.Items.size(); i++)
    {
        if (Item::VERIFY(items, player.Items[i]))
        {
            filtered_items.push_back(player.Items[i]);
        }
    }

    if (player.Items.size() > 0 && filtered_items.size() > 0)
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createItemControls(filtered_items);

        auto idx = filtered_items.size();

        controls.erase(controls.begin() + idx);

        controls.push_back(Button(idx, "icons/yes.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::CONFIRM));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        auto selection = std::vector<int>();

        while (!done)
        {
            SDL_SetWindowTitle(window, "Necklace of Skulls");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                std::string eat_message = "Select " + std::string((limit > 1) ? "provisions" : "provision") + " to EAT." + (limit > 1 ? (" You can EAT up to " + std::to_string(limit) + " provisions.") : "");

                putText(renderer, eat_message.c_str(), font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            std::string eat = "";

            if (selection.size() > 0)
            {
                for (auto i = 0; i < selection.size(); i++)
                {
                    if (i > 0)
                    {
                        eat += ", ";
                    }

                    eat += std::string(Item::Descriptions[filtered_items[selection[i]]]);
                }
            }

            putText(renderer, "SELECTED", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, selection.size() > 0 ? eat.c_str() : "(None)", font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < filtered_items.size(); i++)
            {
                if (std::find(selection.begin(), selection.end(), i) != selection.end())
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < filtered_items.size())
                    {
                        if (std::find(selection.begin(), selection.end(), current) != selection.end())
                        {
                            selection.erase(std::find(selection.begin(), selection.end(), current));
                        }
                        else
                        {
                            if (selection.size() < limit)
                            {
                                selection.push_back(current);
                            }
                        }
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::CONFIRM && !hold)
                {
                    for (auto i = 0; i < selection.size(); i++)
                    {
                        Character::LOSE_ITEMS(player, {filtered_items[selection[i]]});
                    }

                    consumed = selection.size();

                    current = -1;

                    selected = false;

                    done = true;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    if (filtered_items.size() <= 0)
    {
        consumed = -1;
    }

    return consumed;
}

bool donateScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player)
{
    auto done = false;

    if (player.Money > 0)
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto box_space = 10;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = std::vector<Button>();

        auto idx = 0;

        auto button_plus = createHeaderButton(window, "+", clrWH, intDB, arrow_size, arrow_size, -1);
        auto button_minus = createHeaderButton(window, "-", clrWH, intDB, arrow_size, arrow_size, -1);
        auto button_size = 35;

        controls.push_back(Button(idx, button_plus, idx, idx + 1, idx, idx + 2, textx + 2 * text_space, texty + button_size + 2 * box_space, Control::Type::PLUS));
        controls.push_back(Button(idx + 1, button_minus, idx, idx + 1, idx, idx + 2, textx + 2 * text_space + button_space + arrow_size, texty + button_size + 2 * box_space, Control::Type::MINUS));
        controls.push_back(Button(idx + 2, "icons/yes.png", idx + 2, idx + 2, idx + 1, idx + 2, startx, buttony, Control::Type::ACTION));
        controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 3, idx + 3, idx, idx + 3, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto donation = 1;

        while (!done)
        {
            SDL_SetWindowTitle(window, "Make a Donation");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                putText(renderer, "Select amount to DONATE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            std::string donation_text = "DONATE " + (std::to_string(donation)) + " cacao";
            putText(renderer, donation_text.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, button_size, textx + text_space, texty + text_space);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    player.DONATION = donation;

                    player.Money -= donation;

                    done = true;

                    current = -1;

                    selected = false;

                    break;
                }
                else if (controls[current].Type == Control::Type::PLUS && !hold)
                {
                    if (donation < player.Money)
                    {
                        donation++;
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MINUS && !hold)
                {
                    if (donation > 0)
                    {
                        donation--;
                    }

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

bool tradeScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Item::Type mine, Item::Type theirs)
{
    auto done = false;

    if (Character::VERIFY_ITEMS(player, {mine}))
    {
        const char *message = NULL;

        auto error = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto font_size = 20;
        auto text_space = 8;
        auto box_space = 10;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = std::vector<Button>();

        auto idx = 0;

        controls.push_back(Button(idx, "icons/yes.png", idx, idx + 1, idx, idx, startx, buttony, Control::Type::ACTION));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;

        while (!done)
        {
            SDL_SetWindowTitle(window, "Make a Donation");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, boxh * 2, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                putText(renderer, "TRADE", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, boxh, startx, starty);
            }

            putText(renderer, "Life", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            std::string trade_text = "Trade " + std::string(Item::Descriptions[mine]) + " for " + std::string(Item::Descriptions[theirs]) + "?";
            putText(renderer, trade_text.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, textwidth, boxh, textx + text_space, texty + text_space);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    Character::LOSE_ITEMS(player, {mine});
                    Character::GET_ITEMS(player, {theirs});

                    done = true;

                    current = -1;

                    selected = false;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = false;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return done;
}

bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    if (story->Shop.size() > 0)
    {
        std::string message;

        auto error = false;
        auto purchased = false;
        auto done = false;

        Uint32 start_ticks = 0;
        Uint32 duration = 3000;

        auto controls = std::vector<Button>();

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto idx = 0;

        for (auto i = story->Shop.begin(); i != story->Shop.end(); i++)
        {
            auto item = i->first;
            auto price = i->second;

            std::string choice = std::string(Item::Descriptions[item]) + " (" + std::to_string(price) + " cacao)";

            auto text = createText(choice.c_str(), FONT_FILE, 16, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < story->Shop.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[idx].W = textwidth + button_space;

            controls[idx].H = text->h;

            idx++;
        }

        controls.push_back(Button(idx, "icons/items.png", idx - 1, idx + 1, idx - 1, idx + 1, startx, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 1, "icons/back-button.png", idx, idx + 1, idx - 1, idx + 1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto messageh = 0.25 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto box_space = 10;

        while (!done)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }
            else
            {
                SDL_SetWindowTitle(window, "Necklace of Skulls: Shop");
            }

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    error = false;
                }
            }
            else if (purchased)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message.c_str(), font, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    purchased = false;
                }
            }

            if (!error && !purchased)
            {
                putText(renderer, "Select an item to buy", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
            }

            putText(renderer, "Possessions", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
            putText(renderer, (std::to_string(player.Items.size()) + std::string(" item(s)")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);

            putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
            putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < story->Shop.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < story->Shop.size())
                    {
                        auto element = story->Shop.at(current);
                        auto item = element.first;
                        auto price = element.second;

                        if (player.Money >= price)
                        {
                            if (Item::IsUnique(item) && Character::VERIFY_ITEMS(player, {item}))
                            {
                                message = std::string("You already have this item!");

                                start_ticks = SDL_GetTicks();

                                purchased = false;

                                error = true;
                            }
                            else
                            {
                                Character::GET_ITEMS(player, {item});

                                player.Money -= price;

                                while (!Character::VERIFY_POSSESSIONS(player))
                                {
                                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                                }

                                message = std::string(std::string(Item::Descriptions[item]) + " purchased.");

                                start_ticks = SDL_GetTicks();

                                purchased = true;

                                error = false;
                            }
                        }
                        else
                        {
                            message = std::string("You do not have enough cacao to buy that!");

                            start_ticks = SDL_GetTicks();

                            purchased = false;

                            error = true;
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    characterScreen(window, renderer, player, story);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();
    }

    return false;
}

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    Story::Base *next = &notImplemented;

    auto error = false;
    const char *message = NULL;

    Uint32 start_ticks = 0;
    Uint32 duration = 5000;

    if (renderer && story->Choices.size() > 0)
    {
        SDL_Surface *splash = NULL;

        if (story->Image)
        {
            splash = createImage(story->Image);
        }

        auto choices = story->Choices;

        auto selected = false;
        auto current = -1;
        auto done = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto controls = std::vector<Button>();

        auto font_size = 20;
        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;
        auto messageh = 0.25 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto box_space = 10;

        for (int i = 0; i < choices.size(); i++)
        {
            auto text = createText(choices[i].Text, FONT_FILE, font_size, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < choices.size() ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = textwidth + button_space;

            controls[i].H = text->h;
        }

        auto idx = choices.size();

        controls.push_back(Button(idx, "icons/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "icons/user.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 2, "icons/items.png", idx + 1, idx + 3, idx - 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 3, "icons/back-button.png", idx + 2, idx + 3, idx - 1, idx + 3, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        int splash_h = splashw;

        if (splash)
        {
            if (splash->w != splashw)
            {
                splash_h = (int)((double)splashw / splash->w * splash->h);
            }
        }

        while (!done)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }

            fillWindow(renderer, intWH);

            if (splash)
            {
                splash_h = fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            }

            if (!splash || (splash && splash_h < (text_bounds - (boxh + infoh))))
            {
                putText(renderer, "Life", font, text_space, clrWH, (player.Life > 0 && story->Type != Story::Type::DOOM) ? intDB : intRD, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
                putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);
            }

            if (!splash || ((splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space)) && !player.RitualBallStarted))
            {
                putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
            }

            if (!splash || ((splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space)) && player.RitualBallStarted))
            {
                std::string score_string = "Ticks: " + std::to_string(player.Ticks) + "\nCross: " + std::to_string(player.Cross);
                putText(renderer, "SCORES", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                putText(renderer, score_string.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
            }

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < duration)
                {
                    putText(renderer, message, font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, text_space, text_space / 2);

            for (auto i = 0; i < story->Choices.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 2 * text_space, controls[i].H + 2 * text_space, controls[i].X - text_space, controls[i].Y - text_space, intBK);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < story->Choices.size())
                    {
                        if (story->Choices[current].Type == Choice::Type::NORMAL)
                        {
                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::ITEM)
                        {
                            if (Character::VERIFY_ITEMS(player, {story->Choices[current].Item}))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required item!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::ALL_ITEMS)
                        {
                            if (Character::VERIFY_ITEMS(player, story->Choices[current].Items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have all the required items!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::CODEWORD)
                        {
                            if (Character::VERIFY_CODEWORD(player, story->Choices[current].Codeword))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required codeword!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_ITEM)
                        {
                            Character::GET_ITEMS(player, {story->Choices[current].Item});

                            while (!Character::VERIFY_POSSESSIONS(player))
                            {
                                inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                            }

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::GIVE_ITEM || story->Choices[current].Type == Choice::Type::LOSE_ITEM)
                        {
                            if (Character::VERIFY_ITEMS(player, {story->Choices[current].Item}))
                            {
                                Character::LOSE_ITEMS(player, {story->Choices[current].Item});

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required item!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_ALL)
                        {
                            Character::LOSE_ALL(player);

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_MONEY)
                        {
                            if (player.Money >= story->Choices[current].Value)
                            {
                                player.Money -= story->Choices[current].Value;

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have enough money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::MONEY)
                        {
                            if (player.Money >= story->Choices[current].Value)
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have enough money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::LIFE)
                        {
                            Character::GAIN_LIFE(player, story->Choices[current].Value);

                            if (player.Life > 0)
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);
                            }

                            done = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::EAT)
                        {
                            auto threshold = story->Choices[current].Value;

                            auto consumed = eatScreen(window, renderer, player, story->Choices[current].Items, threshold);

                            if (consumed >= 0)
                            {
                                Character::GAIN_LIFE(player, consumed - threshold);

                                if (player.Life > 0)
                                {
                                    next = (Story::Base *)findStory(story->Choices[current].Destination);

                                    done = true;

                                    break;
                                }
                                else
                                {
                                    message = "You died of hunger! This adventure is now over.";

                                    start_ticks = SDL_GetTicks();

                                    error = true;
                                }
                            }
                            else
                            {
                                message = "There is nothing in possessions that you can eat.";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::EAT_HEAL)
                        {
                            auto threshold = story->Choices[current].Value;

                            auto consumed = eatScreen(window, renderer, player, story->Choices[current].Items, threshold);

                            if (consumed >= 0)
                            {
                                Character::GAIN_LIFE(player, threshold);

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "There is nothing in possessions that you can eat.";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ANY)
                        {
                            if (Character::VERIFY_SKILL_ANY(player, story->Choices[current].Skill, story->Choices[current].Items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have any of the required item to use with this skill!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL)
                        {
                            if (Character::VERIFY_SKILL(player, story->Choices[current].Skill))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have the required item to use with this skill!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ANY)
                        {
                            if (Character::VERIFY_SKILL_ANY(player, story->Choices[current].Skill, story->Choices[current].Items))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have any of the required item to use with this skill!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL_ITEM)
                        {
                            if (Character::HAS_SKILL(player, story->Choices[current].Skill) && Character::VERIFY_ITEMS(player, {story->Choices[current].Item}))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                done = true;

                                break;
                            }
                            else
                            {
                                if (Character::HAS_SKILL(player, story->Choices[current].Skill))
                                {
                                    message = "You do not have the required item!";
                                }
                                else
                                {
                                    message = "You do not possess the required skill!";
                                }

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::DONATE)
                        {
                            if (player.Money > 0)
                            {
                                auto result = donateScreen(window, renderer, player);

                                if (result)
                                {
                                    next = (Story::Base *)findStory(story->Choices[current].Destination);
                                }
                                else
                                {
                                    next = story;
                                }

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have any money!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::GIFT)
                        {
                            if (player.Items.size() > 0)
                            {
                                auto nextID = giftScreen(window, renderer, story, player, story->Choices[current].Gifts, story->Choices[current].Destination);

                                next = (Story::Base *)findStory(nextID);

                                done = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have any items to give!";

                                start_ticks = SDL_GetTicks();

                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::LOSE_SKILLS)
                        {
                            int limit = story->Choices[current].Value;

                            auto result = loseSkills(window, renderer, player, limit);

                            if (!result)
                            {
                                continue;
                            }
                            else
                            {
                                if (player.Skills.size() <= limit)
                                {
                                    auto nextID = story->Choices[current].Destination;

                                    if (nextID != story->ID)
                                    {
                                        next = (Story::Base *)findStory(nextID);
                                    }
                                    else
                                    {
                                        next = story;
                                    }

                                    done = true;

                                    break;
                                }
                            }
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    characterScreen(window, renderer, player, story);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    mapScreen(window, renderer);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    next = story;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        TTF_Quit();

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }
    }

    return next;
}

Story::Base *renderChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    Story::Base *next = &notImplemented;

    if (story->Choices.size() > 0)
    {
        next = processChoices(window, renderer, player, story);
    }
    else
    {
        next = (Story::Base *)findStory(story->Continue(player));
    }

    return next;
}

bool saveGame(Character::Base &player, const char *overwrite)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);

    std::ostringstream buffer;

#if defined(_WIN32)

    PWSTR path_str;

    SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path_str);

    std::wstring wpath(path_str);

    CoTaskMemFree(path_str);

    std::string save(wpath.length(), ' ');

    std::copy(wpath.begin(), wpath.end(), save.begin());

    save += "/Saved Games/Necklace of Skulls";

    std::string path = save + "/";

#else

    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL)
    {
        homedir = getpwuid(getuid())->pw_dir;
    }

    std::string save = std::string(homedir) + "/Documents/Saved Games/Necklace of Skulls";
    std::string path = save + "/";

#endif

    fs::create_directories(save);

    if (overwrite != NULL)
    {
        buffer << std::string(overwrite);
    }
    else
    {
        buffer << path << std::to_string(seed) << ".save";
    }

    nlohmann::json data;

    player.Epoch = seed;

    data["name"] = player.Name;
    data["description"] = player.Description;
    data["type"] = player.Type;
    data["life"] = player.Life;
    data["money"] = player.Money;
    data["itemLimit"] = player.ITEM_LIMIT;
    data["lifeLimit"] = player.MAX_LIFE_LIMIT;
    data["skillsLimit"] = player.SKILLS_LIMIT;
    data["donation"] = player.DONATION;
    data["isBlessed"] = player.IsBlessed;
    data["isImmortal"] = player.IsImmortal;
    data["ritualBallStarted"] = player.RitualBallStarted;
    data["ticks"] = player.Ticks;
    data["cross"] = player.Cross;
    data["codewords"] = player.Codewords;
    data["lostItems"] = player.LostItems;
    data["epoch"] = player.Epoch;

    auto skills = std::vector<Skill::Type>();
    auto lostSkills = std::vector<Skill::Type>();

    for (auto i = 0; i < player.Skills.size(); i++)
    {
        skills.push_back(player.Skills[i].Type);
    }

    for (auto i = 0; i < player.LostSkills.size(); i++)
    {
        lostSkills.push_back(player.LostSkills[i].Type);
    }

    data["skills"] = skills;
    data["items"] = player.Items;
    data["lostSkills"] = lostSkills;
    data["lostMoney"] = player.LostMoney;
    data["storyID"] = player.StoryID;

    std::string filename = buffer.str();

    std::ofstream file(filename);

    file << data.dump();

    file.close();

    return true;
}

Character::Base loadGame(std::string file_name)
{
    std::ifstream ifs(file_name);

    auto character = Character::Base();

    if (ifs.good())
    {
        auto data = nlohmann::json::parse(ifs);

        ifs.close();

        std::string name = std::string(data["name"]);

        std::string description = data["description"];

        auto type = static_cast<Character::Type>((int)data["type"]);

        auto skills = std::vector<Skill::Base>();
        auto items = std::vector<Item::Type>();
        auto codewords = std::vector<Codeword::Type>();

        auto lostSkills = std::vector<Skill::Base>();
        auto lostItems = std::vector<Item::Type>();

        for (auto i = 0; i < (int)data["skills"].size(); i++)
        {
            auto skill = static_cast<Skill::Type>((int)data["skills"][i]);
            auto found = Skill::FIND(Skill::ALL, skill);

            if (found >= 0)
            {
                skills.push_back(Skill::ALL[found]);
            }
        }

        for (auto i = 0; i < (int)data["lostSkills"].size(); i++)
        {
            auto skill = static_cast<Skill::Type>((int)data["lostSkills"][i]);
            auto found = Skill::FIND(Skill::ALL, skill);

            if (found >= 0)
            {
                lostSkills.push_back(Skill::ALL[found]);
            }
        }

        for (auto i = 0; i < (int)data["items"].size(); i++)
        {
            auto item = static_cast<Item::Type>((int)data["items"][i]);

            items.push_back(item);
        }

        for (auto i = 0; i < (int)data["lostItems"].size(); i++)
        {
            auto item = static_cast<Item::Type>((int)data["lostItems"][i]);

            lostItems.push_back(item);
        }

        for (auto i = 0; i < (int)data["codewords"].size(); i++)
        {
            auto codeword = static_cast<Codeword::Type>((int)data["codewords"][i]);

            codewords.push_back(codeword);
        }

        auto money = (int)data["money"];
        auto life = (int)data["life"];

        character = Character::Base(name.c_str(), type, description.c_str(), skills, items, codewords, life, money);

        character.LostSkills = lostSkills;
        character.LostItems = lostItems;
        character.LostMoney = (int)data["lostMoney"];

        character.Ticks = (int)data["ticks"];
        character.Cross = (int)data["cross"];

        character.IsBlessed = (bool)data["isBlessed"];
        character.IsImmortal = (bool)data["isImmortal"];
        character.RitualBallStarted = (bool)data["ritualBallStarted"];

        character.DONATION = (int)data["donation"];

        character.ITEM_LIMIT = (int)data["itemLimit"];
        character.MAX_LIFE_LIMIT = (int)data["lifeLimit"];
        character.SKILLS_LIMIT = (int)data["skillsLimit"];
        character.StoryID = (int)data["storyID"];

        try
        {

#if defined(_WIN32) || defined(__arm__)
            character.Epoch = (long long)(data["epoch"]);
#else
            character.Epoch = (long)(data["epoch"]);
#endif
        }
        catch (std::exception &ex)
        {
            character.Epoch = 0;
        }
    }
    else
    {
        character.StoryID = -1;
    }

    return character;
}

#if defined(_WIN32) || defined(__arm__)
std::string time_string(long long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::system_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    std::chrono::system_clock::time_point timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    if (in_time_t >= 0)
    {
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    }

    return ss.str();
}
#else
std::string time_string(long deserialised)
{
    auto epoch = std::chrono::time_point<std::chrono::high_resolution_clock>();
    auto since_epoch = std::chrono::milliseconds(deserialised);
    auto timestamp = epoch + since_epoch;

    auto in_time_t = std::chrono::system_clock::to_time_t(timestamp);

    std::stringstream ss;

    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");

    return ss.str();
}
#endif

std::vector<Button> createFilesList(SDL_Window *window, SDL_Renderer *renderer, std::vector<std::string> list, int start, int last, int limit, bool save_button)
{
    auto controls = std::vector<Button>();

    auto text_space = 8;

    if (list.size() > 0)
    {
        for (int i = 0; i < last - start; i++)
        {
            std::string game_string = "";

            auto index = start + i;

            auto character = loadGame(list[index]);

#if defined(_WIN32) || defined(__arm__)
            long long epoch_long;
#else
            long epoch_long;
#endif

            if (character.Epoch == 0)
            {
                auto epoch = list[index].substr(list[index].find_last_of("/") + 1, list[index].find_last_of(".") - list[index].find_last_of("/") - 1);

#if defined(_WIN32) || defined(__arm__)
                epoch_long = std::stoull(epoch);
#else
                epoch_long = std::stol(epoch);
#endif
            }
            else
            {
                epoch_long = character.Epoch;
            }

            if (character.StoryID != -1)
            {
                auto storyID = std::to_string(character.StoryID);

                game_string += std::string(3 - std::to_string(index + 1).length(), '0') + std::to_string(index + 1) + ". " + character.Name + "\n";
                game_string += "Date: " + time_string(epoch_long) + "\n";
                game_string += "Section: " + std::string(3 - storyID.length(), '0') + storyID + ": ";
                game_string += "Life: " + std::to_string(character.Life);
                game_string += ", Money: " + std::to_string(character.Money);
                game_string += ", Items: " + std::to_string(character.Items.size());
                game_string += ", Codewords: " + std::to_string(character.Codewords.size());
            }

            auto button = createHeaderButton(window, game_string.c_str(), clrWH, intLB, textwidth - 3 * button_space / 2, 0.125 * SCREEN_HEIGHT, text_space);

            auto y = (i > 0 ? controls[i - 1].Y + controls[i - 1].H + 3 * text_space : texty + 2 * text_space);

            controls.push_back(Button(i, button, i, i, (i > 0 ? i - 1 : i), (i < (last - start) ? i + 1 : i), textx + 2 * text_space, y, Control::Type::ACTION));

            controls[i].W = button->w;

            controls[i].H = button->h;
        }
    }

    auto idx = controls.size();

    if (list.size() > limit)
    {
        if (start > 0)
        {
            controls.push_back(Button(idx, "icons/up-arrow.png", idx, idx, idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + border_space, Control::Type::SCROLL_UP));

            idx++;
        }

        if (list.size() - last > 0)
        {
            controls.push_back(Button(idx, "icons/down-arrow.png", idx, idx, start > 0 ? idx - 1 : idx, idx + 1, (1.0 - Margin) * SCREEN_WIDTH - arrow_size, texty + text_bounds - arrow_size - border_space, Control::Type::SCROLL_DOWN));

            idx++;
        }
    }

    controls.push_back(Button(idx, "icons/open.png", idx, idx + 1, idx > 0 ? idx - 1 : idx, idx, startx, buttony, Control::Type::LOAD));

    if (save_button)
    {
        controls.push_back(Button(idx + 1, "icons/disk.png", idx, idx + 2, idx > 0 ? idx - 1 : idx + 1, idx + 1, startx + gridsize, buttony, Control::Type::SAVE));
    }

    idx = controls.size();

    controls.push_back(Button(idx, "icons/back-button.png", idx - 1, idx, list.size() > 0 ? (last - start) : idx, idx, (1.0 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

    return controls;
}

Control::Type gameScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, bool save_botton)
{
    auto result = Control::Type::BACK;
    auto done = false;

    if (window && renderer)
    {
#if defined(_WIN32)
        PWSTR path_str;

        SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, NULL, &path_str);

        std::wstring wpath(path_str);

        CoTaskMemFree(path_str);

        std::string save(wpath.length(), ' ');

        std::copy(wpath.begin(), wpath.end(), save.begin());

        save += "/Saved Games/Necklace of Skulls";

        std::string path = save + "/";
#else
        const char *homedir;

        if ((homedir = getenv("HOME")) == NULL)
        {
            homedir = getpwuid(getuid())->pw_dir;
        }

        std::string save = std::string(homedir) + "/Documents/Saved Games/Necklace of Skulls";
        std::string path = save + "/";
#endif

        fs::create_directories(save);

        std::vector<std::string> entries;

        auto splash = createImage("images/filler6.png");

        auto saved_games = std::multimap<std::filesystem::file_time_type, std::string, std::greater<std::filesystem::file_time_type>>();

        try
        {
            for (const auto &entry : fs::directory_iterator(path))
            {
                auto time_stamp = entry.last_write_time();

#if defined(_WIN32) || defined(__arm__)
                std::string file_name = entry.path().string();
#else
                std::string file_name = entry.path();
#endif

                if (file_name.substr(file_name.find_last_of(".") + 1) == "save")
                {
                    saved_games.insert(std::make_pair(time_stamp, file_name));
                }
            }
        }
        catch (std::exception &ex)
        {
            std::cerr << "Unable to read save directory!" << std::endl;
        }

        for (auto const &entry : saved_games)
        {
            entries.push_back(entry.second);
        }

        auto text_space = 8;
        auto infoh = 0.07 * SCREEN_HEIGHT;
        auto boxh = 0.125 * SCREEN_HEIGHT;
        auto box_space = 10;

        auto offset = 0;
        auto last = 0;
        int limit = (text_bounds - text_space) / (boxh + 3 * text_space);

        last = offset + limit;

        if (last > entries.size())
        {
            last = entries.size();
        }

        auto controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

        auto current = -1;
        auto selected = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;
        auto scrollSpeed = 1;
        auto font_size = 20;
        auto selected_file = -1;

        TTF_Init();

        auto font = TTF_OpenFont(FONT_FILE, font_size);

        while (!done)
        {
            last = offset + limit;

            if (last > entries.size())
            {
                last = entries.size();
            }

            SDL_SetWindowTitle(window, "Necklace of Skulls: LOAD/SAVE game");

            // Fill the surface with background color
            fillWindow(renderer, intWH);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);

            fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, border_space, border_pts);

            putText(renderer, "Selected", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * boxh + infoh));

            if (selected_file >= 0 && selected_file < entries.size())
            {
                std::string game_string = "";

                auto character = loadGame(entries[selected_file]);

#if defined(_WIN32) || defined(__arm__)
                long long epoch_long;
#else
                long epoch_long;
#endif

                if (character.Epoch > 0)
                {
                    epoch_long = character.Epoch;
                }
                else
                {
                    auto epoch = entries[selected_file].substr(entries[selected_file].find_last_of("/") + 1, entries[selected_file].find_last_of(".") - entries[selected_file].find_last_of("/") - 1);

#if defined(_WIN32) || defined(__arm__)
                    epoch_long = std::stoull(epoch);
#else
                    epoch_long = std::stol(epoch);
#endif
                }

                if (character.StoryID != -1)
                {
                    auto storyID = std::to_string(character.StoryID);

                    game_string = "Date: " + time_string(epoch_long) + "\n";
                    game_string += std::string(3 - storyID.length(), '0') + storyID + ": " + character.Name;
                    game_string += "\nLife: " + std::to_string(character.Life);
                    game_string += " Money: " + std::to_string(character.Money);
                }

                putText(renderer, game_string.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh);
            }
            else
            {
                fillRect(renderer, splashw, 2 * boxh, startx, starty + text_bounds - 2 * boxh, intBE);
            }

            if (last - offset > 0)
            {
                for (auto i = 0; i < last - offset; i++)
                {
                    if (offset + i == selected_file)
                    {
                        for (auto size = 4; size >= 0; size--)
                        {
                            drawRect(renderer, controls[i].W + 2 * text_space - 2 * size, controls[i].H + 2 * text_space - 2 * size, controls[i].X - text_space + size, controls[i].Y - text_space + size, intBK);
                        }
                    }
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold)
            {
                if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                {
                    if (offset > 0)
                    {
                        offset -= scrollSpeed;

                        if (offset < 0)
                        {
                            offset = 0;
                        }

                        last = offset + limit;

                        if (last > entries.size())
                        {
                            last = entries.size();
                        }

                        controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

                        SDL_Delay(200);
                    }

                    if (offset <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                {
                    if (entries.size() - last > 0)
                    {
                        if (offset < entries.size() - limit)
                        {
                            offset += scrollSpeed;
                        }

                        if (offset > entries.size() - limit)
                        {
                            offset = entries.size() - limit;
                        }

                        last = offset + limit;

                        if (last > entries.size())
                        {
                            last = entries.size();
                        }

                        controls = createFilesList(window, renderer, entries, offset, last, limit, save_botton);

                        SDL_Delay(200);

                        if (offset > 0)
                        {
                            if (controls[current].Type != Control::Type::SCROLL_DOWN)
                            {
                                current++;
                            }
                        }
                    }

                    if (entries.size() - last <= 0)
                    {
                        selected = false;

                        current = -1;
                    }
                }
                else if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (offset + current == selected_file)
                    {
                        selected_file = -1;
                    }
                    else
                    {
                        selected_file = offset + current;
                    }
                }
                else if (controls[current].Type == Control::Type::LOAD && !hold)
                {
                    if (selected_file >= 0 && selected_file < entries.size())
                    {
                        player = loadGame(entries[selected_file]);

                        result = Control::Type::LOAD;

                        done = true;

                        break;
                    }
                }
                else if (controls[current].Type == Control::Type::SAVE && !hold)
                {
                    if (selected_file != -1)
                    {
                        saveGame(player, entries[selected_file].c_str());
                    }
                    else
                    {
                        saveGame(player, NULL);
                    }

                    result = Control::Type::SAVE;

                    done = true;

                    break;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    result = Control::Type::BACK;

                    done = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;
        }

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }

        TTF_Quit();
    }

    return result;
}

void clipValue(int &val, int min, int max)
{
    if (val < min)
    {
        val = min;
    }
    if (val > max)
    {
        val = max;
    }
}

bool processStory(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    auto quit = false;
    auto font_size = 20;
    auto text_space = 8;

    TTF_Init();

    auto font = TTF_OpenFont(FONT_FILE, font_size);

    auto messageh = 0.25 * SCREEN_HEIGHT;
    auto infoh = 0.07 * SCREEN_HEIGHT;
    auto boxh = 0.125 * SCREEN_HEIGHT;
    auto box_space = 10;

    Character::Base saveCharacter;

    std::vector<Button> controls = std::vector<Button>();

    while (!quit)
    {
        auto flash_message = false;

        auto flash_color = intRD;

        const char *message = NULL;

        Uint32 start_ticks = 0;

        Uint32 duration = 5000;

        player.StoryID = story->ID;

        // capture player state before running the story
        saveCharacter = player;

        auto run_once = true;

        SDL_Surface *splash = NULL;
        SDL_Texture *splashTexture = NULL;
        SDL_Surface *text = NULL;

        if (run_once)
        {
            run_once = false;

            auto jump = story->Background(player);

            if (jump >= 0)
            {
                story = (Story::Base *)findStory(jump);

                continue;
            }

            story->Event(player);
        }

        int splash_h = 250;

        if (story->Image)
        {
            splash = createImage(story->Image);

            if (splash)
            {
                if (splash->w != splashw)
                {
                    splash_h = (int)((double)splashw / splash->w * splash->h);
                }

                splashTexture = SDL_CreateTextureFromSurface(renderer, splash);
            }
        }

        if (story->Text)
        {
            auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

            text = createText(story->Text, FONT_FILE, font_size, clrDB, textwidth, TTF_STYLE_NORMAL);
        }

        auto compact = (text && text->h <= text_bounds - 2 * text_space) || text == NULL;

        if (story->Controls == Story::Controls::STANDARD)
        {
            controls = Story::StandardControls(compact);
        }
        else if (story->Controls == Story::Controls::SHOP)
        {
            controls = Story::ShopControls(compact);
        }
        else if (story->Controls == Story::Controls::TRADE)
        {
            controls = Story::TradeControls(compact);
        }
        else
        {
            controls = Story::ExitControls(compact);
        }

        if (story->Type != Story::Type::NORMAL || player.Life <= 0)
        {
            controls = Story::ExitControls(compact);
        }

        auto trigger_blessing = player.IsBlessed && saveCharacter.Life > player.Life;

        auto bless_text = createText("You have lost some Life Points. Do you wish to use the War God's Blessing?", FONT_FILE, font_size, clrWH, textwidth - 2 * text_space, TTF_STYLE_NORMAL);
        auto message_x = (SCREEN_WIDTH - textwidth) / 2;
        auto message_y = (SCREEN_HEIGHT - messageh) / 2;

        auto message_controls = std::vector<Button>();
        message_controls.push_back(Button(0, "icons/yes.png", 0, 1, 0, 0, message_x + button_space, message_y + messageh - buttonh - button_space, Control::Type::YES));
        message_controls.push_back(Button(1, "icons/no.png", 0, 1, 1, 1, message_x + textwidth - button_space - buttonw, message_y + messageh - buttonh - button_space, Control::Type::NO));

        // Render the image
        if (window && renderer)
        {
            auto scrollSpeed = 20;
            auto hold = false;

            auto selected = false;
            auto current = -1;
            auto offset = 0;

            while (!quit)
            {
                if (story->Title)
                {
                    SDL_SetWindowTitle(window, story->Title);
                }
                else
                {
                    if (story->ID > 0)
                    {
                        SDL_SetWindowTitle(window, (std::string("Necklace of Skulls: ") + std::string(3 - std::to_string(story->ID).length(), '0') + std::to_string(story->ID)).c_str());
                    }
                    else
                    {
                        SDL_SetWindowTitle(window, "Necklace of Skulls");
                    }
                }

                // Fill the surface with background color
                fillWindow(renderer, intWH);

                if (splash)
                {
                    splash_h = fitImage(renderer, splash, startx, texty, splashw, text_bounds);
                }

                if (!splash || (splash && splash_h < (text_bounds - (boxh + infoh))))
                {
                    putText(renderer, "Life", font, text_space, clrWH, (player.Life > 0 && story->Type != Story::Type::DOOM) ? intDB : intRD, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (boxh + infoh));
                    putText(renderer, (std::to_string(player.Life)).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - boxh);
                }

                if (!splash || ((splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space)) && !player.RitualBallStarted))
                {
                    putText(renderer, "Money", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                    putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
                }

                if ((!splash || (splash && splash_h < text_bounds - (2 * (boxh + infoh) + box_space))) && player.RitualBallStarted)
                {
                    std::string score_string = "Ticks: " + std::to_string(player.Ticks) + "\nCross: " + std::to_string(player.Cross);
                    putText(renderer, "SCORES", font, text_space, clrWH, intDB, TTF_STYLE_NORMAL, splashw, infoh, startx, starty + text_bounds - (2 * (boxh + infoh) + box_space));
                    putText(renderer, score_string.c_str(), font, text_space, clrBK, intBE, TTF_STYLE_NORMAL, splashw, boxh, startx, starty + text_bounds - (2 * boxh + infoh + box_space));
                }

                fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);

                if (story->Text && text)
                {
                    renderText(renderer, text, intBE, textx + text_space, texty + text_space, text_bounds - 2 * text_space, offset);
                }

                if (flash_message)
                {
                    if ((SDL_GetTicks() - start_ticks) < duration)
                    {
                        putText(renderer, message, font, text_space, clrWH, flash_color, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                    }
                    else
                    {
                        flash_message = false;
                    }
                }

                renderButtons(renderer, controls, trigger_blessing ? -1 : current, intGR, border_space, border_pts);

                if (trigger_blessing)
                {
                    fillRect(renderer, textwidth, messageh, message_x, message_y, intLB);

                    renderImage(renderer, bless_text, (SCREEN_WIDTH - bless_text->w) / 2, message_y + text_space);

                    renderButtons(renderer, message_controls, current, intWH, border_space, border_pts);
                }

                bool scrollUp = false;
                bool scrollDown = false;

                if (splash)
                {
                    auto mousex = 0;
                    auto mousey = 0;

                    auto state = SDL_GetMouseState(&mousex, &mousey);

                    auto zoomw = (int)(0.80 * (double)textwidth);
                    auto zoomh = (int)(0.80 * (double)text_bounds);

                    clipValue(zoomw, 0, splash->w);
                    clipValue(zoomh, 0, splash->h);

                    auto boundx = splashw;

                    if (splash_h == text_bounds)
                    {
                        boundx = (int)((double)splash_h / splash->h * (double)splash->w);
                    }

                    if (mousex >= startx && mousex <= (startx + boundx) && mousey >= starty && mousey <= (starty + splash_h))
                    {
                        auto scalex = (double)(mousex - startx) / boundx;
                        auto scaley = (double)(mousey - starty) / splash_h;

                        int centerx = (int)(scalex * (double)splash->w);
                        int centery = (int)(scaley * (double)splash->h);

                        clipValue(centerx, zoomw / 2, splash->w - zoomw / 2);
                        clipValue(centery, zoomh / 2, splash->h - zoomh / 2);

                        if (splashTexture)
                        {
                            SDL_Rect src;

                            src.w = zoomw;
                            src.h = zoomh;
                            src.x = centerx - zoomw / 2;
                            src.y = centery - zoomh / 2;

                            SDL_Rect dst;

                            dst.w = zoomw;
                            dst.h = zoomh;
                            dst.x = (textx + (textwidth - zoomw) / 2);
                            dst.y = (texty + (text_bounds - zoomh) / 2);

                            SDL_RenderCopy(renderer, splashTexture, &src, &dst);
                            drawRect(renderer, dst.w, dst.h, dst.x, dst.y, intBK);
                        }
                    }
                }

                if (story->Type == Story::Type::DOOM)
                {
                    putText(renderer, "You have failed. This adventure is over.", font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else if (player.Life <= 0)
                {
                    putText(renderer, "You have died. This adventure is over.", font, text_space, clrWH, intRD, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else if (story->Type == Story::Type::RESTART)
                {
                    putText(renderer, "It is time to begin a new adventure.", font, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }
                else if (story->Type == Story::Type::GOOD)
                {
                    putText(renderer, "You have defeated Necklace of Skulls! This adventure is over. Further adventure awaits!", font, text_space, clrWH, intLB, TTF_STYLE_NORMAL, splashw, messageh, startx, starty);
                }

                if (trigger_blessing)
                {
                    quit = Input::GetInput(renderer, message_controls, current, selected, scrollUp, scrollDown, hold);
                }
                else
                {
                    quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);
                }

                if (!trigger_blessing && ((selected && current >= 0 && current < controls.size()) || scrollUp || scrollDown || hold))
                {
                    if (controls[current].Type == Control::Type::SCROLL_UP || (controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
                    {
                        if (text)
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
                    }
                    else if (controls[current].Type == Control::Type::SCROLL_DOWN || ((controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                    {
                        if (text)
                        {
                            if (text->h >= text_bounds - 2 * text_space)
                            {
                                if (offset < text->h - text_bounds + 2 * text_space)
                                {
                                    offset += scrollSpeed;
                                }

                                if (offset > text->h - text_bounds + 2 * text_space)
                                {
                                    offset = text->h - text_bounds + 2 * text_space;
                                }
                            }
                        }
                    }
                    else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                    {
                        characterScreen(window, renderer, player, story);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::MAP && !hold)
                    {
                        mapScreen(window, renderer);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::USE && !hold)
                    {
                        inventoryScreen(window, renderer, player, story, player.Items, Control::Type::USE, 0);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::SHOP && !hold)
                    {
                        shopScreen(window, renderer, player, story);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::TRADE && !hold)
                    {
                        tradeScreen(window, renderer, player, story->Trade.first, story->Trade.second);

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::GAME && !hold)
                    {
                        auto result = gameScreen(window, renderer, saveCharacter, true);

                        if (result == Control::Type::SAVE)
                        {
                            message = "Game saved!";

                            start_ticks = SDL_GetTicks();

                            flash_message = true;

                            flash_color = intLB;
                        }
                        else if (result == Control::Type::LOAD)
                        {
                            if (saveCharacter.StoryID >= 0 && saveCharacter.Life > 0)
                            {
                                player = saveCharacter;

                                story = (Story::Base *)findStory(saveCharacter.StoryID);

                                message = "Game loaded!";

                                start_ticks = SDL_GetTicks();

                                flash_message = true;

                                flash_color = intLB;

                                break;
                            }
                        }

                        current = -1;

                        selected = false;
                    }
                    else if (controls[current].Type == Control::Type::NEXT && !hold)
                    {
                        if (story->LimitSkills > 0)
                        {
                            auto done = loseSkills(window, renderer, player, story->LimitSkills);

                            if (!done)
                            {
                                continue;
                            }
                            else
                            {
                                story->LimitSkills = 0;
                            }
                        }

                        if (story->Take.size() > 0 && story->Limit > 0)
                        {
                            auto done = takeScreen(window, renderer, player, story->Take, story->Limit);

                            if (!done)
                            {
                                continue;
                            }
                            else
                            {
                                story->Limit = 0;
                            }
                        }

                        if (story->Limit > 0 && story->ToLose.size() > story->Limit)
                        {
                            while (story->ToLose.size() > story->Limit)
                            {
                                inventoryScreen(window, renderer, player, story, story->ToLose, Control::Type::STEAL, story->Limit);
                            }
                        }

                        while (!Character::VERIFY_POSSESSIONS(player))
                        {
                            inventoryScreen(window, renderer, player, story, player.Items, Control::Type::DROP, 0);
                        }

                        current = -1;

                        selected = false;

                        auto next = renderChoices(window, renderer, player, story);

                        if (next->ID != story->ID)
                        {
                            if (story->Bye)
                            {
                                auto bye = createText(story->Bye, FONT_FILE, font_size + 4, clrBK, (SCREEN_WIDTH * (1.0 - 2.0 * Margin)) - 2 * text_space, TTF_STYLE_NORMAL);
                                auto forward = createImage("icons/next.png");

                                if (bye && forward)
                                {
                                    fillWindow(renderer, intWH);

                                    fillRect(renderer, (1.0 - 2.0 * Margin) * SCREEN_WIDTH, 2 * bye->h, startx, (SCREEN_HEIGHT - 2 * bye->h) / 2, intBE);

                                    renderText(renderer, bye, intBE, (SCREEN_WIDTH - bye->w) / 2, (SCREEN_HEIGHT - bye->h) / 2, SCREEN_HEIGHT, 0);

                                    renderImage(renderer, forward, SCREEN_WIDTH * (1.0 - Margin) - buttonw - button_space, buttony);

                                    SDL_RenderPresent(renderer);

                                    Input::WaitForNext(renderer);

                                    SDL_FreeSurface(bye);

                                    bye = NULL;

                                    SDL_FreeSurface(forward);

                                    forward = NULL;
                                }
                            }

                            story = next;

                            break;
                        }
                        else if (player.Life <= 0)
                        {
                            controls = Story::ExitControls(compact);
                        }
                    }
                    else if (controls[current].Type == Control::Type::BACK && !hold)
                    {
                        quit = true;

                        break;
                    }
                }
                else if (trigger_blessing && (selected && current >= 0 && current < message_controls.size()))
                {
                    if (message_controls[current].Type == Control::Type::YES && !hold)
                    {
                        player.IsBlessed = false;

                        player.Life = saveCharacter.Life;

                        trigger_blessing = false;

                        message = "You used the blessing to RECOVER the LOST Life Points.";

                        start_ticks = SDL_GetTicks();

                        flash_message = true;

                        flash_color = intLB;
                    }
                    else
                    {
                        trigger_blessing = false;
                    }
                }
            }
        }

        if (splash)
        {
            SDL_FreeSurface(splash);

            splash = NULL;
        }

        if (splashTexture)
        {
            SDL_DestroyTexture(splashTexture);

            splashTexture = NULL;
        }

        if (text)
        {
            SDL_FreeSurface(text);

            text = NULL;
        }

        if (bless_text)
        {
            SDL_FreeSurface(bless_text);

            bless_text = NULL;
        }
    }

    if (font)
    {
        TTF_CloseFont(font);

        font = NULL;
    }

    TTF_Quit();

    return quit;
}

bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int id)
{
    auto story = (Story::Base *)findStory(id);

    return processStory(window, renderer, player, story);
}

bool mainScreen(SDL_Window *window, SDL_Renderer *renderer, int storyID = 0)
{
    auto font_size = 20;

    auto *introduction = "The sole survivor of an expedition brings news of disaster. Your twin brother is lost in the trackless western sierra. Resolving to find out his fate, you leave the safety of your home far behind. Your quest takes you to lost jungle cities, across mountains and seas, and even into the depths of the underworld.\n\nYou will plunge into the eerie world of Mayan myth. You will confront ghosts and gods, bargain for your life against wily demons, find allies and enemies among both the living and the dead. If you are breave enough to survive the dangers of the spirit-haunted western desert, you must still confront the wizard called Necklace of skulls in a deadly contest whose stakes are nothing less than your own soul.";

    auto splash = createImage("images/skulls-cover.png");

    auto text = createText(introduction, FONT_FILE, font_size, clrWH, SCREEN_WIDTH * (1.0 - 3.0 * Margin) - splashw);

    auto title = "Necklace of Skulls";

    InitializeStories();

    Character::Base Player;

    // Render window
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, title);

        const char *choices[4] = {"New Game", "Load Game", "About", "Exit"};

        auto current = -1;

        auto selected = false;

        auto main_buttonh = 48;

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::NEW;
        controls[1].Type = Control::Type::LOAD;
        controls[2].Type = Control::Type::ABOUT;
        controls[3].Type = Control::Type::QUIT;

        auto quit = false;

        while (!quit)
        {
            // Fill the surface with background color
            fillWindow(renderer, intDB);

            fitImage(renderer, splash, startx, starty, splashw, text_bounds);
            renderText(renderer, text, intDB, startx * 2 + splashw, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, FONT_FILE, current, clrWH, intBK, intRD, font_size, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            Control::Type result;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::NEW:

                    Player = selectCharacter(window, renderer);

                    if (Player.StoryID != -1)
                    {
                        storyScreen(window, renderer, Player, storyID);
                    }

                    current = -1;

                    selected = false;

                    storyID = 0;

                    break;

                case Control::Type::ABOUT:

                    aboutScreen(window, renderer);

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::LOAD:

                    result = gameScreen(window, renderer, Player, false);

                    if (result == Control::Type::LOAD)
                    {
                        // TODO: add sanity check to the loaded player
                        if (Player.StoryID >= 0 && Player.Life > 0)
                        {
                            storyScreen(window, renderer, Player, Player.StoryID);
                        }
                    }

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::QUIT:

                    quit = true;

                    break;

                default:

                    selected = false;

                    quit = false;

                    break;
                }
            }

            SDL_SetWindowTitle(window, title);
        }

        SDL_FreeSurface(splash);
        SDL_FreeSurface(text);

        splash = NULL;
        text = NULL;
    }

    return false;
}

int main(int argc, char **argv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Necklace of Skulls";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "icons/maya.png");

    auto numGamePads = Input::InitializeGamePads();

    auto quit = false;

    auto storyID = 0;

    if (argc > 1)
    {
        storyID = std::atoi(argv[1]);
    }

    if (window)
    {
        quit = mainScreen(window, renderer, storyID);

        // Destroy window and renderer
        SDL_DestroyRenderer(renderer);

        renderer = NULL;

        SDL_DestroyWindow(window);

        window = NULL;
    }

    // Quit SDL subsystems
    IMG_Quit();

    SDL_Quit();

    return 0;
}
