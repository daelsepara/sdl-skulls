#ifndef __CONTROLS__HPP__
#define __CONTROLS__HPP__
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
#endif
