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

#include "constants.hpp"
#include "controls.hpp"
#include "input.hpp"
#include "items.hpp"
#include "skills.hpp"
#include "story.hpp"

#include "character.hpp"

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
    }
}

void waitForEvent(SDL_Renderer *renderer, Uint32 event)
{
    SDL_RenderPresent(renderer);

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

void putImage(SDL_Renderer *renderer, const char *image, int x, int y)
{
    auto surface = createImage(image);

    if (surface)
    {
        renderImage(renderer, surface, x, y);

        SDL_FreeSurface(surface);

        surface = NULL;
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

            renderText(renderer, text, bg, x, y, fontsize, 0);

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
    std::vector<TextButton> controls = std::vector<TextButton>();

    if (num > 0)
    {
        auto margin1 = (1.0 - Margin);
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

template <typename Function>
bool renderWindow(SDL_Window *window, SDL_Renderer *renderer, Function displayScreen)
{
    auto result = displayScreen(window, renderer);

    SDL_SetWindowTitle(window, "Necklace of Skulls");

    return result;
}

template <typename Function>
bool renderWindow(SDL_Window *window, SDL_Renderer *renderer, Function displayScreen, Character::Base &player)
{
    auto result = displayScreen(window, renderer, player);

    SDL_SetWindowTitle(window, "Necklace of Skulls");

    return result;
}

template <typename Function>
bool renderWindow(SDL_Window *window, SDL_Renderer *renderer, Function displayScreen, Character::Base &player, int id)
{
    auto result = displayScreen(window, renderer, player, id);

    return result;
}

template <typename Function>
bool renderWindow(SDL_Window *window, SDL_Renderer *renderer, Function displayScreen, Character::Base &player, Story::Base *story)
{
    auto result = displayScreen(window, renderer, player, story);

    SDL_SetWindowTitle(window, "Necklace of Skulls");

    return result;
}

template <typename Function>
bool renderWindow(SDL_Window *window, SDL_Renderer *renderer, Function displayScreen, Character::Base &player, Control::Type mode)
{
    auto result = displayScreen(window, renderer, player, mode);

    SDL_SetWindowTitle(window, "Necklace of Skulls");

    return result;
}

bool aboutScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto quit = false;

    auto *about = "Virtual Reality Adventure Games are solo adventures with a big difference. They're not random. Whether you live or die doesn't depend on a dice roll -- it's up to you.\n\nTo start your adventure simply choose your character. Each character has a unique selection of four skills: these skills will decide which options are available to you.\n\nAlso note the Life Points and possessions of the character. Life Points are lost each time you are wounded. If you are ever reduced to zero Life Points, you have been killed and the adventure ends. Sometimes you can recover Life Points during the adventure, but you can never have more Life Points that you started with. You can carry up to eight possessions at a time. If you are at this limit and find something else you want, drop one of your other possessions to make room for the new item.\n\nConsider your selection of skills. They establish your special strengths, and will help you to role-play your choices during the adventure. If you arrive at an entry which lists options for more than one of your skills, you can choose which skill to use in that situation.";

    auto splash = createImage("images/pyramid.png");
    auto text = createText(about, "fonts/default.ttf", 18, clrWH, SCREEN_WIDTH * (1.0 - 3 * Margin) - splash->w);

    // Render the image
    if (window && renderer && splash && text)
    {
        SDL_SetWindowTitle(window, "About the game");

        auto selected = false;
        auto current = -1;

        auto about_buttonw = 150;
        auto about_buttonh = 48;

        auto about_buttony = (int)(SCREEN_HEIGHT * (1 - Margin) - buttonh);

        std::vector<TextButton> controls = {TextButton(0, "Back", 0, 0, 0, 0, startx, about_buttony, about_buttonw, about_buttonh, Control::Type::BACK)};

        while (!quit)
        {
            // Fill the surface with background color
            fillWindow(renderer, intDB);

            renderImage(renderer, splash, startx, starty);
            renderText(renderer, text, intDB, startx * 2 + splash->w, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

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

    return quit;
}

bool mapScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto quit = false;

    auto splash = createImage("images/map-one-world.png");

    // Render the image
    if (window && renderer && splash)
    {
        SDL_SetWindowTitle(window, "Map: One World");

        auto selected = false;
        auto current = -1;

        const int map_buttonw = 150;
        const int map_buttonh = 48;
        const int map_buttony = (int)(SCREEN_HEIGHT * (1 - Margin) - map_buttonh);

        std::vector<TextButton> controls = {TextButton(0, "Back", 0, 0, 0, 0, startx, map_buttony, map_buttonw, map_buttonh, Control::Type::BACK)};

        while (!quit)
        {
            // Fill the surface with background color
            fillWindow(renderer, intDB);

            renderImage(renderer, splash, startx, starty);
            renderTextButtons(renderer, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
            {
                break;
            }
        }

        SDL_FreeSurface(splash);

        splash = NULL;
    }

    return quit;
}

void renderAdventurer(SDL_Window *window, SDL_Renderer *renderer, TTF_Font *font, Character::Base &player)
{
    auto main_buttonh = 48;

    const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
    const int profileh = 70;

    auto headerw = 150;
    auto headerh = 36;
    auto space = 8;
    auto font_size = 18;

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
    auto boxh = (profileh) / 2;

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

    putText(renderer, player.Name, font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty);
    putText(renderer, player.Description, font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + headerh);
    putText(renderer, "Skills", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + headerh + marginh);
    putText(renderer, skills.c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, boxh, startx, starty + profileh + 2 * headerh + marginh);

    putText(renderer, "Money", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, (std::to_string(player.Money) + " cacao").c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx, starty + profileh + 3 * headerh + 2 * marginh + boxh);
    putText(renderer, "Life", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx + boxw + marginw, starty + profileh + 2 * headerh + 2 * marginh + boxh);
    putText(renderer, std::to_string(player.Life).c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, boxw, boxh, startx + boxw + marginw, starty + profileh + 3 * headerh + 2 * marginh + boxh);

    putText(renderer, "Possessions", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + profileh + 3 * headerh + 3 * marginh + 2 * boxh);
    putText(renderer, possessions.c_str(), font, space, clrBK, intBE, TTF_STYLE_NORMAL, profilew, profileh, startx, starty + profileh + 4 * headerh + 3 * marginh + 2 * boxh);
}

bool characterScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player)
{
    std::string title = "Necklace of Skulls: Adventure Sheet";

    auto quit = false;

    // Render the image
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title.c_str());

        auto selected = false;
        auto current = -1;

        const int map_buttonw = 150;
        const int map_buttonh = 48;
        const int map_buttony = (int)(SCREEN_HEIGHT * (1 - Margin) - map_buttonh);
        const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
        const int profileh = 70;

        std::vector<Button> controls = {Button(0, "images/back-button.png", 0, 0, -1, -1, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK)};

        auto headerw = 150;
        auto headerh = 36;
        auto space = 8;
        auto font_size = 18;

        auto marginw = Margin * SCREEN_WIDTH;
        auto marginh = Margin * SCREEN_HEIGHT / 2;

        auto boxw = (profilew - marginw) / 2;
        auto boxh = (profileh) / 2;

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

        auto font = TTF_OpenFont("fonts/default.ttf", font_size);

        if (font)
        {
            while (!quit)
            {
                // Fill the surface with background color
                fillWindow(renderer, intWH);

                renderAdventurer(window, renderer, font, player);

                if (player.Codewords.size() > 0)
                {
                    putText(renderer, "Codewords", font, space, clrWH, intDB, TTF_STYLE_NORMAL, headerw, headerh, startx, starty + 2 * profileh + 4 * headerh + 4 * marginh + 2 * boxh);
                    putText(renderer, codewords.c_str(), font, space, clrBK, intBE, TTF_STYLE_ITALIC, profilew - buttonw - 2 * space, profileh, startx, starty + 2 * profileh + 5 * headerh + 4 * marginh + 2 * boxh);
                }

                renderButtons(renderer, controls, current, intGR, space, space / 2);

                bool scrollUp = false;
                bool scrollDown = false;
                bool hold = false;

                quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

                if (selected && current >= 0 && current < controls.size() && controls[current].Type == Control::Type::BACK)
                {
                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;

            TTF_Quit();
        }
    }

    return false;
}

Character::Base selectCharacter(SDL_Window *window, SDL_Renderer *renderer)
{
    std::string title = "Necklace of Skulls: Select Character";

    auto quit = false;

    Character::Base player = Character::WARRIOR;

    // Render the image
    if (window && renderer)
    {
        SDL_SetWindowTitle(window, title.c_str());

        auto selected = false;
        auto current = -1;
        auto character = 0;

        auto main_buttonh = 48;

        const int profilew = SCREEN_WIDTH * (1.0 - 2.0 * Margin);
        const int profileh = 70;

        auto headerw = 150;
        auto headerh = 36;
        auto space = 8;
        auto font_size = 18;

        auto marginw = Margin * SCREEN_WIDTH;
        auto marginh = Margin * SCREEN_HEIGHT / 2;

        const char *choices[4] = {"Previous", "Next", "Glossary", "Start"};

        auto controls = createHTextButtons(choices, 4, main_buttonh, startx, SCREEN_HEIGHT * (1.0 - Margin) - main_buttonh);

        controls[0].Type = Control::Type::BACK;
        controls[1].Type = Control::Type::NEXT;
        controls[2].Type = Control::Type::ACTION;
        controls[3].Type = Control::Type::NEW;

        TTF_Init();

        auto font = TTF_OpenFont("fonts/default.ttf", font_size);

        if (font)
        {
            while (!quit)
            {
                renderAdventurer(window, renderer, font, Character::Classes[character]);

                renderTextButtons(renderer, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

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

                        quit = true;

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

                    selected = false;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;

            TTF_Quit();
        }
    }

    return player;
}

std::vector<Button> createItemControls(Character::Base &player)
{
    auto text_space = 8;
    auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

    auto controls = std::vector<Button>();

    controls.clear();

    for (auto idx = 0; idx < player.Items.size(); idx++)
    {
        auto text = createText(Item::Descriptions[player.Items[idx]], "fonts/default.ttf", 16, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

        auto y = texty + (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H : 2 * text_space);

        controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < player.Items.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));

        controls[idx].W = textwidth + button_space;
        controls[idx].H = text->h;
    }

    auto idx = controls.size();

    auto button_map = Button(idx, "images/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP);
    auto button_disk = Button(idx + 1, "images/disk.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME);
    auto button_user = Button(idx + 2, "images/user.png", idx + 1, idx + 3, idx - 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER);
    auto button_items = Button(idx + 3, "images/items.png", idx + 2, idx + 4, idx - 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE);
    auto button_back = Button(idx + 4, "images/back-button.png", idx + 3, idx + 4, idx - 1, idx + 4, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK);

    controls.push_back(button_map);
    controls.push_back(button_disk);
    controls.push_back(button_user);
    controls.push_back(button_items);
    controls.push_back(button_back);

    return controls;
}

bool inventoryScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Control::Type mode)
{
    if (player.Items.size() > 0)
    {
        auto error = false;
        const char *message = NULL;

        Uint32 start_ticks = 0;
        Uint32 message_duration = 5000;

        auto done = false;

        auto text_space = 8;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto controls = createItemControls(player);

        TTF_Init();

        auto font = TTF_OpenFont("fonts/default.ttf", 16);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        while (!done)
        {
            SDL_SetWindowTitle(window, "Necklace of Skulls: Possessions");

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < message_duration)
                {
                    putText(renderer, message, font, 8, clrWH, intRD, TTF_STYLE_NORMAL, splashw, 150, startx, starty);
                }
                else
                {
                    error = false;
                }
            }

            if (!error)
            {
                if (mode == Control::Type::DROP)
                {
                    putText(renderer, "You are carrying too many items. Select an item to DROP.", font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 75, startx, starty);
                }
                else if (mode == Control::Type::USE)
                {
                    putText(renderer, "Select an item to USE", font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 75, startx, starty);
                }
                else
                {
                    putText(renderer, "You are carrying these items", font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 75, startx, starty);
                }
            }

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, 8, 4);

            for (auto i = 0; i < player.Items.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 16, controls[i].H + 16, controls[i].X - 8, controls[i].Y - 8, intDB);
                }
            }

            done = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < player.Items.size())
                    {
                        auto item = player.Items[current];

                        if (mode == Control::Type::DROP)
                        {
                            Character::LOSE_ITEM(player, {item});

                            controls.clear();

                            controls = createItemControls(player);
                        }
                        else if (mode == Control::Type::USE)
                        {
                        }
                        else
                        {
                        }
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    renderWindow(window, renderer, characterScreen, player);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    renderWindow(window, renderer, mapScreen);

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

            TTF_Quit();
        }
    }

    return false;
}

bool shopScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    if (story->Shop.size() > 0)
    {
        auto error = false;
        auto purchased = false;
        std::string message;

        Uint32 start_ticks = 0;
        Uint32 message_duration = 3000;

        auto done = false;

        auto controls = std::vector<Button>();

        auto text_space = 8;

        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        auto idx = 0;

        for (auto i = story->Shop.begin(); i != story->Shop.end(); i++)
        {
            auto item = i->first;
            auto price = i->second;

            std::string choice = std::string(Item::Descriptions[item]) + " (" + std::to_string(price) + " cacao)";

            auto text = createText(choice.c_str(), "fonts/default.ttf", 16, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = texty + (idx > 0 ? controls[idx - 1].Y + controls[idx - 1].H : 2 * text_space);

            controls.push_back(Button(idx, text, idx, idx, (idx > 0 ? idx - 1 : idx), (idx < story->Shop.size() ? idx + 1 : idx), textx + 2 * text_space, y, Control::Type::ACTION));
            controls[idx].W = textwidth + button_space;
            controls[idx].H = text->h;

            idx++;
        }

        controls.push_back(Button(idx, "images/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "images/disk.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "images/user.png", idx + 1, idx + 3, idx - 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "images/items.png", idx + 2, idx + 4, idx - 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "images/back-button.png", idx + 3, idx + 4, idx - 1, idx + 4, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont("fonts/default.ttf", 16);

        auto selected = false;
        auto current = -1;
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

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
                if ((SDL_GetTicks() - start_ticks) < message_duration)
                {
                    putText(renderer, message.c_str(), font, 8, clrWH, intRD, TTF_STYLE_NORMAL, splashw, 150, startx, starty);
                }
                else
                {
                    error = false;
                }
            }
            else if (purchased)
            {
                if ((SDL_GetTicks() - start_ticks) < message_duration)
                {
                    putText(renderer, message.c_str(), font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 150, startx, starty);
                }
                else
                {
                    purchased = false;
                }
            }

            if (!error && !purchased)
            {
                putText(renderer, "Select an item to buy", font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 150, startx, starty);
            }

            putText(renderer, "Money", font, 8, clrWH, intDB, TTF_STYLE_NORMAL, splashw, 36, startx, starty + text_bounds - 111);
            putText(renderer, (std::to_string(player.Money) + std::string(" cacao")).c_str(), font, 8, clrBK, intBE, TTF_STYLE_NORMAL, splashw, 75, startx, starty + text_bounds - 75);

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, 8, 4);

            for (auto i = 0; i < story->Shop.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 16, controls[i].H + 16, controls[i].X - 8, controls[i].Y - 8, intDB);
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
                            if (Item::IsUnique(item) && Character::VERIFY_ITEM(player, item))
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
                                    renderWindow(window, renderer, inventoryScreen, player, Control::Type::DROP);
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
                    renderWindow(window, renderer, characterScreen, player);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    renderWindow(window, renderer, inventoryScreen, player, Control::Type::USE);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    renderWindow(window, renderer, mapScreen);

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

            TTF_Quit();
        }
    }

    return false;
}

Story::Base *processChoices(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    Story::Base *next = &notImplemented;

    auto error = false;
    const char *message = NULL;

    Uint32 start_ticks = 0;
    Uint32 message_duration = 5000;

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
        auto quit = false;
        auto scrollUp = false;
        auto scrollDown = false;
        auto hold = false;

        auto controls = std::vector<Button>();

        auto text_space = 8;
        auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 2 * text_space;

        for (int i = 0; i < choices.size(); i++)
        {
            auto text = createText(choices[i].Text, "fonts/default.ttf", 20, clrBK, textwidth + button_space, TTF_STYLE_NORMAL);

            auto y = texty + (i > 0 ? controls[i - 1].Y + controls[i - 1].H : 2 * text_space);

            controls.push_back(Button(i, text, i, i, (i > 0 ? i - 1 : i), (i < choices.size() ? i + 1 : i), textx + 16, y, Control::Type::ACTION));
            controls[i].W = textwidth + button_space;
            controls[i].H = text->h;
        }

        auto idx = choices.size();

        controls.push_back(Button(idx, "images/map.png", idx - 1, idx + 1, idx - 1, idx, startx, buttony, Control::Type::MAP));
        controls.push_back(Button(idx + 1, "images/disk.png", idx, idx + 2, idx - 1, idx + 1, startx + gridsize, buttony, Control::Type::GAME));
        controls.push_back(Button(idx + 2, "images/user.png", idx + 1, idx + 3, idx - 1, idx + 2, startx + 2 * gridsize, buttony, Control::Type::CHARACTER));
        controls.push_back(Button(idx + 3, "images/items.png", idx + 2, idx + 4, idx - 1, idx + 3, startx + 3 * gridsize, buttony, Control::Type::USE));
        controls.push_back(Button(idx + 4, "images/back-button.png", idx + 3, idx + 4, idx - 1, idx + 4, (1 - Margin) * SCREEN_WIDTH - buttonw, buttony, Control::Type::BACK));

        TTF_Init();

        auto font = TTF_OpenFont("fonts/default.ttf", 20);

        while (!quit)
        {
            if (story->Title)
            {
                SDL_SetWindowTitle(window, story->Title);
            }

            fillWindow(renderer, intWH);

            if (error)
            {
                if ((SDL_GetTicks() - start_ticks) < message_duration)
                {
                    putText(renderer, message, font, 8, clrWH, intRD, TTF_STYLE_NORMAL, splashw, 150, startx, starty);
                }
                else
                {
                    error = false;

                    if (splash)
                    {
                        renderImage(renderer, splash, startx, starty);
                    }
                }
            }
            else
            {
                if (splash)
                {
                    renderImage(renderer, splash, startx, starty);
                }
            }

            fillRect(renderer, textwidth + arrow_size + button_space, text_bounds, textx, texty, intBE);

            renderButtons(renderer, controls, current, intGR, 8, 4);

            for (auto i = 0; i < story->Choices.size(); i++)
            {
                if (i != current)
                {
                    drawRect(renderer, controls[i].W + 16, controls[i].H + 16, controls[i].X - 8, controls[i].Y - 8, intDB);
                }
            }

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected)
            {
                if (controls[current].Type == Control::Type::ACTION && !hold)
                {
                    if (current >= 0 && current < story->Choices.size())
                    {
                        if (story->Choices[current].Type == Choice::Type::NORMAL)
                        {
                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            quit = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::GET_ITEM)
                        {
                            Character::GET_ITEMS(player, {story->Choices[current].Item});

                            next = (Story::Base *)findStory(story->Choices[current].Destination);

                            quit = true;

                            break;
                        }
                        else if (story->Choices[current].Type == Choice::Type::GIVE_ITEM)
                        {
                            if (Character::VERIFY_ITEM(player, story->Choices[current].Item))
                            {
                                Character::LOSE_ITEM(player, {story->Choices[current].Item});

                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                quit = true;

                                break;
                            }
                            else
                            {
                                message = "You do not have the required item!";
                                start_ticks = SDL_GetTicks();
                                error = true;
                            }
                        }
                        else if (story->Choices[current].Type == Choice::Type::SKILL)
                        {
                            if (Character::VERIFY_SKILL(player, story->Choices[current].Skill))
                            {
                                next = (Story::Base *)findStory(story->Choices[current].Destination);

                                quit = true;

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
                    }
                }
                else if (controls[current].Type == Control::Type::CHARACTER && !hold)
                {
                    renderWindow(window, renderer, characterScreen, player);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::USE && !hold)
                {
                    renderWindow(window, renderer, inventoryScreen, player, Control::Type::USE);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::MAP && !hold)
                {
                    renderWindow(window, renderer, mapScreen);

                    current = -1;

                    selected = false;
                }
                else if (controls[current].Type == Control::Type::BACK && !hold)
                {
                    next = story;

                    quit = true;

                    break;
                }
            }
        }

        if (font)
        {
            TTF_CloseFont(font);

            font = NULL;

            TTF_Quit();
        }

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

bool processStory(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, Story::Base *story)
{
    auto quit = false;

    while (!quit)
    {
        auto run_once = true;

        SDL_Surface *splash = NULL;

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

        if (story->Image)
        {
            splash = createImage(story->Image);
        }

        if (story->Text)
        {
            auto textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space) - 16;

            text = createText(story->Text, "fonts/default.ttf", 20, clrDB, textwidth, TTF_STYLE_NORMAL);
        }

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
                    SDL_SetWindowTitle(window, (std::string("Necklace of Skulls: ") + std::to_string(story->ID)).c_str());
                }

                // Fill the surface with background color
                fillWindow(renderer, intWH);

                if (story->Image)
                {
                    renderImage(renderer, splash, startx, texty);
                }

                if (story->Text)
                {
                    fillRect(renderer, textwidth, text_bounds, textx, texty, intBE);
                    renderText(renderer, text, intBE, textx + 8, texty + 8, text_bounds - 16, offset);
                }

                renderButtons(renderer, story->Controls, current, intGR, border_space, border_pts);

                bool scrollUp = false;
                bool scrollDown = false;

                quit = Input::GetInput(renderer, story->Controls, current, selected, scrollUp, scrollDown, hold);

                if ((selected && current >= 0 && current < story->Controls.size()) || scrollUp || scrollDown || hold)
                {
                    if (story->Controls[current].Type == Control::Type::SCROLL_UP || (story->Controls[current].Type == Control::Type::SCROLL_UP && hold) || scrollUp)
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
                    else if (story->Controls[current].Type == Control::Type::SCROLL_DOWN || ((story->Controls[current].Type == Control::Type::SCROLL_DOWN && hold) || scrollDown))
                    {
                        if (text->h >= text_bounds - 16)
                        {
                            if (offset < text->h - text_bounds + 16)
                            {
                                offset += scrollSpeed;
                            }

                            if (offset > text->h - text_bounds + 16)
                            {
                                offset = text->h - text_bounds + 16;
                            }
                        }
                    }
                    else if (story->Controls[current].Type == Control::Type::CHARACTER && !hold)
                    {
                        renderWindow(window, renderer, characterScreen, player);

                        current = -1;

                        selected = false;
                    }
                    else if (story->Controls[current].Type == Control::Type::MAP && !hold)
                    {
                        renderWindow(window, renderer, mapScreen);

                        current = -1;

                        selected = false;
                    }
                    else if (story->Controls[current].Type == Control::Type::USE && !hold)
                    {
                        renderWindow(window, renderer, inventoryScreen, player, Control::Type::USE);

                        current = -1;

                        selected = false;
                    }
                    else if (story->Controls[current].Type == Control::Type::SHOP && !hold)
                    {
                        renderWindow(window, renderer, shopScreen, player, story);

                        current = -1;

                        selected = false;
                    }
                    else if (story->Controls[current].Type == Control::Type::NEXT && !hold)
                    {
                        if (player.Life > 0)
                        {
                            while (!Character::VERIFY_POSSESSIONS(player))
                            {
                                renderWindow(window, renderer, inventoryScreen, player, Control::Type::DROP);
                            }

                            current = -1;

                            selected = false;

                            auto next = renderChoices(window, renderer, player, story);

                            if (next->ID != story->ID)
                            {
                                if (story->Bye)
                                {
                                    auto bye = createText(story->Bye, "fonts/default.ttf", 24, clrBK, (SCREEN_WIDTH * (1.0 - 2.0 * Margin)), TTF_STYLE_NORMAL);
                                    auto forward = createImage("images/next.png");

                                    if (bye && forward)
                                    {
                                        fillWindow(renderer, intWH);

                                        renderText(renderer, bye, intBE, (SCREEN_WIDTH - bye->w) / 2, (SCREEN_HEIGHT - bye->h) / 2, SCREEN_HEIGHT, 0);
                                        renderImage(renderer, forward, SCREEN_WIDTH * (1.0 - Margin) - buttonw - button_space, buttony);

                                        SDL_RenderPresent(renderer);

                                        Input::WaitForNext();

                                        SDL_FreeSurface(bye);

                                        bye = NULL;

                                        SDL_FreeSurface(forward);

                                        forward = NULL;
                                    }
                                }

                                story = next;

                                break;
                            }
                        }
                    }
                    else if (story->Controls[current].Type == Control::Type::BACK && !hold)
                    {
                        quit = true;

                        break;
                    }
                }
            }

            if (splash)
            {
                SDL_FreeSurface(splash);

                splash = NULL;
            }

            if (text)
            {
                SDL_FreeSurface(text);

                text = NULL;
            }
        }
    }

    return quit;
}

