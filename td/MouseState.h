#ifndef __TD_MOUSE_STATE_H
#define __TD_MOUSE_STATE_H

#include "Display.h"

namespace td
{
    class MouseState
    {
    private:
        td::Display const * m_bindedDisplay;

        double m_x, m_y, m_dx, m_dy;

        bool m_isLeftButtonDown;

    public:
        MouseState(td::Display const * const display)
        {
            m_bindedDisplay = display;
        }

        void update()
        {
            double lastX = m_x;
            double lastY = m_y;

            m_bindedDisplay->mouseCoords(&m_x, &m_y);

            m_dx = m_x - lastX;
            m_dy = m_y - lastY;

            m_isLeftButtonDown = m_bindedDisplay->isLeftMouseDown();
        }

        double const & x() const { return m_x; }
        double const & y() const { return m_y; }
        double const & dx() const { return m_dx; }
        double const & dy() const { return m_dy; }
        bool const & isLeftButtonDown() const { return m_isLeftButtonDown; }
    };
}

#endif // __TD_MOUSE_STATE_H
