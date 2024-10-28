
#ifndef _WINDOW
#define _WINDOW

#include "EventSystem.h"
#include <string>
#include <utility>
#include <glfw/glfw3.h>

// forward declarations (ignore)
class Window;
namespace Input {
    bool isKeyDown(const Window&, int);
    void getCursorPos(const Window&, double*, double*);
    bool isButtonPressed(const Window&, int);
}   // forward declarations

enum WindowStyle {
    RESIZABLE,
    FIXED,
    POPUP,
    CUSTOM
};

/* TODO: icon support (glfwSetWindowIcon), cursor (glfwSetCursor) */
class Window {
public:
    Window(WindowStyle style, bool fullscreen = false, bool cursorVisible = true);
    ~Window();

    Window() = delete;
    Window(const Window& wnd) = delete;
    Window& operator = (const Window& wnd) = delete;

    void setTitle(std::string title);
    std::string getTitle();
    void setFullscreen(bool fullscreen);
    bool getFullscreen() const {return m_fullscreen;}
    void setCursorVisibility(bool visible);
    bool getCursorVisibility() const {return m_cursorVisible;}
    bool shouldClose() const;
    void getSize(int* pWidth, int* pHeight);
    void show(std::string title, int width, int height);
    void close() const;

    /* TODO: add macros or functions to make forEachEvent less verbose
        * current usage example:
        *      window.forEachEvent(std::function<void(engine::Event&)>([this](engine::Event& event) {
        *          this->processEvent(event);
        *      }));
        */
    void forEachEvent(const std::function<void(Event&)>& action);

    void addKeyEvent(int key, int scancode, int action, int mods);
    void addCharEvent(uint32_t codepoint, int mods);
    void addDropEvent(int count, const char* paths[]);
    void addScrollEvent(double dx, double dy);
    void addCursorEnterEvent(int entered);
    void addCursorPosEvent(double x, double y);
    void addMouseButtonEvent(int button, int actions, int mods);
    void addWindowCloseEvent();
    void addWindowResizeEvent(int width, int height);
    void addWindowFocusEvent(int focused);
    void addWindowIconifyEvent(int iconified);

private:
    EventSystem m_eventSystem;
    GLFWwindow* m_pWindow;
    int m_windowedPos[2];
    int m_windowedSize[2];
    WindowStyle m_style;
    bool m_fullscreen;
    bool m_cursorVisible;
    bool m_isShowing;

    static uint16_t s_nWindows;

    friend bool Input::isKeyDown(const Window&, int);
    friend void Input::getCursorPos(const Window&, double*, double*);
    friend bool Input::isButtonPressed(const Window&, int);
};

Window* getWindow(GLFWwindow* wnd);
void keyCallback(GLFWwindow* wnd, int key, int scancode, int action, int mods);
void charCallback(GLFWwindow* wnd, uint32_t codepoint, int mods);
void dropCallback(GLFWwindow* wnd, int count, const char* paths[]);
void scrollCallback(GLFWwindow* wnd, double dx, double dy);
void cursorEnterCallback(GLFWwindow* wnd, int entered);
void cursorPosCallback(GLFWwindow* wnd, double x, double y);
void mouseButtonCallback(GLFWwindow* wnd, int button, int action, int mods);
void windowCloseCallback(GLFWwindow* wnd);
void windowSizeCallback(GLFWwindow* wnd, int w, int h);
void windowFocusCallback(GLFWwindow* wnd, int focus);
void windowIconifyCallback(GLFWwindow* wnd, int iconified);

#endif