bool storyScreen(SDL_Window *window, SDL_Renderer *renderer, Character::Base &player, int id)
{
    auto story = (Story::Base *)findStory(id);

    return processStory(window, renderer, player, story);
}

bool mainScreen(SDL_Window *window, SDL_Renderer *renderer)
{
    auto *introduction = "The sole survivor of an expedition brings news of disaster. Your twin brother is lost in the trackless western sierra. Resolving to find out his fate, you leave the safety of your home far behind. Your quest takes you to lost jungle cities, across mountains and seas, and even into the depths of the underworld.\n\nYou will plunge into the eerie world of Mayan myth. You will confront ghosts and gods, bargain for your life against wily demons, find allies and enemies among both the living and the dead. If you are breave enough to survive the dangers of the spirit-haunted western desert, you must still confront the wizard called Necklace of skulls in a deadly contest whose stakes are nothing less than your own soul.";

    auto splash = createImage("images/skulls-cover.png");
    auto text = createText(introduction, "fonts/default.ttf", 20, clrWH, SCREEN_WIDTH * 0.85 - splash->w);

    auto title = "Necklace of Skulls";

    InitializeStories();

    Character::Base Player;
    auto storyID = 0;

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

            renderImage(renderer, splash, startx, starty);
            renderText(renderer, text, intDB, startx * 2 + splash->w, starty, SCREEN_HEIGHT * (1.0 - 2 * Margin), 0);
            renderTextButtons(renderer, controls, "fonts/default.ttf", current, clrWH, intBK, intRD, 20, TTF_STYLE_NORMAL);

            bool scrollUp = false;
            bool scrollDown = false;
            bool hold = false;

            quit = Input::GetInput(renderer, controls, current, selected, scrollUp, scrollDown, hold);

            if (selected && current >= 0 && current < controls.size())
            {
                switch (controls[current].Type)
                {
                case Control::Type::NEW:

                    Player = selectCharacter(window, renderer);

                    renderWindow(window, renderer, storyScreen, Player, storyID);

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::ABOUT:

                    renderWindow(window, renderer, aboutScreen);

                    current = -1;

                    selected = false;

                    break;

                case Control::Type::LOAD:

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

int main(int argc, char **argsv)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    auto title = "Necklace of Skulls";

    createWindow(SDL_INIT_VIDEO, &window, &renderer, title, "images/maya.png");

    auto numGamePads = Input::InitializeGamePads();

    auto quit = false;

    if (window)
    {
        quit = renderWindow(window, renderer, mainScreen);

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
