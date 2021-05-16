#ifndef __CONSTANTS__HPP__
#define __CONSTANTS__HPP__

#include <SDL.h>

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

// Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const double Margin = 0.05;

const int splashw = 250;
const int startx = (SCREEN_WIDTH * Margin);
const int starty = (SCREEN_HEIGHT * Margin);
const int textx = startx * 2 + splashw;
const int texty = starty;
const int buttonw = 64;
const int buttonh = 64;
const int buttony = (int)(SCREEN_HEIGHT * (1.0 - Margin) - buttonh);

const int button_space = 20;
const int gridsize = buttonw + button_space;
const int border_space = 8;
const int border_pts = 4;
const int arrow_size = 32;
const int text_bounds = SCREEN_HEIGHT * (1.0 - Margin * 2.0) - buttonh - button_space * 2;
const int textwidth = ((1 - Margin) * SCREEN_WIDTH) - (textx + arrow_size + button_space);

#endif
