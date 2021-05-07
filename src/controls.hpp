#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

class TextButton
{
public:
    int ID = -1;
    const char *Text = NULL;

    int Left = -1;
    int Right = -1;
    int Up = -1;
    int Down = -1;

    int X = 0;
    int Y = 0;

    int W = 0;
    int H = 0;

    TextButton(int id, const char *text, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        ID = id;
        Text = text;
        Left = left;
        Right = right;
        Up = up;
        Down = right;
        X = x;
        Y = y;
        W = w;
        H = h;
    }
};

class Button
{
public:
    int ID = -1;
    const char *File = NULL;

    int Left = -1;
    int Right = -1;
    int Up = -1;
    int Down = -1;

    int X = 0;
    int Y = 0;

    int W = 0;
    int H = 0;

    SDL_Surface *Surface = NULL;

    Button(int id, const char *file, int left, int right, int up, int down, int x, int y, int w, int h)
    {
        ID = id;
        File = file;
        Left = left;
        Right = right;
        Up = up;
        Down = right;
        X = x;
        Y = y;
        W = w;
        H = h;

        Surface = IMG_Load(File);

        if (Surface == NULL)
        {
            std::cerr << "Unable to load image " << file << "! SDL Error: " << SDL_GetError() << std::endl;
        }
    }

    ~Button()
    {
        // Deallocate the memory that was previously reserved
        //  for this surface.
        if (Surface)
        {
            SDL_FreeSurface(Surface);

            Surface = NULL;
        }
    }
};
#endif
