#ifndef __TD_KEYBOARDSTATE_H__
#define __TD_KEYBOARDSTATE_H__


#include "Display.h"

namespace td
{
    class KeyboardState
    {
    private:
        td::Display* m_bindedDisplay;

    public:
        KeyboardState(td::Display* const display)
        {
            m_bindedDisplay = display;
        }

        bool isKeyPressed(const int & key) const
        {
            return m_bindedDisplay->isKeyPressed(key);
        }

        bool isKeyUp(const int & key) const
        {
            return !isKeyPressed(key);
        }
    };
}

#endif // __TD_KEYBOARDSTATE_H__
