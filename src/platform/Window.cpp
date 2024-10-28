#include "Window.h"
#include <iostream>

uint16_t Window::s_nWindows = 0;

Window::Window(WindowStyle style, bool fullscreen, bool cursorVisible)
    : m_fullscreen(fullscreen), m_cursorVisible(cursorVisible), m_style(style), m_eventSystem(), m_pWindow(nullptr), m_isShowing(false)
{
    if(s_nWindows==0) {
        glfwInit();
    }
    ++s_nWindows;
}

Window::~Window() {
    glfwDestroyWindow(m_pWindow);
    --s_nWindows;

    // destroy remaining windows just in case
    if(s_nWindows==0) {
        glfwTerminate();
    }
}

void Window::setTitle(std::string title) {
    glfwSetWindowTitle(m_pWindow, title.c_str());
}

std::string Window::getTitle() {
    std::string s = glfwGetWindowTitle(m_pWindow);
    return s;
}

void Window::setFullscreen(bool fullscreen) {
    GLFWmonitor* monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    glfwSetWindowMonitor(m_pWindow, monitor, m_windowedPos[0], m_windowedPos[1],
        m_windowedSize[0], m_windowedSize[1], GLFW_DONT_CARE);
        m_fullscreen = fullscreen;
}

void Window::setCursorVisibility(bool visible) {
    m_cursorVisible = visible;
    if(m_cursorVisible) {
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_pWindow);
}

void Window::getSize(int* pWidth, int* pHeight) {
    glfwGetWindowSize(m_pWindow, pWidth, pHeight);
}

void Window::show(std::string title, int width, int height) {
    /* TODO: check errors in parameters */

    if(m_isShowing) {
        std::cout << "WARN: trying to show a window already showed" << std::endl;
        return;
    }

    // settings to create the window
    switch(m_style) {
        case RESIZABLE:
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            break;
        case FIXED:
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
            break;
        case POPUP:
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            break;
        case CUSTOM:
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
            break;
    }
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    /* TODO: add macOS and X11 related hints */

    GLFWmonitor* monitor = m_fullscreen ? glfwGetPrimaryMonitor() : nullptr;

    // window creation
    m_pWindow = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if(m_pWindow==nullptr) {
        std::cout << "ERROR: window failed to be created" << std::endl;
    }

    glfwGetWindowPos(m_pWindow, &m_windowedPos[0], &m_windowedPos[1]);
    glfwGetWindowSize(m_pWindow, &m_windowedSize[0], &m_windowedSize[1]);
    setCursorVisibility(m_cursorVisible);
    glfwSetWindowUserPointer(m_pWindow, this);

    // setting callbacks
    glfwSetKeyCallback(m_pWindow, keyCallback);
    glfwSetCharModsCallback(m_pWindow, charCallback);
    glfwSetDropCallback(m_pWindow, dropCallback);
    glfwSetScrollCallback(m_pWindow, scrollCallback);
    glfwSetCursorEnterCallback(m_pWindow, cursorEnterCallback);
    glfwSetCursorPosCallback(m_pWindow, cursorPosCallback);
    glfwSetMouseButtonCallback(m_pWindow, mouseButtonCallback);
    glfwSetWindowCloseCallback(m_pWindow, windowCloseCallback);
    glfwSetWindowSizeCallback(m_pWindow, windowSizeCallback);
    glfwSetWindowFocusCallback(m_pWindow, windowFocusCallback);
    glfwSetWindowIconifyCallback(m_pWindow, windowIconifyCallback);

    m_isShowing = true;
}

void Window::close() const {
    glfwSetWindowShouldClose(m_pWindow, GLFW_FALSE);
}

void Window::forEachEvent(const std::function<void(Event&)>& action) {
    m_eventSystem.forEach(action);
}


// event callbacks related functions
void Window::addKeyEvent(int key, int scancode, int action, int mods) {
    m_eventSystem.addEvent<KeyEvent>(key, scancode, action, mods);
}

void Window::addCharEvent(uint32_t codepoint, int mods) {
    m_eventSystem.addEvent<CharEvent>(codepoint, mods);
}

void Window::addDropEvent(int count, const char* paths[]) {
    m_eventSystem.addEvent<DropEvent>(count, paths);
}

void Window::addScrollEvent(double dx, double dy) {
    m_eventSystem.addEvent<ScrollEvent>(dx, dy);
}

void Window::addCursorEnterEvent(int entered) {
    m_eventSystem.addEvent<CursorEnterEvent>(entered);
}

void Window::addCursorPosEvent(double x, double y) {
    m_eventSystem.addEvent<CursorPosEvent>(x, y);
}

void Window::addMouseButtonEvent(int button, int action, int mods) {
    m_eventSystem.addEvent<MouseButtonEvent>(button, action, mods);
}

void Window::addWindowCloseEvent() {
    m_eventSystem.addEvent<WindowCloseEvent>();
}

void Window::addWindowResizeEvent(int width, int height) {
    m_eventSystem.addEvent<WindowResizeEvent>(width, height);
}

void Window::addWindowFocusEvent(int focused) {
    m_eventSystem.addEvent<WindowFocusEvent>(focused);
}

void Window::addWindowIconifyEvent(int iconified) {
    m_eventSystem.addEvent<WindowIconifyEvent>(iconified);
}


// non-member functions

Window* getWindow(GLFWwindow* wnd) {
    return static_cast<Window*>(glfwGetWindowUserPointer(wnd));
}

void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
    getWindow(wnd)->addKeyEvent(key, scancode, action, mods);
}

void charCallback(GLFWwindow* wnd, uint32_t codepoint, int mods) {
    getWindow(wnd)->addCharEvent(codepoint, mods);
}

void dropCallback(GLFWwindow* wnd, int count, const char* paths[]) {
    getWindow(wnd)->addDropEvent(count, paths);
}

void scrollCallback(GLFWwindow* wnd, double dx, double dy) {
    getWindow(wnd)->addScrollEvent(dx, dy);
}

void cursorEnterCallback(GLFWwindow* wnd, int entered) {
    getWindow(wnd)->addCursorEnterEvent(entered);
}

void cursorPosCallback(GLFWwindow* wnd, double x, double y) {
    getWindow(wnd)->addCursorPosEvent(x, y);
}

void mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods) {
    getWindow(wnd)->addMouseButtonEvent(button, action, mods);
}

void windowCloseCallback(GLFWwindow* wnd) {
    getWindow(wnd)->addWindowCloseEvent();
}

void windowSizeCallback(GLFWwindow* wnd, int w, int h) {
    getWindow(wnd)->addWindowResizeEvent(w, h);
}

void windowFocusCallback(GLFWwindow* wnd, int focus) {
    getWindow(wnd)->addWindowFocusEvent(focus);
}

void windowIconifyCallback(GLFWwindow* wnd, int iconified) {
    getWindow(wnd)->addWindowIconifyEvent(iconified);
}
