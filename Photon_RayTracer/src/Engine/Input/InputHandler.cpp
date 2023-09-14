#include <precomp.h>
#include "InputHandler.h"

namespace RinkWilbrink { namespace Engine { namespace Input
{
    bool InputHandler::GetMouseDown(size_t _button) { return keyStates[_button].GetDown(); }
    bool InputHandler::GetMouseUp(size_t _button) { return keyStates[_button].GetUp(); }
    bool InputHandler::GetMouse(size_t _button) { return keyStates[_button].GetPressed(); }

    bool InputHandler::GetKeyDown(size_t _button) { return keyStates[_button].GetDown(); }
    bool InputHandler::GetKeyUp(size_t _button) { return keyStates[_button].GetUp(); }
    bool InputHandler::GetKey(size_t _button) { return keyStates[_button].GetPressed(); }

    Tmpl8::vec2 InputHandler::GetMousePosition()
    {
        return mousePosition;
    }

    //--------------------------------------------------

    void InputHandler::MouseUp(size_t _button)
    {
        keyStates[_button - 1].Up(true);
        keyStates[_button - 1].Pressed(false);
    }
    void InputHandler::MouseDown(size_t _button)
    {
        keyStates[_button - 1].Down(true);
        keyStates[_button - 1].Pressed(true);
    }

    void InputHandler::KeyUp(size_t _button)
    {
        keyStates[_button].Up(true);
        keyStates[_button].Pressed(false);
    }
    void InputHandler::KeyDown(size_t _button)
    {
        keyStates[_button].Down(true);
        keyStates[_button].Pressed(true);
    }

    void InputHandler::MouseMovement(const int& _x, const int& _y)
    {
        mousePosition.x = static_cast<float>(_x);
        mousePosition.y = static_cast<float>(_y);
    }

    void InputHandler::ResetBools()
    {
        for (size_t i = 0; i < keyStates.get_size(); i++)
        {
            keyStates[i].Down(false);
            keyStates[i].Up(false);
        }
    }
}}}