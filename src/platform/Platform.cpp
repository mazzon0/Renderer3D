#include "Platform.h"
#include <glfw/glfw3.h>

namespace Platform {
    void sendEvents() {
        glfwPollEvents();
    }
}