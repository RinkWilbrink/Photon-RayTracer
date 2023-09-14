#pragma once
#include "KeyboardKeys.h"
#include "notstd/Array.h"

#pragma warning (disable : 4804) // Unsafe use of Type
// This warning pops up for the Bit Wise operations.

namespace RinkWilbrink { namespace Engine { namespace Input
{
    struct KeyState
    {
        // Return an empty
        static KeyState Empty() { return KeyState(); }

        void Down(const bool& _newState) { states ^= (-_newState ^ states) & (1UL << 0); }
        void Up(const bool& _newState) { states ^= (-_newState ^ states) & (1UL << 1); }
        void Pressed(const bool& _newState) { states ^= (-_newState ^ states) & (1UL << 2); }
        bool GetDown() { return (states >> 0) & 1U; }
        bool GetUp() { return (states >> 1) & 1U; }
        bool GetPressed() { return (states >> 2) & 1U; }

    private:
        byte states = 0;
    };

    class InputHandler
    {
        // Variables
    private:
        notstd::Array<KeyState> keyStates = notstd::Array<KeyState>(KEYCODE_NUM_SCANCODES);

        Tmpl8::vec2 mousePosition = Tmpl8::vec2(0, 0);

        // Functions
    public:
        bool GetKeyDown(size_t _button);
        bool GetKeyUp(size_t _button);
        bool GetKey(size_t _button);
        bool GetMouseDown(size_t _button);
        bool GetMouseUp(size_t _button);
        bool GetMouse(size_t _button);

        Tmpl8::vec2 GetMousePosition();

        void MouseUp(size_t _button);
        void MouseDown(size_t _button);

        void KeyUp(size_t _button);
        void KeyDown(size_t _button);

        void MouseMovement(const int& _x, const int& _y);

        // Reset the Down and Pressed states for all the Keys.
        void ResetBools();
    };
}}}