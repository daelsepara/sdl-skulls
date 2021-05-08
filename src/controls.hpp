#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

enum class ControlType
{
    ANY = 0,
    ABOUT,
    SCROLL_UP,
    SCROLL_DOWN,
    MAP,
    CHARACTER,
    GAME,
    NEW,
    LOAD,
    SAVE,
    NEXT,
    ACTION,
    BACK,
    QUIT
};

class Control
{
public:
    int ID = -1;

    int Left = -1;
    int Right = -1;
    int Up = -1;
    int Down = -1;

    int X = 0;
    int Y = 0;

    int W = 0;
    int H = 0;

    ControlType Type = ControlType::ANY;
};

class TextButton : public Control
{
private:

    void construct(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        ID = id;
        Text = text;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        W = w;
        H = h;
    }
    
public:
    const char *Text = NULL;

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        construct(id, text, left, right, up, down, x, y, w, h);
    }

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h, ControlType type)
    {
        Type = type;

        construct(id, text, left, right, up, down, x, y, w, h);
    }
};

class Button : public Control
{
private:
    SDL_Surface *createImage(const char *file)
    {
        auto surface = IMG_Load(File);

        if (surface == NULL)
        {
            std::cerr << "Unable to load image " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            W = surface->w;
            H = surface->h;
        }

        return surface;
    }

    void construct(int id, const char *file, int left, int right, int up, int down, int x, int y)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;

        Surface = createImage(file);
    }

public:
    const char *File = NULL;

    SDL_Surface *Surface = NULL;

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y)
    {
        construct(id, file, left, right, up, down, x, y);
    }

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, ControlType type)
    {
        Type = type;

        construct(id, file, left, right, up, down, x, y);
    }
};
#endif
