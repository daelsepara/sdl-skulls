#ifndef __INPUT__HPP__
#define __INPUT__HPP__

#include <vector>
#include <SDL.h>

template <typename T>
bool getInput(SDL_Window *window, std::vector<T> choices, int &current, bool &selected, bool &scrollUp, bool &scrollDown, bool &hold)
{
    // Update the surface
    SDL_UpdateWindowSurface(window);

    SDL_Event result;

    auto quit = false;

    selected = false;
    scrollUp = false;
    scrollDown = false;

    auto sensitivity = 32000;

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
            if (result.key.keysym.sym == SDLK_PAGEUP)
            {
                scrollUp = true;
                scrollDown = false;

                current = -1;

                break;
            }
            else if (result.key.keysym.sym == SDLK_PAGEDOWN)
            {
                scrollDown = true;
                scrollUp = false;

                current = -1;

                break;
            }
            else if (current < 0)
            {
                current = choices[0].ID;
            }
            else if (result.key.keysym.sym == SDLK_TAB || result.key.keysym.sym == SDLK_KP_TAB || result.key.keysym.sym == SDL_SCANCODE_KP_TAB)
            {
                if (current < 0)
                {
                    current = choices[0].ID;
                }
                else if (current == choices.size() - 1)
                {
                    current = choices[0].ID;
                }
                else if (current >= 0 && current < choices.size() - 1)
                {
                    current = choices[current].Right;
                }
            }
            else if (current >= 0 && current < choices.size())
            {
                if (result.key.keysym.sym == SDLK_LEFT)
                {
                    current = choices[current].Left;
                }
                else if (result.key.keysym.sym == SDLK_RIGHT)
                {
                    current = choices[current].Right;
                }
                else if (result.key.keysym.sym == SDLK_UP)
                {
                    current = choices[current].Up;
                }
                else if (result.key.keysym.sym == SDLK_DOWN)
                {
                    current = choices[current].Down;
                }
                else if (result.key.keysym.sym == SDLK_KP_ENTER || result.key.keysym.sym == SDLK_RETURN || result.key.keysym.sym == SDLK_RETURN2)
                {
                    selected = true;
                }
            }

            SDL_Delay(100);

            break;
        }
        else if (result.type == SDL_CONTROLLERAXISMOTION)
        {
            if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
            {
                if (current < 0)
                {
                    current = choices[0].ID;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (current >= 0 && current < choices.size())
                    {
                        current = choices[current].Left;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (current >= 0 && current < choices.size())
                    {
                        current = choices[current].Right;
                    }
                }

                SDL_Delay(100);

                break;
            }
            else if (result.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
            {
                if (current < 0)
                {
                    current = choices[0].ID;
                }
                else if (result.caxis.value < -sensitivity)
                {
                    if (current >= 0 && current < choices.size())
                    {
                        current = choices[current].Up;
                    }
                }
                else if (result.caxis.value > sensitivity)
                {
                    if (current >= 0 && current < choices.size())
                    {
                        current = choices[current].Down;
                    }
                }

                SDL_Delay(100);

                break;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONDOWN && result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
        {
            hold = true;

            if (current >= 0 && current < choices.size() && (choices[current].Type == ControlType::SCROLL_UP || choices[current].Type == ControlType::SCROLL_DOWN))
            {
                selected = true;

                break;
            }
        }
        else if (result.type == SDL_CONTROLLERBUTTONUP)
        {
            selected = false;
            hold = false;

            if (current < 0)
            {
                current = choices[0].ID;
            }
            else if (current >= 0 && current < choices.size())
            {
                if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT)
                {
                    current = choices[current].Left;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT)
                {
                    current = choices[current].Right;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP)
                {
                    current = choices[current].Up;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
                {
                    current = choices[current].Down;
                }
                else if (result.cbutton.button == SDL_CONTROLLER_BUTTON_A)
                {
                    selected = true;
                }
            }

            SDL_Delay(100);

            break;
        }
        else if (result.type == SDL_MOUSEMOTION)
        {
            hold = false;

            for (auto i = 0; i < choices.size(); i++)
            {
                if (result.motion.x >= choices[i].X && result.motion.x <= choices[i].X + choices[i].W - 1 && result.motion.y >= choices[i].Y && result.motion.y <= choices[i].Y + choices[i].H - 1)
                {
                    current = choices[i].ID;

                    break;
                }
                else
                {
                    current = -1;
                }
            }

            break;
        }
        else if (result.type == SDL_MOUSEBUTTONDOWN && result.button.button == SDL_BUTTON_LEFT)
        {
            hold = true;

            if (current >= 0 && current < choices.size() && (choices[current].Type == ControlType::SCROLL_UP || choices[current].Type == ControlType::SCROLL_DOWN))
            {
                selected = true;

                break;
            }
        }
        else if (result.type == SDL_MOUSEBUTTONUP && result.button.button == SDL_BUTTON_LEFT)
        {
            if (hold)
            {
                hold = false;

                if (current >= 0 && current < choices.size())
                {
                    selected = true;

                    break;
                }
            }
        }
        else if (result.type == SDL_MOUSEWHEEL)
        {
            current = -1;

            if (result.wheel.y < 0 || result.wheel.x < 0)
            {
                scrollUp = false;
                scrollDown = true;
            }
            else
            {
                scrollUp = true;
                scrollDown = false;
            }

            break;
        }
        else if (hold)
        {
            if (current >= 0 && current < choices.size() && (choices[current].Type == ControlType::SCROLL_UP || choices[current].Type == ControlType::SCROLL_DOWN))
            {
                SDL_Delay(50);

                break;
            }
        }
    }

    return quit;
}
#endif
