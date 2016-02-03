#ifndef __TD_DISPLAY_H__
#define __TD_DISPLAY_H__

#include <iostream>
#include <stdio.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "log.h"

using namespace std;

using namespace td;

namespace td
{
    class Display
    {
    private:
        GLFWwindow* m_window;
        GLenum m_buffers;

        Display()
        {
            m_buffers = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
        }

    public:

        static Display& instance()
        {
            static Display display;

            return display;
        }

        ~Display()
        {
            glfwDestroyWindow(m_window);
            glfwTerminate();

            td::log::info("Display terminated");
        }

        void setBuffers(GLenum buffers)
        {
            m_buffers = buffers;
        }

        bool initialize(int width, int height, string title);
        void update();
        void clearBuffers();
        bool shouldClose() const;

        void mouseCoords(double* x, double* y) const;

        bool isLeftMouseDown() const;
        bool isKeyPressed(const int & key) const;
    };
}
#endif // __TD_DISPLAY_H__
