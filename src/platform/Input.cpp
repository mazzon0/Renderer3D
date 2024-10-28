#include "Input.h"
#include <glfw/glfw3.h>

namespace Input {
    bool isKeyDown(const Window& window, int key) {
        return glfwGetKey(window.m_pWindow, key) == GLFW_PRESS;
    }

    void getCursorPos(const Window& window, double* x, double* y) {
        glfwGetCursorPos(window.m_pWindow, x, y);
    }
    
    bool isButtonPressed(const Window& window, int button) {
        return glfwGetMouseButton(window.m_pWindow, button) == GLFW_PRESS;
    }

}