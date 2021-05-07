#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

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

    bool IsScroll = false;
};

class TextButton : public Control
{
public:
    const char *Text = NULL;

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
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
};

class Button : public Control
{
public:
    const char *File = NULL;

    SDL_Surface *Surface = NULL;

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, bool scroll = false)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = down;
        X = x;
        Y = y;
        IsScroll = scroll;
        
        Surface = IMG_Load(File);

        if (Surface == NULL)
        {
            std::cerr << "Unable to load image " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            W = Surface->w;
            H = Surface->h;
        }
    }
};
#endif
