#include "Display.h"

GLFWwindow* initGLFW(int width, int height, string title);

bool initGLEW();
void assignCallbacks(GLFWwindow* window);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void cursorPosCallback(GLFWwindow* window, double x, double y);
void keyCallback(GLFWwindow* window, int key, int action, int scanCode, int modifiersBitField);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int bitmask);

struct
{
    int width;
    int height;
} screen;

struct
{
    double x, y;
    bool isLeftDown;
} mouse;

bool keyStates[GLFW_KEY_LAST + 1];

bool Display::isKeyPressed(const int & key) const
{
    return keyStates[key];
}

bool Display::isLeftMouseDown() const
{
    return mouse.isLeftDown;
}

void Display::mouseCoords(double* x, double* y) const
{
    *x = mouse.x;
    *y = mouse.y;
}

bool Display::initialize(int width, int height, string title)
{
    if (m_window)
    {
        td::log::warning("Cannot reinitialize a display");
    }
    else
    {
        m_window = initGLFW(width, height, title);

        if (m_window && initGLEW())
        {
            assignCallbacks(m_window);

            screen.width = width;
            screen.height = height;

            return true;
        }
    }

    return false;
}

void Display::clearBuffers()
{
    glClear(m_buffers);
}

void Display::update()
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

bool Display::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

GLFWwindow* initGLFW(int width, int height, string title)
{
    GLFWwindow* retVal = NULL;

    if (!glfwInit())
    {
        log::error("Cannot initialize GLFW");
    }
    else
    {
        retVal = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

        if (!retVal)
        {
            log::error("Cannot create GLFW window");
            glfwTerminate();
        }
        else
        {
            glfwMakeContextCurrent(retVal);
        }
    }

    return retVal;
}

bool initGLEW()
{
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);

    printf("Renderer: %s\n", renderer);
    printf("Version : %s\n", version);
    log::lineBreak();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    return true;
}

void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    char num[5];
    sprintf(num, "%d, %d", width, height);
    log::info("Size changed: " + string(num));
    glViewport(0, 0, width, height);

    screen.width = width;
    screen.height = height;
}

void assignCallbacks(GLFWwindow* window)
{
    glfwSetWindowSizeCallback(window, windowSizeCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int bitmask)
{
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            mouse.isLeftDown = action == GLFW_PRESS;
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int modifiersBitField)
{
    switch (action)
    {
    case GLFW_PRESS:
        keyStates[key] = true;
        break;
    case GLFW_RELEASE:
        keyStates[key] = false;
        break;
    }
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
    mouse.x = (x / screen.width * 2.0f) - 1.0f;
    mouse.y = (y / screen.height * -2.0f) + 1.0f;
}
